/*============================================================================================================
Descprition:  Zimmer Guardian
Author:       Jan Syed
Date:         19.12.2022
Version:      V0.1.2
============================================================================================================*/

int trigger = 13;
int echo = 12;

long dauer = 0;
long entfernung = 0;

long distanz;

const int t1 = 250; //immer ms

const int buzzer = 11;

const int g_led = 9;
const int r_led = 10;

bool alarm_ein = false;
bool alarm_triggered = false;

String passwort = "1234";




