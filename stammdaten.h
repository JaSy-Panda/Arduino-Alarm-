/*============================================================================================================
Descprition:  Stammdaten Zimmer Guardian
Author:       Jan Syed
Date:         12.03.2023
Version:      V1.0.0
============================================================================================================*/

//Variabeln für das Programm

int maxPwLengh = 8; // Variabel für die Maximale Passwortlänge

const int t1 = 250; //Delay beim Wechseln auf *-Zeichen bei der PW eingaben

String passwort = "1234"; //standard PW

bool alarmOn = false; // Alarm Scharf status
bool alarmTriggered = false; // Alarm ausgelöst status

//Buzzer
const int buzzer = 11; // Pin 11 für den Buzzer

//LEDs
const int g_led = 9; //Pin 9 für die Grüne LED
const int r_led = 10; // Pin 10 für die Rote LED

//Ultraschallsensor
int trigger = 13; //Pin 13 to Trigger
int echo = 12; //Pin 12 to Echo

long dauer = 0; //Variabel für die Zeitmessung Out / In
long entfernung = 0; // Variabel für die gemessene Entfernung

long getDistance(); // Initailisierung Funktion für die Messung Distanz

//LCD Hardware
#include <Wire.h> //Libary für LCD
#include <LiquidCrystal_I2C.h> //LCD Libary
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD format

//Keypad Hardware
#include <Keypad.h> //Libary für Keypad
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




