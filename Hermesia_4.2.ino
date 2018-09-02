//***********************************************************************************************//
//===========================Programme ISN affichage à commande wifi=============================//
//=====================================ISN Beaucamps Ligny=======================================//
//====================================Wemos D1 mini ESP8266======================================//
//==============================Adafruit 16x8 RBG Led Matrix Panel===============================//
//==========================WITTEVRONGEL Quentin  &&  FROMENTIN Nathan===========================//
//========================================Janvier 2018===========================================//
//***********************************************************************************************//



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>

#define Led_data_PIN   2.

File myFile;
RTC_DS1307 rtc;

//**********************************************DEFINITION DES VARIABLES***********************************************//

int red;
int green;
int blue;
int Width =      64;
int Height =     8;
int Brightness = 20;
int vitesse =    20;
int id = 1;
int id_arduino = 1;
int httpCode;
int flag = 0;
int place;
int place2;
int y;
int annee;
int mois;
int jour;
int heure;
int minutes;
int id_test = 0;
int lol;
int count = 0;

int disp = 0;
int pixelsInText;
const int chipSelect = D8;


String FLAG = "0";
String rouge;
String vert;
String bleue;
String afficher;
String buff;
String Weather;
String Temp;
String Years;
String Months;
String nbrMonths;
String nbrDays;
String Hours;
String Minutes;
String Adresse_site;
String dispText;
String dispColor;
String str;
String texte;
String ssid2;
String password2;
String liste[50];
String listeA[50];
String listeB[150];
String LIEN;
String buffers;
String texte_code;
String Version_SD = "3.8.2";
String LIEN_repo;
String texte_MAJ;
String Version;

char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
char NouveauLien [50];
const char* ssid;
const char* password;






Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);
int x = matrix.width();











void setup () {



  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(matrix.Color(80, 255, 0));



  Wire.begin();
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  //****************************************CARTE SD*******************************************//

  Serial.print("Initializing SD card...");

  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.print("SD Card Error !");
    matrix.show();
    while (1);
  }

  Serial.println("initialization done.");
  matrix.setTextColor(matrix.Color(0, 255, 0));


  //************************************LECTURE DU FICHIER*************************************//

  myFile = SD.open("Arduinfo.txt");
  if (myFile) {
    Serial.println("Arduinfo.txt");
    while (myFile.available()) {
      char ltr = myFile.read();
      texte += ltr;
    }

    myFile.close();

  } else {
    Serial.println("error opening Arduinfo.txt");
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.print("File Error !");
    matrix.show();
  }

  myFile = SD.open("UPDATE.txt");
  if (myFile) {
    while (myFile.available()) {
      char ltr = myFile.read();
      texte_MAJ += ltr;
    }
    myFile.close();

    //***************************************DECODAGE*******************************************//







    decodeur(texte, "%/s%");

    ssid2 = liste[0];
    password2 = liste[1];
    buff = liste[2];
    id_arduino = buff.toInt();
    String nbrdalles = liste[3];
    Width = nbrdalles.toInt() * 32;
    Serial.println(Width);
    String luminosite = liste[4];
    Brightness = luminosite.toInt();
    String speeds = liste[5];
    vitesse = speeds.toInt();
    vitesse = 255 - vitesse;
    Adresse_site = liste[6];

    ssid = ssid2.c_str();
    password = password2.c_str();

    decodeur(texte_MAJ, "%/s%");

    LIEN_repo = liste[0];
    Version_SD = liste[1];
    Serial.println(Version_SD);


    //**********************************PARAMETRAGE DU PANNEAU*********************************//

    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(Brightness);
    matrix.setTextColor(matrix.Color(80, 255, 0));

    matrix.setBrightness(Brightness);
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.print("V");
    matrix.print(Version_SD);
    matrix.show();
    delay(2000);
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.print("SD Card OK");
    matrix.show();
    delay(1000);


    //*************************************CONNEXION WIFI**************************************//

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      matrix.setBrightness(Brightness);
      delay(1000);
      Serial.print("Connecting..");
      Serial.println(ssid2 + password2);
      matrix.fillScreen(0);
      matrix.setCursor(0, 0);
      matrix.setTextColor(matrix.Color(255, 0, 0));
      matrix.print("connecting");
      matrix.show();
    }

    Serial.print("C'est bon");
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color(0, 255, 0));
    matrix.print("Connected !");
    matrix.show();
    delay(1000);
    MAJ();
  }

}

