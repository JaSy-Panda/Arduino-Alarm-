/*============================================================================================================
Descprition:  Zimmer Guardian
Author:       Jan Syed
Date:         13.03.2023
Version:      V1.0.0
============================================================================================================*/

#include "stammdaten.h" //Stammdaten der Hardware und Variabeln

void setup() {

  pinMode(buzzer, OUTPUT); //Initialisierung Buzzer als Output

  pinMode(g_led, OUTPUT); //Initialisierung Grüne LED als Output
  pinMode(r_led, OUTPUT); //Initialisierung Rote LED als Output

  //Initialisierung Ultraschallsensor
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  lcd.init(); //Initialisierung LCD

}

void loop() {

  //Abfrage Zustand der Analge

  if(!alarmOn && !alarmTriggered) // Wenn der Alarm NICHT Scharf ist und auch NICHT Ausgelöst dann:
  {

    lcd.backlight(); // LCD wird Eingeschaltet
    
    //Auswahl Menu-LCD
    lcd.setCursor(0, 0);
    lcd.print("A - Alarm Scharf");
    lcd.setCursor(0, 1);
    lcd.print("B - PW Aendern");

    Taste = Tastenfeld.getKey(); //User eingabe auf Keypad erkennen

    //Auswahl User
    if(Taste == 'A') //Wenn der User die Taste A drückt
    {
      
      startupAlarm(); //Alarm wird Scharf gesltellt
      alarmOn = true; //Alarm ist Scharf

    }
    else if(Taste == 'B' ) //Wenn der User B drückt
    {

      confirmBeep(); //BEEP für bestätigung das Menupunkt ausgewählt wurde

      //Text: Altes PW Eingeben
      lcdClearSetCurser();
      lcd.print("Altes PW ");
      lcd.setCursor(0, 1);
      lcd.print("Eingeben");
      delay(2000);

      passwordCheck(); //Altes Passwort wird überprüft
      confirmBeep(); //Bestätigungs BEEP

      bool pwCompare = true; //BOOL für den Status des PW-Vergleich

      while(pwCompare) //Führt den Code aus bis der Vergleich richtig ist
      {

        //Text: Neues PW Eingeben
        lcdClearSetCurser();
        lcd.print("Neues PW ");
        lcd.setCursor(0, 1);
        lcd.print("Eingeben");
        delay(2000);

        String firstPw = getNewPw(); //PW wird beim User abgefragt und im String gespeichert
        confirmBeep(); //Bestätigungs BEEP

        //Text: PW nochmals bestaetigen
        lcdClearSetCurser();
        lcd.print("PW nochmals");
        lcd.setCursor(0, 1);
        lcd.print("bestaetigen");
        delay(2000);

        String secondPw = getNewPw();//PW wird nochmals beim User abgefragt und im String gespeichert

        if(firstPw == secondPw) //Wenn die beiden PWs gleich sind
        {
          
          confirmBeep();//Bestätigungs BEEP

          //Text: PW wurde geandert
          lcdClearSetCurser();
          lcd.print("PW wurde");
          lcd.setCursor(0, 1);
          lcd.print("geaendert");
          delay(1000);
          gLedFlashing(); //Grüne LED blinkt als Bestätigung       

          pwCompare = false;//PW sind gleich. Der Vergleich ist abgeschlossen. BOOL wird dafür false

        }
        else // Wenn die PWs nicht übereinstimmen
        {

          //Text PW stimmen nicht ueberein
          lcdClearSetCurser();
          lcd.print("PW stimmen nicht");
          lcd.setCursor(0, 1);
          lcd.print("ueberein");

          //BEEP und Rote LED Blikent als Ablehnung
          tone(buzzer, 100);
          digitalWrite(r_led, HIGH);
          delay(200);
          noTone(buzzer);
          digitalWrite(r_led, LOW);
          delay(3000);

          //Text: Widerholen
          lcdClearSetCurser();
          lcd.print("Wiederholen");
          delay(3000);

          //BOOL wird nicht verändert die Abfrage wiederholt sich

        }

        passwort = firstPw;// Das temporäre PW wird als effektives PW abgespeichert

      }

    } 

  } 
  
  if (alarmOn && !alarmTriggered) //Wenn der Alarm Scharf ist aber NICHT Ausgelöst dann:
  {

    int distanz =  getDistance(); // Distaz wird gemessen und zwischen gespeichert

    if(distanz < 30) // Überprüfung ab die Distanz näher als 30CM ist
    {

     alarmTriggered = true;// Wenn Ja, wird der status auf Ausgelöst gesezt
     lcd.backlight(); //Licht des LCD wird wieder eingeschaltet

    } 
    
    if(alarmOn && alarmTriggered) //Wenn der Alarm ausgelöst hat
    {
      digitalWrite(r_led, HIGH); //Rote LED wird eingeschaltet
      digitalWrite(g_led, LOW); // Grüne LED wird ausgeschaltet
      tone(buzzer,800); //Buzzer wird eingeschaltet

      passwordCheck(); //Passwort muss richtig eingegeben werden

      //Wenn das PW richtig eingegeben wurde wird der Buzzer und die LED ausgeschaltet
      noTone(buzzer);
      digitalWrite(r_led, LOW);

      //Text: Alarm Deaktiviert
      lcdClearSetCurser();
      lcd.print("Alarm Deaktiviert");

      //Bestätigungs BEEP und Blinken Grüne LED
      confirmBeep();
      gLedFlashing();

      //Der Zustand der Anlage wird zurückgesetzt
      alarmTriggered = false;
      alarmOn = false;

    }

  }

}



