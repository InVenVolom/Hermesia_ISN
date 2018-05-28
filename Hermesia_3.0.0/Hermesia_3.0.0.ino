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

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>


#define Led_data_PIN   2//arduino pin for WS2812 data

File myFile;
RTC_DS1307 rtc;




String texte;
String ssid2;
String password2;
String liste[50];
String listeB[150];
char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
const char* ssid;
const char* password;

int red;
int green;
int blue;
int Width =      64;
int Height =     8;
int Brightness = 20;
int vitesse =    20;
char NouveauLien [50];
int id = 1;
int id_arduino = 1;
int httpCode;
int flag = 0;
int place;
int place2;
int y;
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
int annee;
int mois;
int jour;
int heure;
int minutes;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);


int id_test = 0;
int lol;
int count = 0;
int FLAG = 0;




// Set Defaults
String dispText;
String dispColor;
int disp = 0;
int pixelsInText;
int x = matrix.width();

/**********************************************FIN LED*********************************************/


String str;






void defilement() {
  //if (flag = 1){
  while (x > (matrix.width() - pixelsInText - 50)) {
    matrix.fillScreen(0);
    matrix.setCursor(--x, 0);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(vitesse);

  }

}


void defilement_2() {
  //if (flag = 1){
  x = 0;
  while (x > (matrix.width() - pixelsInText + 25)) {
    matrix.fillScreen(0);
    matrix.setCursor(--x, 0);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(vitesse);


  }
  int x_2 = x;
  //flag = 0;
  //  }
}


