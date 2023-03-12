/*============================================================================================================
Descprition:  Zimmer Guardian
Author:       Jan Syed
Date:         05.03.23
Version:      V0.1.2
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


    
    //digitalWrite(g_led, LOW);
    //digitalWrite(r_led, LOW);
    //noTone(buzzer);

    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("A - Alarm Scharf");

    lcd.setCursor(0, 1);
    lcd.print("B - Change PW");

    Taste = Tastenfeld.getKey();

    if(Taste == 'A')
    {
      
      startupAlarm();
      alarm_ein = true;

    }
    else if(Taste == 'B' ){

      //passwordCheck();

      
      String firstPw = getNewPw();
      passwort = firstPw;
      

    } 
    /*else if (Taste == 'D')
    {
      
      lcdflashing();

      passwort = "4321";
      
    } */


  } 
  
  if (alarm_ein && !alarm_triggered)
  {
    Taste = Tastenfeld.getKey();

    int distanz =  getDistance();

    if(distanz < 30)
    {

     alarm_triggered = true;
     lcd.backlight();

    } 
    
    if(alarm_ein && alarm_triggered)
    {
      digitalWrite(r_led, HIGH);
      digitalWrite(g_led, LOW);
      tone(buzzer,800);

      passwordCheck();

      noTone(buzzer);
      digitalWrite(r_led, LOW);





      confirmBeep();


      alarm_triggered = false;
      alarm_ein = false;

    }

  }

}

//Start Countdown des Alarms

void startupAlarm(){

  lcd.clear();
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

  
  /*tone(buzzer, 300);

  delay(100);

  tone(buzzer, 800);
  digitalWrite(g_led, HIGH);

  delay(100);
    
  noTone(buzzer); */

  digitalWrite(g_led, HIGH);
  confirmBeep();
  lcd.noBacklight();

}

//

long getDistance(){

  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);

  entfernung = (dauer/2) * 0.03432;

  return entfernung;

}

//Funktion

String getNewPw(){

  String eingabe = "";
  int i = 0; // Variable für die Passwort Länge
  bool pwInput = true;

  lcdPwEing();



  while(pwInput){
    
    Taste = Tastenfeld.waitForKey();

    if(Taste == '#'){

      if( i > 3)
      {

       pwInput = false;
       return eingabe;

      }
      else{

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PW ist zu kurz");

        delay(1000);

        lcdPwEing();

        eingabe = "";
        i = 0;

      }

    }
    else if (Taste == '*')
    {
      eingabe = "";
      i = 0;

      lcdPwEing();

    }
    else if( Taste == 'A' || Taste == 'B' || Taste == 'C'|| Taste == 'D')
    {
      //Filter damit A, B, C und D nicht als Passwort eingegeben werden können
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

void passwordCheck(){

  String eingabe = "";
  int i = 0; // Variable für die Passwort Länge
  bool pwInput = true;

  lcdPwEing();

  while(pwInput){
    
    Taste = Tastenfeld.waitForKey();

    if(Taste == '#'){

      if(eingabe == passwort)
      {

       pwInput = false;

      }
      else{

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PW ist Falsch!");

        delay(1000);

        lcdPwEing();

        eingabe = "";
        i = 0;

      }

    }
    else if (Taste == '*')
    {
      eingabe = "";
      i = 0;

      lcdPwEing();

    }
    else if( Taste == 'A' || Taste == 'B' || Taste == 'C'|| Taste == 'D')
    {
      //Filter damit A, B, C und D nicht als Passwort eingegeben werden können
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

void confirmBeep(){

  tone(buzzer, 300);
  delay(100);
 
  tone(buzzer, 800);
  delay(100);

  noTone(buzzer);

}

void lcdPwEing(){

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PW Eingeben");

}

void lcdflashing(){

  int countdown = 2;

  while(countdown != 0)
    {
      
      delay(300);
      lcd.noBacklight();

      delay(300);
      lcd.backlight();

      countdown--;

      lcd.clear();

    } 

}