//****************************************FIN SETUP*****************************************//

void loop() {

  Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(Brightness);
  matrix.setTextColor(matrix.Color(80, 255, 0));

  //*********************************RECUPERATION DATE ET HEURE*******************************//

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    if (FLAG == "0") {

      LIEN = Adresse_site + "/Get/getYear.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Years = http.getString();
        Serial.println(Years);
        annee = Years.toInt();
        http.end();
        httpCode = 0;
      }



      LIEN = Adresse_site + "/Get/getMonth.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Months = http.getString();
        Serial.println(Months);
        http.end();
        httpCode = 0;
      }



      LIEN = Adresse_site + "/Get/getNbrMonth.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        nbrMonths = http.getString();
        Serial.println(nbrMonths);
        mois = nbrMonths.toInt();
        http.end();
        httpCode = 0;
      }



      LIEN = Adresse_site + "/Get/getNbrDay.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        nbrDays = http.getString();
        Serial.println(nbrDays);
        jour = nbrDays.toInt();
        http.end();
        httpCode = 0;
      }



      LIEN = Adresse_site + "/Get/getHour.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Hours = http.getString();
        Serial.println(Hours);
        heure = Hours.toInt();
        http.end();
        httpCode = 0;
      }



      LIEN = Adresse_site + "/Get/getMinute.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Minutes = http.getString();
        Serial.println(Minutes);
        minutes = Minutes.toInt();
        http.end();
        httpCode = 0;
      }

      rtc.adjust(DateTime(annee, mois, jour, heure, minutes, 0));
      FLAG = "1";
      Serial.println("FLAG ==" + FLAG);

    }



    DateTime now = rtc.now();



    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    //*******************************RECUPERATION METEO****************************//

    if (id == 1) {
      LIEN = Adresse_site + "/Get/getWeather.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Weather = http.getString();
        Serial.println(Weather);
        http.end();
      }
    }



    if (id == 1) {
      LIEN = Adresse_site + "/Get/getTemperature.php";
      http.begin(LIEN);
      httpCode = http.GET();

      if (httpCode > 0) {
        Temp = http.getString();
        Temp = Temp.toInt();
        Temp = String(Temp);
        Serial.println(Temp);
        http.end();
      }
    }

    //*******************************RECUPERATION TEXTE**********************************//

    LIEN = Adresse_site + "/Arduino/DataBaseArduino.php?id=" + id;
    http.begin(LIEN);
    httpCode = http.GET();
    Serial.print("httpCode = ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      str = http.getString();
      Serial.println(str);
      http.end();

      //***********************************DECODAGE TEXTE***********************************//

      place = str.indexOf("%/s%");

      if (place != -1) {
        decodeur(str, "%/s%");
        afficher = liste[0];
        rouge = liste[1];
        bleue = liste[2];
        vert = liste[3];
        String transition = liste[4];
        texte = liste[5];
        Serial.println(texte);

        //********************************VERIFICATION AFFICHEUR*****************************//

        for (int b = 0; b < 9; b++) {
          buff = texte[id_test];
          lol = buff.toInt();
          Serial.println(id_arduino);
          Serial.println(lol);

          if (lol == id_arduino) {
            Serial.println("id verifié");
            Serial.println(place);
            Serial.println(afficher);
            Serial.print("rouge = ");
            Serial.println(rouge);
            Serial.print("vert = ");
            Serial.println(vert);
            Serial.print("bleue = ");
            Serial.println(bleue);

            //************************REMPLACEMENT COMMANDES SPECIALES*************************//

            red = rouge.toInt();
            green = vert.toInt();
            blue = bleue.toInt();

            afficher.replace("036%/b%116%/b%101%/b%109%/b%112%/b%", codeur(Temp, "%/b%"));
            afficher.replace("036%/b%119%/b%101%/b%097%/b%116%/b%104%/b%101%/b%114%/b%", codeur(Weather, "%/b%"));

            String NBRJour = String(now.day(), DEC);
            String NBRMois = String(now.month(), DEC);
            String ANNEE = String(now.year(), DEC);

            String HEURES = String(now.hour(), DEC);

            String MINUTES = String(now.minute(), DEC);

            String SECONDES = String(now.second(), DEC);



            afficher.replace("036%/b%110%/b%098%/b%114%/b%106%/b%111%/b%117%/b%114%/b%", codeur(NBRJour, "%/b%"));
            afficher.replace("036%/b%106%/b%111%/b%117%/b%114%/b%", codeur(daysOfTheWeek[now.dayOfTheWeek()], "%/b%"));
            afficher.replace("036%/b%109%/b%111%/b%105%/b%115%/b%", codeur(Months, "%/b%"));
            afficher.replace("036%/b%110%/b%098%/b%114%/b%109%/b%111%/b%105%/b%115%/b%", codeur(NBRMois, "%/b%"));
            afficher.replace("036%/b%097%/b%110%/b%110%/b%101%/b%101%/b%", codeur(ANNEE, "%/b%"));
            afficher.replace("036%/b%104%/b%101%/b%117%/b%114%/b%101%/b%", codeur(HEURES, "%/b%"));
            afficher.replace("036%/b%109%/b%105%/b%110%/b%117%/b%116%/b%101%/b%", codeur(MINUTES, "%/b%"));

            //***********************************AFFICHAGE************************************//

            pixelsInText = (afficher.length() / 6) * 7;
            x = matrix.width();
            Serial.print("x = ");
            Serial.println(x);
            String abc = "ABC";
            char b = abc.charAt(2);
            int b_ascii_value = b;
            Serial.println(b_ascii_value);


            if (transition == "Droite") {

              matrix.setBrightness(Brightness);
              x = matrix.width();
              while (x > (matrix.width() - pixelsInText - 125)) {
                matrix.fillScreen(0);
                matrix.setCursor(--x, 0);
                matrix.setTextColor(matrix.Color(red, green, blue));
                buffers = afficher;
                while (buffers.indexOf("%/b%") >= 0) {

                  place = buffers.indexOf("%/b%");
                  int texte24 = buffers.substring(0, place).toInt();
                  matrix.write(texte24);
                  //Serial.write(texte24);
                  buffers = buffers.substring(place + 4);

                }
                //Serial.println("");
                matrix.show();
                delay(vitesse);
              }


            }



            if (transition == "Haut") {

              matrix.setBrightness(Brightness);
              y = -8;
              while (y < 1) {
                matrix.fillScreen(0);
                matrix.setCursor(0, y++);
                //Serial.println(y);
                matrix.setTextColor(matrix.Color(red, green, blue));
                buffers = afficher;
                while (buffers.indexOf("%/b%") >= 0) {

                  place = buffers.indexOf("%/b%");
                  int texte24 = buffers.substring(0, place).toInt();
                  matrix.write(texte24);
                  //Serial.write(texte24);
                  buffers = buffers.substring(place + 4);
                }
                // Serial.println("");
                matrix.show();
                delay(vitesse);
              }
              delay(1000);



              if (pixelsInText >= matrix.width()) {
                Serial.println("defilement haut");
                matrix.setBrightness(Brightness);
                x = 0;
                while (x > (matrix.width() - pixelsInText + 70)) {
                  matrix.fillScreen(0);
                  matrix.setCursor(--x, 0);
                  //Serial.println(x);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }
                delay(1000);
                matrix.setBrightness(Brightness);
                y = 0;
                while (y < 9) {
                  matrix.fillScreen(0);
                  matrix.setCursor(x, y++);
                  //Serial.println(y);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }

              } else {
                matrix.setBrightness(Brightness);
                y = 0;
                while (y < 9) {
                  matrix.fillScreen(0);
                  matrix.setCursor(0, y++);
                  //Serial.println(y);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }
              }
            }



            if (transition == "Bas") {

              matrix.setBrightness(Brightness);
              y = 8;
              while (y > -1) {
                matrix.fillScreen(0);
                matrix.setCursor(0, y--);
                matrix.setTextColor(matrix.Color(red, green, blue));
                buffers = afficher;
                while (buffers.indexOf("%/b%") >= 0) {

                  place = buffers.indexOf("%/b%");
                  int texte24 = buffers.substring(0, place).toInt();
                  matrix.write(texte24);
                  //Serial.write(texte24);
                  buffers = buffers.substring(place + 4);
                }
                //Serial.println("");
                matrix.show();
                delay(vitesse);
              }
              delay(1000);



              if (pixelsInText >= Width) {
                matrix.setBrightness(Brightness);
                x = 0;
                while (x > (matrix.width() - pixelsInText + 70)) {
                  matrix.fillScreen(0);
                  matrix.setCursor(--x, 0);
                  //Serial.println(x);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }
                delay(1000);
                matrix.setBrightness(Brightness);
                y = 0;
                while (y > -9) {
                  matrix.fillScreen(0);
                  matrix.setCursor(x, y--);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }

              } else {
                matrix.setBrightness(Brightness);
                y = 0;
                while (y > -9) {
                  matrix.fillScreen(0);
                  matrix.setCursor(0, y--);
                  matrix.setTextColor(matrix.Color(red, green, blue));
                  buffers = afficher;
                  while (buffers.indexOf("%/b%") >= 0) {

                    place = buffers.indexOf("%/b%");
                    int texte24 = buffers.substring(0, place).toInt();
                    matrix.write(texte24);
                    //Serial.write(texte24);
                    buffers = buffers.substring(place + 4);
                  }
                  //Serial.println("");
                  matrix.show();
                  delay(vitesse);
                }
              }
            }

            goto lolmdr;

          } else {
            Serial.println("id non verifié");
            id_test++;
          }

          Serial.println(texte[id_test]);
        }

lolmdr:

        id_test = 0;
        lol = 0;
        id++;

      } else {
        id = 1;
      }



    } else {
      id = 1;
      Serial.println("Aucune info trouvé");
      str = "";
      httpCode = 0;
      //ESP.restart();
      WiFi.disconnect();
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting...");
      }

      Serial.print("C'est bon");
      delay(1000);
    }

  } else {
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color(0, 255, 0));
    matrix.print("Deco :(");
    matrix.show();
  }























}