void defilement_bas_1() {
  //if (flag = 0){
  y = 8;
  while (y > -1) {
    matrix.fillScreen(0);
    matrix.setCursor(0, y--);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

}

void defilement_bas_2() {
  //if (flag = 0){
  y = 0;
  while (y > -9) {
    matrix.fillScreen(0);
    matrix.setCursor(0, y--);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

}
void defilement_bas_3() {
  //if (flag = 0){
  y = 0;
  while (y > -9) {
    matrix.fillScreen(0);
    matrix.setCursor(x, y--);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

}

void defilement_haut_1() {

  y = -8;
  while (y < 1) {
    matrix.fillScreen(0);
    matrix.setCursor(0, y++);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

}

void defilement_haut_2() {
  //if (flag = 0){
  y = 0;
  while (y < 9) {
    matrix.fillScreen(0);
    matrix.setCursor(0, y++);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

}

void defilement_haut_3() {
  //if (flag = 0){
  y = 0;
  while (y < 9) {
    matrix.fillScreen(0);
    matrix.setCursor(x, y++);
    matrix.setTextColor(matrix.Color(red, green, blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

  }
  //  flag = 1;
  //}

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

const int chipSelect = D8;


//Fin Led

void setup () {

  // Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);

  Wire.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");




  // ouvre le fichier
  myFile = SD.open("Arduinfo.txt");
  if (myFile) {
    Serial.println("Arduinfo.txt");

    // Lit le fichier caracteres par caracteres
    while (myFile.available()) {

      char ltr = myFile.read();
      texte += ltr;


    }
    // ferme le fichier
    myFile.close();
  } else {
    // si le fichier n'est pas present
    Serial.println("error opening Arduinfo.txt");
  }
  decodeur(texte, "%/s%");
  ssid2 = liste[0];
  password2 = liste[1];
  buff = liste[2];
  id_arduino = buff.toInt();
  String nbrdalles = liste[3];
  Width = nbrdalles.toInt() * 32;
  String luminosite = liste[4];
  Brightness = luminosite.toInt();
  String speeds = liste[5];
  vitesse = speeds.toInt();
  vitesse = 255 - vitesse;



  ssid = ssid2.c_str();
  password = password2.c_str();



  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(Brightness);
  matrix.setTextColor(matrix.Color(80, 255, 0));




  WiFi.begin(ssid, password);
  Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);

  while (WiFi.status() != WL_CONNECTED) {

    //Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);

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
}

void loop() {

  Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(Brightness);
  matrix.setTextColor(matrix.Color(80, 255, 0));

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status


    HTTPClient http;  //Declare an object of class HTTPClient

    if (FLAG == 0) {

      http.begin("http://hermesia.ddns.net/Get/getYear.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Years = http.getString();   //Get the request response payload
        Serial.println(Years);
        annee = Years.toInt();

        http.end();   //Close connection
        httpCode = 0;
      }

      http.begin("http://hermesia.ddns.net/Get/getMonth.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Months = http.getString();   //Get the request response payload
        Serial.println(Months);

        http.end();   //Close connection
        httpCode = 0;
      }
      http.begin("http://hermesia.ddns.net/Get/getNbrMonth.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        nbrMonths = http.getString();   //Get the request response payload
        Serial.println(nbrMonths);
        mois = nbrMonths.toInt();

        http.end();   //Close connection
        httpCode = 0;
      }

      http.begin("http://hermesia.ddns.net/Get/getNbrDay.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        nbrDays = http.getString();   //Get the request response payload
        Serial.println(nbrDays);
        jour = nbrDays.toInt();

        http.end();   //Close connection
        httpCode = 0;
      }

      http.begin("http://hermesia.ddns.net/Get/getHour.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Hours = http.getString();   //Get the request response payload
        Serial.println(Hours);
        heure = Hours.toInt();

        http.end();   //Close connection
        httpCode = 0;
      }

      http.begin("http://hermesia.ddns.net/Get/getMinute.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Minutes = http.getString();   //Get the request response payload
        Serial.println(Minutes);
        minutes = Minutes.toInt();

        http.end();   //Close connection
        httpCode = 0;
      }






      rtc.adjust(DateTime(annee, mois, jour, heure, minutes, 0));
      FLAG = 1;
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

    if (id == 1) {
      http.begin("http://hermesia.ddns.net/Get/getWeather.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Weather = http.getString();   //Get the request response payload
        Serial.println(Weather);

        http.end();   //Close connection

      }
    }

    if (id == 1) {
      http.begin("http://hermesia.ddns.net/Get/getTemperature.php");  //Specify request destination
      httpCode = http.GET();                                                                  //Send the request

      if (httpCode > 0) { //Check the returning code

        Temp = http.getString();   //Get the request response payload
        Serial.println(Weather);

        http.end();   //Close connection

      }
    }

    sprintf(NouveauLien, "http://hermesia.ddns.net/Arduino/DataBaseArduino.php?id=%i", id);
    http.begin(NouveauLien);  //Specify request destination
    httpCode = http.GET();                                                                  //Send the request


    if (httpCode > 0) { //Check the returning code

      str = http.getString();   //Get the request response payload
      Serial.println(str);

      http.end();   //Close connection

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

            red = rouge.toInt();
            green = vert.toInt();
            blue = bleue.toInt();

            afficher.replace("$temp", Temp);
            afficher.replace("$weather", Weather);

            String NBRJour = String(now.day(), DEC);
            String NBRMois = String(now.month(), DEC);
            String ANNEE = String(now.year(), DEC);
            String HEURES = String(now.hour(), DEC);
            
            String MINUTES = String(now.minute(), DEC);
            String SECONDES = String(now.second(), DEC);

            afficher.replace("ç", "c");
            afficher.replace("°", "^");
            afficher.replace("é", "e");
            afficher.replace("è", "e");
            afficher.replace("ê", "e");
            afficher.replace("ë", "e");
            afficher.replace("à", "a");
            afficher.replace("ä", "a");
            afficher.replace("â", "a");
            afficher.replace("ù", "u");
            afficher.replace("û", "u");
            afficher.replace("ü", "i");

            afficher.replace("$nbrjour", NBRJour);
            afficher.replace("$jour", daysOfTheWeek[now.dayOfTheWeek()]);
            afficher.replace("$mois", Months);
            afficher.replace("$annee", ANNEE);
            afficher.replace("$heure", HEURES);
            afficher.replace("$minute", MINUTES);


            pixelsInText = (afficher.length() * 7) + 8;
            x = matrix.width();

            if (transition == "Droite") {
              defilement();
            }

            if (transition == "Haut") {

              defilement_haut_1();
              delay(1000);
              if (pixelsInText >= Width) {
                defilement_2();
                delay(1000);
                defilement_haut_3();
              } else {

                defilement_haut_2();
              }
            }

            if (transition == "Bas") {
              defilement_bas_1();
              delay(1000);
              if (pixelsInText >= Width) {
                defilement_2();
                delay(1000);
                defilement_bas_3();
              } else {

                defilement_bas_2();
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
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {

        delay(1000);
        Serial.print("Connecting..");

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