void startupAlarm() //Funktion für das Scharfstellen
{

  lcd.clear(); //LCD Leer

  int countdown = 10;//Countdownlänge für den start. Dauer 10s


  while(countdown != 0) //Countdown funktion. Wird wiederholt solange der Countdwon nicht 0 ist.
    {
      //Text: Alarmscharf in: (Sekunde)
      lcd.setCursor(0, 0);
      lcd.print("Alarm Scharf in:");
      lcd.setCursor(0, 1);
      lcd.print(countdown);
      
      //LED und BEEP fürs Zählen
      digitalWrite(r_led, HIGH);
      tone(buzzer, 300);
      delay(250);
      digitalWrite(r_led, LOW);
      noTone(buzzer);
      delay(750);

      countdown--; //Countdown -1

      lcd.clear(); //Reset LCD

    }

    //Countdown ist Fertig

    digitalWrite(g_led, HIGH); //Grüne LED wird eingeschalter zum signaliesieren das die Anlage Scharf ist
    confirmBeep(); //Bestätigungs BEEP 
    lcd.noBacklight(); //LCD wird aus

}



long getDistance()//Funktion zum ermitteln der Distanz
{

  //Zeit des ECHO wird gemässen
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);

  //Entfernung berechnen
  entfernung = (dauer/2) * 0.03432;

  return entfernung; //Die Entfernung wird zurückgegeben

}



String getNewPw() //Funktion für die Abfrage des neuen PWs
{

  String eingabe = ""; //String in dem die Eingabe gespeichert wird
  int i = 0; // Zähler für die PW Länge
  bool pwInput = true; //BOOL für den Status der Eingabe

  lcdPwEing();//Text: PW Eingeben



  while(pwInput) //Solange die Eingabe nicht bestätigt wurde. Wird es wiederholt
  {
    
    Taste = Tastenfeld.waitForKey(); //Warten bis User eine Taste drückt. Scheibt den input in "Taste"

    if(Taste == '#') // Wenn die #-Taste gedrückt wird. Wird die Eingabe bestätigt
    {

      if( i > 3) //Überprüfung ob das Passwort min 4 Zeichen lang ist
      {

       pwInput = false; //Wenn Ja abfrage beenden
       return eingabe; //Gibt das neue PW zurück

      }
      else // Wenn das PW kürzer als 4 Zeiche ist
      {

        //Text: PW ist zu kurz
        lcdClearSetCurser();
        lcd.print("PW ist zu kurz");
        delay(1000);

        lcdPwEing(); //Text: PW Eingeben

        eingabe = ""; //die Eingabe wird zurückgesezt 
        i = 0; //Reset zähler

      }

    }
    else if (Taste == '*') //Wenn die *-Taste gedrückt wird. Wird die Eingabe Zurückgesezt
    {
      //Die *-Taste dient zum zurücksezten der Eingabe

      eingabe = ""; //die Eingabe wird zurückgesezt 
      i = 0; //Reset zähler


      lcdPwEing(); //Text: PW Eingeben

    }
    else if( Taste == 'A' || Taste == 'B' || Taste == 'C'|| Taste == 'D')//Filter damit A, B, C und D nicht als Passwort eingegeben werden können
    {
      //Filter damit A, B, C und D nicht als Passwort eingegeben werden können
    }
    else if( (Taste) && (i < maxPwLengh) )//Wenn eine Zahl-taste gedrückt wird und die Länge des PW nicht überschritten wurde
    {
      eingabe += Taste; //Speicher die Eingabe

      //Zeig kurz das Eingegebene Passwort auf dem LCD. Danach wird es mit * "versteckt"
      lcd.setCursor(i, 1);
      lcd.print(Taste);
      delay(t1);
      lcd.setCursor(i, 1);
      lcd.print("*");

      i++;//Zähler +1

    }

  }



}

