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

const int btn_calibration = 3;

//Pines de motores
//Motores del lado izquierdo
const int Iz1 = 7;
const int Iz2 = 6;

//Motores del lado derecho
const int De1 = 5;
const int De2 = 4;

const int vIz = 9;   //Velocidad de motores izquierdos
const int vDe = 8;    //velocidad de motores derechos

//Sensores infrarrojos
const int si_Iz = A3;   //Izquierdo
const int si_De = A1;   //Derecho
const int si_DeE = A0;  //Derecho Externo
const int si_IzE = A4;  //Izquierdo Externo
const int si_Fr = A2;   //Frontal

//Valores de frecuencia para sensor de color
const int s0D = 32;
const int s1D = 34;
const int s2D = 38;       //Sensor de color izquierdo
const int s3D = 36;
const int outD = 40;

const int s0I = 30;
const int s1I = 26;
const int s2I = 24;       //Sensor de color derecho
const int s3I = 28;
const int outI = 22;

int redD = 0;  
int greenD = 0;           //Balance de colores en sensor de color Derecho
int blueD = 0;

int redI = 0;
int greenI = 0;           //Balance de colores en sensor izquierdo
int blueI = 0;



//Definición de funciones
void adelante();
void izquierda();
void derecha();
void colorReadD();
void colorReadI();
void printdata(int SI_iz, int SI_izE, int SI_De, int SI_DeE, int colorD, int colorI);
int colorSelectD(int rd, int gd, int bd);
int colorSelectI(int ri, int gi, int bi);


int colorSelectD(int rd, int gd, int bd){
  if(rd < bd && gd > bd && rd < 35){
    return 1;                                 //Rojo
  }else if(bd < rd && bd  < gd && gd < rd){
    return 2;                                 //Azul
  }else if(rd > gd && bd > gd){
    return 3;                                 //Verde
  }else{
    return 0;
  }
}

int colorSelectI(int ri, int gi, int bi){
  if(ri < bi && gi > bi && ri < 35){
    return 1;                                 //Rojo
  }else if(bi < ri && bi  < gi && gi < ri){
    return 2;                                 //Azul
  }else if(ri > gi && bi> gi){
    return 3;                                 //Verde
  }else{
    return 0;
  }
}


