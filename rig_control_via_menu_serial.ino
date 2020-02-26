/*
  Auteur: data_twiiix
  from: Bruxelles, Belgium
  BTC / ETH address: 115K3ZpjPrtkw1fyQSEM2dNzUsgrPiD7Ns / 0x6D7Adc94EEd5645467E1087B5b3CBe4F9f56E4d6
*/

// include the library code:
#include <Wire.h> // consulter la doc de Wire pour les E/S du module DS1307 en fonction de votre carte arduino
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A0 //Sonde DS18B20
#define start 2         // pin qui controle l'optocoupleur
#define relais 3        //pin du relais de sécurité qui permet de coupé l'alim. du Rig
#define runCheck A1     //pin brancher sur la pin 3.3v du TMP Header de ma carte mère.
char rxChar = 0;        // var pour les entrées series.
unsigned int sinceH;    //  """""""""""" contennir l'heure du dernier démarage.
unsigned int sinceM;
unsigned int sinceS;


//----------------------------------------------
// configuration a editer par l"utilisateur
// déclaration de l'horaire de démarage
const int hStart = 23; // Heurre de démarage du rig
const int mStart = 0;  // Minute (sans "0" devant le chiffre) de démarage du rig
const int sStart = 0;  // Seconde de démarage du rig

// déclaration de l'horaire d'arrêt
const int hStop = 7; // Heurre d'arrêt du rig
const int mStop = 0; // Minute (sans "0" devant le chiffre) de démarage du rig
const int sStop = 0; // Seconde d'arrêt du rig
//fin edition utilisateur
//----------------------------------------------

int isWorking = false;
int timerWorking = true;
int isHigh = true;

//ajouter  RTClib
RTC_DS1307 rtc;
//ajout de oneWire
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void logTemp(void)
{
  if (isWorking)
  {
    if (millis() % (3600 * 10) == 0)
    {
      sensors.requestTemperatures();
      float tempValue = sensors.getTempCByIndex(0);
      Serial.print("Température: ");
      Serial.print("");
      Serial.print(tempValue);
      Serial.print(" C°");
      Serial.println("");
    }
  }
}

void isRunning()
{
  int runCheckValue = analogRead(runCheck);
  if (runCheckValue > 15 && !isWorking)
  {
    isWorking = true;
  }
  if (runCheckValue < 14 && isWorking)
  {
    isWorking = false;
  }
}

