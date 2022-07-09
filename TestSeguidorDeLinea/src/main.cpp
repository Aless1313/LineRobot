//Código de testeo de seguidor de linea
//07072022

#include <Arduino.h>

//Librerias de pantalla
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Definicion de tamaño de pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Pines de motores
//Motores del lado izquierdo
const int Iz1 = 7;
const int Iz2 = 6;

//Motores del lado derecho
const int De1 = 5;
const int De2 = 4;

const int vIz = 10;   //Velocidad de motores izquierdos
const int vDe = 9;    //velocidad de motores derechos

//Sensores infrarrojos
const int si_Iz = A0;   //Izquierdo
const int si_De = A1;   //Derecho
const int si_DeE = A2;  //Derecho Externo
const int si_IzE = A3;  //Izquierdo Externo
const int si_Fr = A4;   //Frontal

//Valores de frecuencia para sensor de color
const int s0D = 22;
const int s1D = 23;
const int s2D = 24;       //Sensor de color izquierdo
const int s3D = 25;
const int outD = 26;

const int s0I = 27;
const int s1I = 28;
const int s2I = 29;       //Sensor de color derecho
const int s3I = 30;
const int outI = 31;

int redD = 0;
int greenD = 0;
int blueD = 0;

int redI = 0;
int greenI = 0;
int blueI = 0;



//funciones
void adelante();
void izquierda();
void derecha();


void setup() {
  
  //Definicion de pines

  pinMode(Iz1, OUTPUT);
  pinMode(Iz2, OUTPUT);
  pinMode(De1, OUTPUT);
  pinMode(De2, OUTPUT);
  pinMode(vIz, OUTPUT);
  pinMode(vDe, OUTPUT);

  pinMode(si_Iz, INPUT);
  pinMode(si_De, INPUT);
  pinMode(si_IzE, INPUT);
  pinMode(si_DeE, INPUT);
  pinMode(si_Fr, INPUT);

  pinMode(s0D, OUTPUT);
  pinMode(s1D, OUTPUT);
  pinMode(s2D, OUTPUT);
  pinMode(s3D, OUTPUT);
  pinMode(outD, INPUT);
  digitalWrite(s0D, HIGH);
  digitalWrite(s1D, HIGH);

  pinMode(s0I, OUTPUT);
  pinMode(s1I, OUTPUT);
  pinMode(s2I, OUTPUT);
  pinMode(s3I, OUTPUT);
  pinMode(outI, INPUT);
  digitalWrite(s0I, HIGH);
  digitalWrite(s1I, HIGH);
  //***************************************************************************

  //Iniciar pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  delay(10);

}

void loop() {

  //Lectura de infrarojos
  int Data_siIz = analogRead(si_Iz);
  int Data_siDe = analogRead(si_De);
  int Data_siIzE = analogRead(si_IzE);
  int Data_siDeE = analogRead(si_DeE);
  int Data_siFr = analogRead(si_Fr);

  //Lectura de color
  colorReadD();
  int colorD = colorSelectD(redD, greenD, blueD);
  int colorI = colorSelectI(redI, greenI, blueI);
  
  //**************************************************************************************************************************
  printdata(si_Iz, si_IzE, si_De, si_DeE, colorD, colorI);      //Función para imprimir valores en OLED
  //**************************************************************************************************************************

  //Si la lectura analogica es menor de 40 es porque detecta blanco o nada
  //Si la lectura es mayor a 40 es porque detecta negro o algo
  
  if(Data_siIz < 40 && Data_siDe < 40){                   //Ambos sensores detectan blanco -> Ir adelante
    adelante();
    
  }else if(Data_siIz > 40 && Data_siDe < 40){             //Sensor izquierdo detecta negro -> Moverse ligero hacia la izquierda
    derecha();
    delay(100);

  }else if(Data_siIz < 40 && Data_siDe > 40){             //Sensor derecho detecta negro  -> Moverse ligero a la derecha
    izquierda();
    delay(100);

  }else if(Data_siIzE < 40){                              //Linea en limite izquierdo   ->  Moverse brusco a la izquierda
    izquierda();
    delay(200);

  }else if(Data_siDeE < 40){                              //Linea en limite derecho     ->  Moverse brusco a la derecha
    derecha();
    delay(200);
  }else if(Data_siDe < 40 && Data_siDeE < 40 && Data_siIz > 40 && Data_siIzE > 40){            //Vuelta muy cerrada  -> Giro a la derecha cerrado
    //Funcion aqui
  }else if(Data_siIz < 40 && Data_siIzE < 40 && Data_siDe > 40 && Data_siDeE > 40){           //Vuelta muy cerrada   -> Giro a la izquierda cerrado
    //Funcion aqui
  }else if(Data_siDe <40 && Data_siDeE <40 && Data_siIz < 40 && Data_siIzE < 40 && Data_siFr > 40){             //Interseccion tipo T   -> Detenerse y comprobar giro con cuadro verde
    //Funcion aqui
  }else if(Data_siDe < 40 && Data_siDeE < 40 && Data_siIz < 40 && Data_siIzE <40 && Data_siFr < 40){      //Interseccion en cruz  -> Detenerse y comprobar giro con cuadro verde
    //Funcion aqui
  }

}

