void decodeur(String Texte_recu) {


int i = 0;
while(Texte_recu.indexOf("%/s%")>=0){
 int place = Texte_recu.indexOf("%/s%");
 Serial.println(place);
 liste[i] = Texte_recu.substring(0,place);
 Texte_recu = Texte_recu.substring(place+4);
 Serial.println(liste[i]);
 i++;
}


 
}