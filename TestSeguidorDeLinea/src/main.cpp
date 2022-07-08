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
const int si_Iz = A0;
const int si_De = A1;
const int si_DeE = A2;
const int si_IzE = A3; 


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

  //Iniciar pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  delay(10);

}

void loop() {
  int Data_siIz = analogRead(si_Iz);
  int Data_siDe = analogRead(si_De);
  int Data_siIzE = analogRead(si_IzE);
  int Data_siDeE = analogRead(si_DeE);

  //**************************************************************************************************************************
  //Imprimir valores en pantalla
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("Sensores Izquierdos= ");
  
  display.setCursor(0,10);
  display.setTextSize(2);
  display.print(Data_siIz);

  display.setCursor(50,10);
  display.setTextSize(2);
  display.print(Data_siIzE);

  display.setCursor(0,35);
  display.setTextSize(1);
  display.print("Sensores Derechos= ");
  
  display.setCursor(0,45);
  display.setTextSize(2);
  display.print(Data_siDe);

  display.setCursor(50,45);
  display.setTextSize(2);
  display.print(Data_siDe);

  display.display();


  
  //**************************************************************************************************************************

  //Si la lectura analogica es menor de 40 es porque detecta blanco o nada
  //Si la lectura es mayor a 40 es porque detecta negro o algo
  
  if(Data_siIz < 40 && Data_siDe < 40){                   //Ambos sensores detectan blanco -> Ir adelante
    adelante();
    
  }else if(Data_siIz > 40 && Data_siDe < 40){             //Sensor izquierdo detecta negro -> Moverse hacia la izquierda
    derecha();
    delay(100);
  }else if(Data_siIz < 40 && Data_siDe > 40){             //Sensor derecho detecta negro  -> Moverese a la derecha
    izquierda();
    delay(100);
  }else if(Data_siIzE < 40){
    izquierda();
    delay(200);
  }else if(Data_siDeE < 40){
    derecha();
    delay(200);
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