void timer()
{
  DateTime now = rtc.now();
  if (!isWorking)
  {
    if (now.hour() == hStart && now.minute() == mStart && now.second() == sStart)
    {
      Serial.println("");
      Serial.print("Démarré automatiquement à:");
      Serial.print(" ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println("");
      digitalWrite(start, HIGH);
      delay(150);
      digitalWrite(start, LOW);
      delay(1000);
      isWorking = !isWorking;
      since();
    }
  }
  if (isWorking)
  {
    if (now.hour() == hStop && now.minute() == mStop && now.second() == sStop)
    {
      Serial.println("");
      Serial.print("Arrêté automatiquement à:");
      Serial.print(" ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
      digitalWrite(start, HIGH);
      delay(150);
      digitalWrite(start, LOW);
      delay(1000);
      isWorking = !isWorking;
    }
  }
}

int since()
{
  DateTime now = rtc.now();
  sinceH = now.hour();
  sinceM = now.minute();
  sinceS = now.second();
}

float readTemp()
{
  sensors.requestTemperatures();
  float tempValue = sensors.getTempCByIndex(0);
  return tempValue;
}

void menu(void)
{
  Serial.println("");
  Serial.println("^^^ Liste des commandes: ^^^");
  Serial.println("? -> Affice le Menu.");
  Serial.println("r -> Affiche le résumé des options du Rig.");
  Serial.println("d -> Démarre le Rig.");
  Serial.println("s -> Arrête le Rig.");
  Serial.println("a -> active/désactive le timer.");
}

void resumer(void)
{
  DateTime now = rtc.now();
  Serial.println(" ");
  Serial.println("Résumé de l'activité minière:");
  Serial.println("*****************************");
  Serial.print("Heure courante:");
  Serial.print(" ");
  if (now.hour() < 10)
  {
    Serial.print("0");
    Serial.print(now.hour(), DEC);
  }
  else
  {
    Serial.print(now.hour(), DEC);
  }
  Serial.print(':');
  if (now.minute() < 10)
  {
    Serial.print("0");
    Serial.print(now.minute(), DEC);
  }
  else
  {
    Serial.print(now.minute(), DEC);
  }
  Serial.print(':');
  if (now.second() < 10)
  {
    Serial.print("0");
    Serial.print(now.second(), DEC);
  }
  else
  {
    Serial.print(now.second(), DEC);
  }
  Serial.println();
  Serial.print("Heure de démarrage:");
  if (hStart < 10)
  {
    Serial.print("0");
    Serial.print(hStart);
  }
  else
  {
    Serial.print(hStart);
  }
  Serial.print(':');
  if (mStart < 10)
  {
    Serial.print("0");
    Serial.print(mStart);
  }
  else
  {
    Serial.print(mStart);
  }
  Serial.print(':');
  if (sStart < 10)
  {
    Serial.print("0");
    Serial.print(sStart);
  }
  else
  {
    Serial.print(sStart);
  }
  Serial.println();
  Serial.print("Heure d'arrêt:");
  if (hStop < 10)
  {
    Serial.print("0");
    Serial.print(hStop);
  }
  else
  {
    Serial.print(hStop);
  }
  Serial.print(':');
  if (mStop < 10)
  {
    Serial.print("0");
    Serial.print(mStop);
  }
  else
  {
    Serial.print(mStop);
  }
  Serial.print(':');
  if (sStop < 10)
  {
    Serial.print("0");
    Serial.print(sStop);
  }
  else
  {
    Serial.print(sStop);
  }
  Serial.println();
  Serial.print("Date et heure du dernier démarage:");
  Serial.print(" ");
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.year(), DEC);
  Serial.print(" - ");
  if (sinceH < 10)
  {
    Serial.print("0");
    Serial.print(sinceH);
    Serial.print(":");
  }
  else
  {
    Serial.print(sinceH);
    Serial.print(":");
  }
  if (sinceM < 10)
  {
    Serial.print("0");
    Serial.print(sinceM);
    Serial.print(":");
  }
  else
  {
    Serial.print(sinceM);
    Serial.print(":");
  }
  if (sinceS < 10)
  {
    Serial.print("0");
    Serial.print(sinceS);
  }
  else
  {
    Serial.print(sinceS);
  }
  Serial.println("");
  Serial.print("Le Rig tourne:");
  if (isWorking)
  {
    Serial.print(" ");
    Serial.print("Oui!");
    Serial.println("");
  }
  else
  {
    Serial.print(" ");

    Serial.print("Non!");
    Serial.println("");
  }

  Serial.print("Etat du Timer:");
  if (timerWorking)
  {

    Serial.print(" ");

    Serial.print("Activé!");
  }
  else
  {
    Serial.print(" ");
    Serial.print("Désctivé :(( - entre 'd' pour faire courir ton rig.");
  }
  Serial.println();
  Serial.print("Température au seins de la plate-forme:");
  Serial.print(" ");
  Serial.print(readTemp());
  Serial.print("°");
  Serial.print(" Celsius.");
  Serial.println();
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  rtc.begin();
  sensors.begin();
  //rtc.adjust(DateTime(__DATE__,__TIME__));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  pinMode(start, OUTPUT);
  pinMode(A1, INPUT);
  Serial.flush(); // effacage du buffer memoire de réception
  menu();
}

void loop()
{
  logTemp();
  if (timerWorking)
  {
    timer();
  }
  //    tempCheck()
  isRunning();
  DateTime now = rtc.now();
  timer();
  if (Serial.available() > 0)
  {                         //  verifie si le port serie est près a recevoir
    rxChar = Serial.read(); //  recupère les caractères passée via le voie serier dans une var.
    Serial.flush();         //  vide la mem. tempon d réception.
    switch (rxChar)
    {
    case 'a':
    case 'A':
      if (timerWorking)
      {
        Serial.println(" ");
        Serial.println("Désactivation du timer");
        Serial.println(" ");
        timerWorking = !timerWorking;
      }
      else
      {
        Serial.println(" ");
        Serial.println("Réactivation du timer");
        Serial.println(" ");
        timerWorking = !timerWorking;
      }
      break;
    case 'r':
    case 'R':
      resumer();
      break;
    case 'd':
    case 'D':
      if (!isWorking)
      {
        digitalWrite(start, HIGH);
        delay(150);
        digitalWrite(start, LOW);
        isWorking = !isWorking;
        since();
        delay(1000);
        Serial.println("");
        Serial.print("Démarré à:");
        Serial.print(" ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println(" ");
      }
      else
      {
        Serial.println("");
        Serial.print("Le rig cours déjà?! ");
        Serial.println("");
      }
      break;
    case 's':
    case 'S':
      if (isWorking)
      {
        digitalWrite(start, HIGH);
        delay(150);
        digitalWrite(start, LOW);
        isWorking = !isWorking;
        delay(1000);
        Serial.println("");
        Serial.print("Arrêté à:");
        Serial.print(" ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
      }
      else
      {
        Serial.println("");
        Serial.print("Le rig est déjà éteint?! non? - tape 'z' si il tourne");
        Serial.println("");
      }
      break;
    case '?':
      menu(); //appel du menu par ça fonction
      break;
    }
  }
}
