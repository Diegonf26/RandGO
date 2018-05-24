#include <NewPing.h>
#include "Timer.h"

//la libreria Tone usa el timer 1
//La libreria timer utiliza el timer 2
//Motor vibracion timer 0
 
/*Aqui se configuran los pines donde debemos conectar el sensor*/
#define TRIGGER_PIN  9
#define ECHO_PIN     8
#define sal_buzzer 10          //Pin de Tono de timer 1 (No mover)
#define sal_vibrar 6
#define pul_opcion_salida 4   //

#define MAX_DISTANCE 200      //[cm]
#define time_obt_dist 300
#define lim_max_dist 150      //Limite para detectar [cm]


Timer t;

volatile unsigned int opc_salida=0;
volatile unsigned int dist_ultrasonico=3;
volatile unsigned int volu_buzzer=10;        //Valor de OCR1A
volatile unsigned int val_pulsador;
volatile unsigned int conta=0;
volatile bool mem=false;
volatile bool mem_c=false;
volatile bool mem_m=false;
volatile bool mem_l=false;
volatile unsigned int mem2=0;

volatile int cont_vibrar=0;
volatile bool funcion=false;
 
/*Crear el objeto de la clase NewPing*/
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

///////////////////////Interrupciones//////////////////////
void selec_opc_salida(){
  val_pulsador = digitalRead(pul_opcion_salida);  //lectura de pulsador
  if (val_pulsador == LOW&&mem==false)
  {
    val_pulsador = digitalRead(pul_opcion_salida);  //lectura de pulsador
    if (val_pulsador== LOW)
    {
      mem=true;
      if(cont_vibrar>=30 && funcion==false){
        funcion=true;
        cont_vibrar=0;
        
      }
      else if(cont_vibrar>=30 && funcion==true){
        funcion=false;
        cont_vibrar=0;
      }

      if (funcion==false){
        switch(opc_salida){
          case 0:
          opc_salida=1;
          Serial.println("Opcion sonido");
          break;
          case 1:
          opc_salida=0;
          Serial.println("apagado");
          break;
          default:
          opc_salida=0;
          break;
        }
      }
      else {
        switch(opc_salida){
          case 2:
          opc_salida=3;
          Serial.println("Opcion sonido");
          break;
          case 3:
          opc_salida=0;
          Serial.println("apagado");
          break;
          case 0:
          opc_salida=2;
          Serial.println("apagado");
          break;
          default:
          opc_salida=0;
          break;
        }
    }
    delay(20);
  }
  else if(val_pulsador == HIGH&&mem==true){
    mem=false;
    cont_vibrar=0;
  }
}
}
 
void setup() {
  Serial.begin(9600);
    //Configuracion de Pines
  pinMode(sal_buzzer, OUTPUT);
  pinMode(sal_vibrar, OUTPUT);
  pinMode(pul_opcion_salida, INPUT_PULLUP);
  delay(1000);
  int tickEvent = t.every(time_obt_dist,obt_dist);
}
 
void loop() {
  // Esperar 1 segundo entre mediciones
  t.update();
  selec_opc_salida();
  switch(opc_salida){
    case 2:
    if(dist_ultrasonico>1&&dist_ultrasonico<=lim_max_dist){
      on_motor_vibrar(70);//255-0.7*dist_ultrasonico);
      off_buzzer();
    }
    else{
      off_motor_vibrar();
      mem2=0;
      off_buzzer();
    }
    break;
    case 1:
    if(dist_ultrasonico>1&&dist_ultrasonico<=lim_max_dist){
      off_motor_vibrar();
      pitido();
    }
    else {
      off_motor_vibrar();
      mem2=0;
      off_buzzer();
    }
    break;
    case 3:
    if(dist_ultrasonico>1&&dist_ultrasonico<=lim_max_dist){
      on_motor_vibrar(70);//255-0.7*dist_ultrasonico);
      pitido();
    }
    else {
      off_motor_vibrar();
      mem2=0;
      off_buzzer();
    }
    break;
    case 0:
    off_motor_vibrar();
    off_buzzer();
    mem2=0;
    break;
    default:
    Serial.println("Error de opcion");
    break;
  } 
}

void obt_dist(){
  // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
  dist_ultrasonico = (sonar.ping_median())/US_ROUNDTRIP_CM;
 
  Serial.print("Dist: ");
  Serial.print(dist_ultrasonico);
  Serial.print("ontador: ");
  Serial.println(conta);  
  conta+=1;
  if(conta>=12) conta=0;

  cont_vibrar++;

  
}

void on_motor_vibrar(int valor){
  analogWrite(sal_vibrar, valor);
}
void off_motor_vibrar(){
  analogWrite(sal_vibrar, 0);
}

void on_buzzer(){
  analogWrite(sal_buzzer, 255);
}
void off_buzzer(){
  analogWrite(sal_buzzer,0);
}

void pitido(){
  if(dist_ultrasonico>=1&&dist_ultrasonico<50){
    if(mem2!=1){
      conta=0;
      mem2=1;
    }
      switch (conta){
      case 0:
        on_buzzer(); 
      break;
      case 1:
        off_buzzer();  
      break;
      case 2:
        on_buzzer();
      break;
      case 3:
        off_buzzer();
      break;
      case 4:
        on_buzzer();  
      break;
      case 5:
        off_buzzer();
      break;
      case 6:
        on_buzzer();
      break;
      case 7:
        off_buzzer();
      break;
      case 8:
        on_buzzer();  
      break;
      case 9:
        off_buzzer();
      break;
      case 10:
        on_buzzer();
      break;
      case 11:
        off_buzzer();
      break;
    
    }
  }
  else if(dist_ultrasonico>=50 && dist_ultrasonico<100)
  {
     if(mem2!=2){
      conta=0;
      mem2=2;
     }
      switch (conta){
      case 0:
        on_buzzer(); 
      break;
      case 1:
        off_buzzer();  
      break;
      case 3:
        on_buzzer();
      break;
      case 4:
        off_buzzer();
      break;
      case 6:
        on_buzzer();  
      break;
      case 7:
        off_buzzer();
      break;
      case 9:
        on_buzzer();  
      break;
      case 10:
        off_buzzer();
      break;
    }
  }
  else if(dist_ultrasonico>=100 && dist_ultrasonico<=150)
  {
      if(mem2!=3){
      conta=0;
      mem2=3;
      }
      switch (conta){
      case 0:
        on_buzzer(); 
      break;
      case 1:
        off_buzzer();  
      break;
      case 4:
        on_buzzer();  
      break;
      case 5:
        off_buzzer();
      break;
      case 8:
        on_buzzer();  
      break;
      case 9:
        off_buzzer();
      break;
    }
  }
}