void setup() {
  
  //Definicion de pines

  pinMode(Iz1, OUTPUT);
  pinMode(Iz2, OUTPUT);
  pinMode(De1, OUTPUT);       //Motores
  pinMode(De2, OUTPUT);
  pinMode(vIz, OUTPUT);
  pinMode(vDe, OUTPUT);

  pinMode(si_Iz, INPUT);
  pinMode(si_De, INPUT);
  pinMode(si_IzE, INPUT);     //Sensores infrarrojos 
  pinMode(si_DeE, INPUT);
  pinMode(si_Fr, INPUT);

  pinMode(s0D, OUTPUT);
  pinMode(s1D, OUTPUT);
  pinMode(s2D, OUTPUT);         //Sensor de color derecho
  pinMode(s3D, OUTPUT);
  pinMode(outD, INPUT);
  digitalWrite(s0D, HIGH);
  digitalWrite(s1D, HIGH);

  pinMode(s0I, OUTPUT);
  pinMode(s1I, OUTPUT);
  pinMode(s2I, OUTPUT);         //Sensor de color izquierdo
  pinMode(s3I, OUTPUT);
  pinMode(outI, INPUT);
  digitalWrite(s0I, HIGH);
  digitalWrite(s1I, HIGH);

  pinMode(btn_calibration, INPUT);
  //***************************************************************************

  //Iniciar pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();
  delay(500);
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
   display.clearDisplay();
  
  display.setCursor(17, 0);
  display.setTextSize(1);
  display.println("Infrared Sensors");
  
  display.setCursor(6, 10);
  display.setTextSize(1);
  display.println(char(174));

  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(Data_siIzE);      //Lectura sensor izquierdo externo

  display.setCursor(34, 10);
  display.setTextSize(1);
  display.println(char(174));
  
  display.setCursor(29, 20);
  display.setTextSize(1);
  display.print(Data_siIz);       //Lectura sensor izquierdo 

  display.setCursor(63, 10);
  display.setTextSize(1);
  display.print(char(185));

  display.setCursor(58,20);
  display.setTextSize(1);
  display.print(Data_siFr);       //Lectura sensor frontal

  display.setCursor(94, 10);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(85, 20);
  display.setTextSize(1);
  display.print(Data_siDe);         //Lectura sensor derecho

  display.setCursor(117, 10);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(110, 20);
  display.setTextSize(1);
  display.print(Data_siDeE);      //Lectura sensor derecho externo

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print(colorI);

  display.setCursor(40, 40);
  display.setTextSize(1);
  display.print(char(174));

  display.setCursor(52, 40);
  display.setTextSize(1);
  display.print("Color");

  display.setCursor(87, 40);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(98, 40);
  display.setTextSize(1);
  display.print(colorD);

  display.setCursor(0, 55);
  display.setTextSize(1);
  display.print("Distance: ");

  display.setCursor(55, 55);
  display.setTextSize(1);
  display.print("123");

  display.setCursor(75, 55);
  display.setTextSize(1);
  display.print("cm");

  display.display();
  //**************************************************************************************************************************

  //Si la lectura analogica es menor de 40 es porque detecta blanco o nada
  //Si la lectura es mayor a 40 es porque detecta negro o algo
  
  if(Data_siIz < 40 && Data_siDe < 40){                   //Ambos sensores detectan blanco -> Ir adelante
    adelante();
    
  }else if(Data_siIz > 40 && Data_siDe < 40){             //Sensor izquierdo detecta negro -> Moverse ligero hacia la izquierda
    derecha();
    

  }else if(Data_siIz < 40 && Data_siDe > 40){             //Sensor derecho detecta negro  -> Moverse ligero a la derecha
    izquierda();
 

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

  analogWrite(vIz, 70);
  analogWrite(vDe, 70);

  digitalWrite(Iz1, LOW);
  digitalWrite(Iz2, HIGH);    //Iz2 en HIGH es adelante

  digitalWrite(De1, LOW);
  digitalWrite(De2, HIGH);    //De2 en HIGH es adelante

}

void derecha(){
  
  analogWrite(vIz, 60);
  analogWrite(vDe, 0);

  digitalWrite(Iz1, LOW);
  digitalWrite(Iz2, HIGH);

  digitalWrite(De1, LOW);
  digitalWrite(De2, HIGH);

}

void izquierda(){

  analogWrite(vIz, 0);
  analogWrite(vDe, 60);

  digitalWrite(Iz1, LOW);
  digitalWrite(Iz2, HIGH);

  digitalWrite(De1, LOW);
  digitalWrite(De2, HIGH);

}

void printdata(int SI_iz, int SI_izE, int SI_De, int SI_DeE, int colorD, int colorI){
  //Imprimir valores en pantalla
  display.clearDisplay();
  
  display.setCursor(17, 0);
  display.setTextSize(1);
  display.println("Infrared Sensors");
  
  display.setCursor(6, 10);
  display.setTextSize(1);
  display.println(char(174));

  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(SI_izE);      //Lectura sensor izquierdo externo

  display.setCursor(34, 10);
  display.setTextSize(1);
  display.println(char(174));
  
  display.setCursor(29, 20);
  display.setTextSize(1);
  display.print(SI_iz);       //Lectura sensor izquierdo 

  display.setCursor(63, 10);
  display.setTextSize(1);
  display.print(char(185));

  display.setCursor(58,20);
  display.setTextSize(1);
  display.print(si_Fr);       //Lectura sensor frontal

  display.setCursor(94, 10);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(85, 20);
  display.setTextSize(1);
  display.print(si_De);         //Lectura sensor derecho

  display.setCursor(117, 10);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(110, 20);
  display.setTextSize(1);
  display.print(si_DeE);      //Lectura sensor derecho externo

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print("Green");

  display.setCursor(40, 40);
  display.setTextSize(1);
  display.print(char(174));

  display.setCursor(52, 40);
  display.setTextSize(1);
  display.print("Color");

  display.setCursor(87, 40);
  display.setTextSize(1);
  display.print(char(175));

  display.setCursor(98, 40);
  display.setTextSize(1);
  display.print("Green");

  display.setCursor(0, 55);
  display.setTextSize(1);
  display.print("Distance: ");

  display.setCursor(55, 55);
  display.setTextSize(1);
  display.print("123");

  display.setCursor(75, 55);
  display.setTextSize(1);
  display.print("cm");

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


