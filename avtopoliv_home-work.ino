//Created by AK (Artem Karchenko) in 07.10.2017;

#include "LowPower.h"

#define version 1.35

#define led 11          //светодиод
#define pomp 3          //помпа
#define sensorPin A1    //датчик влажности
#define sensorPower  10  //плюс датчика
#define waterPower  8  //плюс датчика воды
#define waterMinusPower  9  //минус датчика воды
#define wetPin A0       //граница влажности
#define buzzer 4       //бузер 
boolean woter = 0;        //наличие датчика воды (1 - есть, 0 - нет)
#define waterPin 2     //если не используется, то неважно

#define Time 512       //максимальное время полива сек --(можно менять)

int realTime = Time;
int realWater = 0;


void sleep(){
  Serial.print ("\t\tsystem: Sleep...\n");
  digitalWrite (sensorPower, LOW);
  digitalWrite (waterPower, LOW);
  delay (50);                                      //для стабильности
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // спать 8 секунд
  }

void wake_up(){
  Serial.println ("\tsystem: Wake up!!!\n");
  digitalWrite (sensorPower, HIGH);
  digitalWrite (waterPower, HIGH);
 }

void water(){
  
  if (woter == 0){
    realWater = 1;       //имитация наличия воды
  }
 
  else{
    realWater = !digitalRead (waterPin);     //читает пин с датчиком
  }
}

void setup(){
  
  Serial.begin(9600);
  Serial.println ("-------------------------------------");                    // приветствие
  Serial.print ("version: avtopoliv_home-work --- "); Serial.println (version);
  Serial.println ("-------------------------------------");
  Serial.println ("Working...");
  
  pinMode (led, OUTPUT);
  pinMode (pomp, OUTPUT);
  pinMode (sensorPin, INPUT);
  pinMode (buzzer, OUTPUT);
  pinMode (waterPin ,INPUT_PULLUP);
  pinMode (waterPower, OUTPUT);
  pinMode (sensorPower, OUTPUT);
  pinMode (waterMinusPower, OUTPUT);
  digitalWrite (waterMinusPower, LOW);
  digitalWrite (pomp, HIGH);
 
 delay (1000);         // для феншуя (можно убрать)
}

void loop(){
 water();                 //имитация датчика воды
 wake_up();               //проснуться
 delay (50);              //для стабильности
 
 int wet = analogRead (wetPin);                //порог влажности
 int val = analogRead (sensorPin);             //датчик

  if ((val < wet) && (realTime != 0) && (realWater == 1)){  //когда поливает()
    Serial.print (val);
    Serial.print ("\tmin:");
    Serial.print (wet);
    Serial.print ("\ttime: ");
    Serial.print (realTime);
    
    digitalWrite (pomp, HIGH);
    realTime--;                          //уменьшается оставшееся время
  }

  else if ((realTime == 0) || (realWater == 0)){       //ошибка
    Serial.print (val); 
    Serial.print ("\ttime:");
    Serial.println (realTime);
          
   while (1){ 
     digitalWrite (pomp, LOW);           //выключает помпу
     Serial.println ("error...");        //пишет ошибку
     digitalWrite (buzzer, HIGH);             //бузер на 13 пине
     sleep();
     digitalWrite (buzzer, LOW);             //бузер на 13 пине
     sleep();
   }
  }
  
  
  else {                         //не поливает (нет ошибок)
    Serial.print (val);
    Serial.print ("\tmin:");
    Serial.print (wet);
    Serial.print ("\ttime: ");
    Serial.print (realTime);
    
    digitalWrite (pomp, LOW);            //выключает помпу
    realTime = Time;                     //восстанавливаем время
    sleep();
  }

 delay (10);                              //для стабильности
}