void passwordCheck() // Überprüfung Passwort
{

  String eingabe = ""; //String in dem die Eingabe gespeichert wird
  int i = 0; // Zähler für die Anzahl eingaben
  bool pwInput = true; //BOOL für den Status der Eingabe

  lcdPwEing(); //Text: PW Eingebe

  while(pwInput) //Solange die Eingabe nicht bestätigt wurde. Wird es wiederholt
  {
    
    Taste = Tastenfeld.waitForKey(); //Warten bis User eine Taste drückt. Scheibt den input in "Taste"

    if(Taste == '#')// Wenn die #-Taste gedrückt wird. Wird die Eingabe bestätigt
    {

      if(eingabe == passwort) //Wenn die Eingabe und das definierte PW übereinstimmen
      {

       pwInput = false; //keine wiederholung mehr

      }
      else //Wenn die Eingabe und das definierte PW NICHT übereinstimmen
      {

        //Text: PW ist Falsch£
        lcdClearSetCurser();
        lcd.print("PW ist Falsch!");
        delay(1000);

        lcdPwEing(); //Text: PW Eingeben

        eingabe = ""; //die Eingabe wird zurückgesezt
        i = 0; //Reset zähler

      }

    }
    else if (Taste == '*') //Wenn die *-Taste gedrückt wird. Wird die Eingabe Zurückgesezt
    {
      eingabe = ""; //die Eingabe wird zurückgesezt
      i = 0; //Reset zähler

      lcdPwEing();

    }
    else if( Taste == 'A' || Taste == 'B' || Taste == 'C'|| Taste == 'D')
    {
      //Filter damit A, B, C und D nicht als Passwort eingegeben werden können
    }
    else if( (Taste) && (i < maxPwLengh) ) //Wenn eine Zahl-taste gedrückt wird und die Länge des PW nicht überschritten wurde
    {
      eingabe += Taste; //Speicher die Eingabe

      //Zeig kurz das Eingegebene Passwort auf dem LCD. Danach wird es mit * "versteckt"
      lcd.setCursor(i, 1);
      lcd.print(Taste);
      delay(t1);
      lcd.setCursor(i, 1);
      lcd.print("*");

      i++; //Zähler +1

    }

  }

}



void confirmBeep() //BEEP-Ton für Bestätigung
{

  //Duuu - Düü geräusch
  tone(buzzer, 300);
  delay(100);
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);

}

void lcdPwEing() // funktion für den LCD Text: PW Eingeben
{

  lcdClearSetCurser();
  lcd.print("PW Eingeben");

}

void gLedFlashing() //Funktion für das Blinken der Grünen LED
{

  int countdown = 4;

  while(countdown != 0)
    {
      
      delay(250);
      digitalWrite(g_led, HIGH);
      delay(250);
      digitalWrite(g_led, LOW);

      countdown--;

      lcd.clear();

    } 

}

//lcdClearSetCurser breinigt den LCD und setzt den Cursor wierder 0.0. Damit dieser code nicht immer wiederholft geschrieben werden muss
void lcdClearSetCurser(){
    
  lcd.clear();
  lcd.setCursor(0, 0);

}


