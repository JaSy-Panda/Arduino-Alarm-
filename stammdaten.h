/*============================================================================================================
Descprition:  Stammdaten (Zimmer Guardian)
Author:       Jan Syed
Date:         26.02.2023
Version:      Alpha
============================================================================================================*/

int trigger = 13;
int echo = 12;

long dauer = 0;
long entfern = 0;

const int t1 = 250; //immer ms

const int buzzer = 10;

const int g_led = 8;
const int r_led = 9;

const int tast1 = 9;
const int tast2 = 10;

int status_tast1 = 0;
int status_tast2 = 0;

int alarm_status = 0;
int ein_status = 0;
