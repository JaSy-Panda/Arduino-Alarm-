/*============================================================================================================
Descprition:  Aufgabe 5
Author:       Jan Syed
Date:         01.03.2023
Version:      V0.1.1
============================================================================================================*/

#include "stammdaten.h"

#include <Keypad.h>
const byte COLS = 4; //4 Spalten
const byte ROWS = 4; //4 Zeilen
//Die Ziffern und Zeichen des Keypads werden eingegeben:
char hexaKeys[ROWS][COLS]={
{'D','#','0','*'},
{'C','9','8','7'},
{'B','6','5','4'},
{'A','3','2','1'}
};

byte colPins[COLS] = {31,33,35,37}; //Definition der Pins für die 4 Spalten
byte rowPins[ROWS] = {39,41,43,45};//Definition der Pins für die 4 Zeilen
char Taste; //Taste ist die Variable für die jeweils gedrückte Taste.
Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Das Keypad kann absofort mit "Tastenfeld" angesprochen werden

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

long getDistance();

void setup() {
  Serial.begin (9600);
  pinMode(buzzer, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(g_led, OUTPUT);
  pinMode(r_led, OUTPUT);

  lcd.init();

}

void loop() {

  if(!alarm_ein && !alarm_triggered){


    lcd.backlight();
    digitalWrite(g_led, LOW);
    digitalWrite(r_led, LOW);
    noTone(buzzer);

    lcd.setCursor(0, 0);
    lcd.print("A - Alarm Scharf");

    lcd.setCursor(0, 1);
    lcd.print("B - Change PW");

    Taste = Tastenfeld.getKey();

    if(Taste == 'A')
    {
      lcd.clear();
      startupAlarm();

      alarm_ein = true;


    }
    else if(Taste == 'B' ){
      
      digitalWrite(g_led, HIGH);
      delay(250);
      digitalWrite(g_led, LOW);
      Serial.println("okey");

    } 



  } 
  
  if (alarm_ein && !alarm_triggered)
  {
    Taste = Tastenfeld.getKey();

    int distanz =  getDistance();

    if(distanz < 30)
    {

     alarm_triggered = true;

    } 
    else if(Taste == 'B')
    {

      alarm_ein = false;

    } 
    
    if(alarm_ein && alarm_triggered)
    {
      digitalWrite(r_led, HIGH);
      digitalWrite(g_led, LOW);
      tone(buzzer,800);

      passwortEingabe();

      alarm_triggered = false;
      alarm_ein = false;

    }

  }

}

//Start Countdown des Alarms

void startupAlarm(){


  int countdown = 3;

  /*while(countdown != 0)
    {
      lcd.setCursor(0, 0);
      lcd.print("Alarm Scharf in:");

      lcd.setCursor(0, 1);
      lcd.print(countdown);
      
      digitalWrite(r_led, HIGH);
      tone(buzzer, 300);

      delay(250);

      digitalWrite(r_led, LOW);
      noTone(buzzer);

      delay(750);

      countdown--;

      lcd.clear();



    } */

  tone(buzzer, 300);

  delay(100);

  tone(buzzer, 800);
  digitalWrite(g_led, HIGH);

  delay(100);
    
  noTone(buzzer);


}



long getDistance(){

  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);

  entfernung = (dauer/2) * 0.03432;

  return entfernung;

}

void passwortEingabe(){

  String eingabe = "";
  int i = 0; // Variable für die Passwort Länge
  bool pw_abfrage = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PW Eingeben");

  while(pw_abfrage){
    
    Taste = Tastenfeld.waitForKey();

    if(Taste == '#'){

      if(eingabe == passwort)
      {
       pw_abfrage = false;

      }
      else{

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Falsches Kennwort");

        delay(300);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PW Eingeben");

        eingabe = "";
        i = 0;

      }


    }
    else if (Taste == '*')
    {
      eingabe = "";
      i = 0;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PW Eingeben");

    }
    else if( Taste == 'A' || Taste == 'B' || Taste == 'C'|| Taste == 'D')
    {
      Serial.println("A, B, C oder D ist keine Zahl");
    }
    else if( (Taste) && (i < 8) )
    {
      eingabe += Taste;

      Serial.println(eingabe);

      lcd.setCursor(i, 1);
      lcd.print(Taste);

      delay(200);
      lcd.setCursor(i, 1);
      lcd.print("*");

      i++;

    }

  }

}



