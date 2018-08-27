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

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>



/**************************************************WIFI********************************************/


/**************************************************LED*********************************************/

#define Led_data_PIN   2//arduino pin for WS2812 data

File myFile;
String texte;
String ssid2;
String password2;
const char* ssid;
const char* password;

int red;
int green;
int blue;
int Width =      64;
int Height =     8;
int Brightness = 50;
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
int id_test = 0;
int lol;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(Width, Height, Led_data_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,  NEO_GRB + NEO_KHZ800);


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
  while(x > (matrix.width() - pixelsInText)){
        matrix.fillScreen(0);
        matrix.setCursor(--x-64, 0);
        matrix.setTextColor(matrix.Color(red,green,blue));
        matrix.print(afficher);
        matrix.show();
        delay(vitesse);

      }
      //flag = 0;
  //  }
}

void defilement_vertical1(){
  //if (flag = 0){
  y = 8;
  while(y > -1){
    matrix.fillScreen(0);
    matrix.setCursor(0, y--);
    matrix.setTextColor(matrix.Color(red,green,blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

    }
  //  flag = 1;
  //}

}

void defilement_vertical2(){
  //if (flag = 0){
  y = -8;
  while(y < 1){
    matrix.fillScreen(0);
    matrix.setCursor(0, y++);
    matrix.setTextColor(matrix.Color(red,green,blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

    }
  //  flag = 1;
  //}

}

void defilement_vertical3(){
  //if (flag = 0){
  y = 0;
  while(y < 9){
    matrix.fillScreen(0);
    matrix.setCursor(0, y++);
    matrix.setTextColor(matrix.Color(red,green,blue));
    matrix.print(afficher);
    matrix.show();
    delay(50);

    }
  //  flag = 1;
  //}

}


//Fin Led

void setup () {





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

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.


  // re-open the file for reading:
  myFile = SD.open("Arduinfo.txt");
  if (myFile) {
    Serial.println("Arduinfo.txt");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {

      char ltr = myFile.read();
      texte += ltr;


    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Arduinfo.txt");
  }
  place = texte.indexOf("%/s%");
  ssid2 = texte.substring(0,place);
  password2 = texte.substring(place+4);


  ssid = ssid2.c_str();
  password = password2.c_str();

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(Brightness);
  matrix.setTextColor(matrix.Color(80,255,0));




  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color(255,0,0));
    matrix.print("connecting");
    matrix.show();


  }
  Serial.print("C'est bon");
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.setTextColor(matrix.Color(0,255,0));
  matrix.print("connected !");
  matrix.show();
  delay(1000);
}

void loop() {



  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status


    HTTPClient http;  //Declare an object of class HTTPClient



//text%/s%255000255%/s%125

/************************************************TEXTE**********************************************/



    sprintf(NouveauLien, "http://109.216.58.40:25565/DataBaseArduino2.php?id=%i", id);
    http.begin(NouveauLien);  //Specify request destination
    httpCode = http.GET();                                                                  //Send the request


    if (httpCode > 0) { //Check the returning code

      str = http.getString();   //Get the request response payload
      Serial.println(str);

      http.end();   //Close connection

      place = str.indexOf("%/s%");

      if(place != -1) {
        rouge = str.substring(place + 4, place + 7);
        bleue = str.substring(place+ 7, place + 10);
        vert = str.substring(place + 10, place + 13);
        afficher = str.substring(0, place);
        texte = str.substring(place+13);

        for(int b=0; b<9; b++){
        buff = texte[id_test];
        lol = buff.toInt();
        Serial.println(id_arduino);
        Serial.println(lol);
        if (lol == id_arduino){
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

        pixelsInText = (afficher.length() * 7)+8;
        x = matrix.width();


        if(pixelsInText -15 > Width){
        defilement_vertical2();
        delay(1000);
        defilement();
        }else{
          defilement_vertical2();
          delay(1000);
          defilement_vertical3();
          id_test = 0;
          lol = 0;
        }
        delay(1000);
        goto lolmdr;
        
        
        }else{
        Serial.println("id non verifié");
        id_test++;
        
        
        
        }
        Serial.println(texte[id_test]);
      
      
        
      
    
      }
      lolmdr:
      id_test = 0;
      id++;





        

        

      }else {
          id = 1;
      }


    }else {
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

  }else{

    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color(0,255,0));
    matrix.print("Deco :(");
    matrix.show();

  }
}