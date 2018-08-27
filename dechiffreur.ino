void decodeur(String texte_recu){

int i = 0;
int place;

  while(texte_recu.indexOf("%/s%") >= 0){
    place = texte_recu.indexOf("%/s%");
    liste[i] = texte_recu.substring(0, place);
    texte_recu = texte_recu.substring(place+4);
    i++;
    
  }
  
}