void adelante(){

  analogWrite(vIz, 60);
  analogWrite(vDe, 60);

  digitalWrite(Iz1, LOW);
  digitalWrite(Iz2, HIGH);    //Iz2 en HIGH es adelante

  digitalWrite(De1, LOW);
  digitalWrite(De2, HIGH);    //De2 en HIGH es adelante

}

void derecha(){
  
  analogWrite(vIz, 55);
  analogWrite(vDe, 55);

  digitalWrite(Iz1, LOW);
  digitalWrite(Iz2, HIGH);

  digitalWrite(De1, HIGH);
  digitalWrite(De2, LOW);

}

void izquierda(){

  analogWrite(vIz, 55);
  analogWrite(vDe, 55);

  digitalWrite(Iz1, HIGH);
  digitalWrite(Iz2, LOW);

  digitalWrite(De1, LOW);
  digitalWrite(De2, HIGH);

}

void printdata(int SI_iz, int SI_izE, int SI_De, int SI_DeE, int colorD, int colorI){
  //Imprimir valores en pantalla
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("Sensores Izquierdos= ");
  
  display.setCursor(0,10);
  display.setTextSize(1);
  display.print(SI_iz);

  display.setCursor(50,10);
  display.setTextSize(1);
  display.print(SI_izE);

  display.setCursor(0,30);
  display.setTextSize(1);
  display.print("Sensores Derechos= ");
  
  display.setCursor(0,45);
  display.setTextSize(1);
  display.print(SI_De);

  display.setCursor(50,45);
  display.setTextSize(1);
  display.print(SI_DeE);

  display.setCursor(0,60);
  display.setTextSize(1);
  display.print(colorD);

  display.setCursor(50,60);
  display.setTextSize(1);
  display.print(colorI);


  display.display();
}

void colorReadD(){
  digitalWrite(s2D,LOW);
  digitalWrite(s3D, LOW);
  redD = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);
  
  digitalWrite(s3D, HIGH);
  blueD = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);

  digitalWrite(s2D, HIGH);
  greenD = pulseIn(outD, digitalRead(outD) == HIGH ? LOW : HIGH);
}

void colorReadI(){
  digitalWrite(s2I,LOW);
  digitalWrite(s3I, LOW);
  redI = pulseIn(outI, digitalRead(outI) == HIGH ? LOW : HIGH);
  
  digitalWrite(s3I, HIGH);
  blueI = pulseIn(outI, digitalRead(outI) == HIGH ? LOW : HIGH);

  digitalWrite(s2I, HIGH);
  greenI = pulseIn(outI, digitalRead(outI) == HIGH ? LOW : HIGH);
}

int colorSelectD(int r, int g, int b){
  if(r < b && g > b && r < 35){
    return 1;                                 //Rojo
  }else if(b < r && b  < g && g < r){
    return 2;                                 //Azul
  }else if(r > g && b > g){
    return 3;                                 //Verde
  }else{
    return 0;
  }
}

int colorSelectI(int r, int g, int b){
  if(r < b && g > b && r < 35){
    return 1;                                 //Rojo
  }else if(b < r && b  < g && g < r){
    return 2;                                 //Azul
  }else if(r > g && b > g){
    return 3;                                 //Verde
  }else{
    return 0;
  }
}