//**********************************SOUS PROGRAMME************************************//







String codeur(String Texte, String separateur) {
  int i = 0;
  String texte_code = "";
  String buff = Texte;
  while (i < buff.length()) {
    char b = buff.charAt(i);
    int b_ascii_value = b;
    Serial.println(b_ascii_value);
    texte_code = texte_code + b_ascii_value + separateur;
    i++;
  }
  return texte_code;
}



void MAJ() {

  HTTPClient http;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);
matrix.setBrightness(Brightness);

  String LIEN_MAJ = Adresse_site + "/Arduino/update/version.txt";
  http.begin(LIEN_MAJ);
  httpCode = http.GET();
  Serial.print("httpCode = ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    Version = http.getString();
    Serial.println(Version);
    http.end();
  }
  decodeur(Version, "%/s%");
  Version = liste[1];
  LIEN_repo = Adresse_site + "/Arduino/update/";

  Serial.println(Version);
  Serial.println(LIEN_repo);


  if (Version != Version_SD) {

    String lien_update = LIEN_repo + Version + ".bin";


    SD.remove("UPDATE.txt");
    File fichier = SD.open("UPDATE.txt", FILE_WRITE);
    fichier.print(LIEN_repo + "%/s%" + Version + "%/s%");
    fichier.close();


    if (WiFi.status() == WL_CONNECTED) {

      matrix.fillScreen(0);
      matrix.setTextColor(matrix.Color(0, 0, 255));
      matrix.setCursor(0,0);
      matrix.print("Updating...");
      matrix.show();
      t_httpUpdate_return ret = ESPhttpUpdate.update(lien_update);
      //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

      switch (ret) {
        case HTTP_UPDATE_FAILED:

          break;

        case HTTP_UPDATE_NO_UPDATES:

          break;

        case HTTP_UPDATE_OK:

          break;
      }
    }


  } else {


    matrix.fillScreen(0);
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.print("no update");
    matrix.show();

  }

}


void decodeur(String texte_recu, String separator) {



  int i = 0;
  int place;
  i = 0;
  while (texte_recu.indexOf(separator) >= 0) {
    place = texte_recu.indexOf(separator);
    liste[i] = texte_recu.substring(0, place);
    texte_recu = texte_recu.substring(place + separator.length());
    i++;
  }
}















