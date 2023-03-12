/*============================================================================================================
Descprition:  Alarm (Zimmer Guardian)
Author:       Jan Syed
Date:         26.02.2023
Version:      Alpha
============================================================================================================*/

#include "stammdaten.h"

void setup() {
  Serial.begin (9600);
  pinMode(buzzer, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(g_led, OUTPUT);
  pinMode(r_led, OUTPUT);

  pinMode(tast1, INPUT);
  pinMode(tast2, INPUT);

}

void loop() {
  
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);

  entfern = (dauer/2) * 0.03432;

  status_tast1=digitalRead(tast1);
  status_tast2=digitalRead(tast2);

  if(status_tast2 == HIGH)
  {
    ein_status = 1;
  }




  //Check Entfernung
  if (entfern >= 2000 || entfern <= 0)
  {
    Serial.println("Kein Messwert");
  }
  else
  {
    Serial.println(entfern);
    Serial.println("cm");
  }

  //Set Alarm
  if (entfern <= 30)
  {
    alarm_status = 1; 
  }
  
  //Alarm off
  if (status_tast1 == HIGH && alarm_status == 1)
  {
    alarm_status = 0;
    tone(buzzer, 200);
    delay(250); 
    noTone(buzzer);

    for(int i = 0; i < 20; i++)
    {
      digitalWrite(r_led, HIGH);
      delay(175);
      digitalWrite(r_led, LOW);
      delay(175);
    }
    
    tone(buzzer, 200);
    delay(250); 
    noTone(buzzer);
    

  }
 

  if (alarm_status == 1)
  {
    digitalWrite(r_led, HIGH);
    tone(buzzer, 500);
    delay(175);
    digitalWrite(r_led, LOW);
    delay(175);

    digitalWrite(r_led, HIGH);
    tone(buzzer, 700);
    delay(175);
    digitalWrite(r_led, LOW);
    delay(175);   

  }
  else {

    digitalWrite(r_led, LOW);
    noTone(buzzer);

  }
  


}
