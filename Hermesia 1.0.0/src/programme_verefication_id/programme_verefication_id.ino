String texte;
String buff;
int id = 2;
int lol;
int id_test = 0;
String recu = "text%/s%255000255125";
int place;
int red;
int green;
int blue;
String rouge;
String vert;
String bleue;
String afficher;



void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    place = recu.indexOf("%/s%");
    rouge = recu.substring(place+4, place+7);
    vert = recu.substring(place+7, place+10);
    bleue = recu.substring(place+10, place+13);
    afficher = recu.substring(0, place);
    texte = recu.substring(place+13);
    
    Serial.println("rouge = " + rouge);
    Serial.println("vert = " + vert);
    Serial.println("bleue = " + bleue);
    Serial.println("texte = " + afficher);
    Serial.println("id = " + texte);
    

  
    while (texte[id_test] > 0 ){
      buff = texte[id_test];
      lol = buff.toInt();
      if (lol == id){
        Serial.println("id verifié");
      }else{
        Serial.println("id non verifié");
      }
      Serial.println(texte[id_test]);
      
      
      id_test++;
      
    
    }
    
}


void loop() {
    // put your main code here, to run repeatedly:
}

