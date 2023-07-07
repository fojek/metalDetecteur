
// Constantes
const int NBMESURES = 100;
const int NBSIG = 3;
const int GAIN = 200; 
const int ECARTTYPE = 3;

const int MINFREQ = 200;
const int MIDFREQ = 5000;
const int MAXFREQ = 8800;

const int PINLEDR = 5;
const int PINLEDG = 6;
const int PINLEDB = 6;
const int PINSPEAKER = 10;
const int PINMESURE = A0;

// Classe Mesure
class Mesures {
protected:
  int mesures[NBMESURES];
  int index = 0;
  float moyenne = 0;
  float ecartType = 0;
  bool estPret = false;

  int toneAct = 0;

  int ledR = 0;
  int ledG = 0;
  int ledB = 0;
  
public:
  Mesures() {};

  void init() {
    for(int i=0; i<NBMESURES; ++i){
       ajoutMesure(analogRead(PINMESURE));
    }

    calcMoyenne();
    calcEcartType();
  };
  
  void ajoutMesure(int mes) {
    int val = abs(mes - moyenne);
    
    Serial.println(toString());
    
    if(val > 3*ecartType) {
      mesures[index] = moyenne;
      calcSorties(val);
      return;
    } else if (val > 2*ecartType) {
      mesures[index] = moyenne;
      calcSorties(val);
      return;
    } else if (val > 1.5*ecartType) {
      mesures[index] = mes;
      calcSorties(val);
      return;
    }
    
    mesures[index] = mes;

    calcSorties(val);

    if(++index == NBMESURES) {
      index = 0;
    }

    calcMoyenne();
    calcEcartType();
  };

  void calcMoyenne() {
    long total = 0;
    for(int i=0; i<NBMESURES; ++i){
      total += mesures[i];
    }
    moyenne = total / NBMESURES;
  };

  void calcEcartType(){
    float var = 0;
    for(int i=0; i<NBMESURES; ++i){
      var += (mesures[i] - moyenne) * (mesures[i] - moyenne);
    }
    
    var /= NBMESURES;
    ecartType = sqrt(var)*2;
    //ecartType= ECARTTYPE;
    //delay(50);
  };

  void calcSorties(int detection){
    
    /*if(detection == 2) {
      tone(PINSPEAKER, 2000);
      return;
    } else if (detection == 1) {
      tone(PINSPEAKER, 20 + GAIN*abs(mesures[index]-moyenne)/(NBSIG*ecartType));
    } else {
      tone(PINSPEAKER, 0);
    }*/
    
    float mesureAbsolue = abs(mesures[index]-moyenne);
    
    //if(mesures[index] < (moyenne - NBSIG*ecartType/3) || mesures[index] > (moyenne + NBSIG*ecartType/3)) {
      //tone(PINSPEAKER, 50 + GAIN*abs((mesures[index]-moyenne))/(NBSIG*ecartType));
    tone(PINSPEAKER, 20 + GAIN*abs(detection)/(NBSIG*ecartType));
      
      //analogWrite(PINSPEAKER, abs(mesures[index]-moyenne)/100*128);
    //} else { 
    // tone(PINSPEAKER, 100);
    //}

    if(mesures[index] > (moyenne + NBSIG*ecartType)) {
      // Lecture au dessus de zéro
      //digitalWrite(PINLEDR, LOW);
      //digitalWrite(PINLEDG, LOW);
      digitalWrite(PINLEDB, HIGH);
      
      //mesures[index] = mesures[index] - NBSIG*ecartType;
      
    } else if(mesures[index] < (moyenne - NBSIG*ecartType)) {
      // Lecture en dessous de zéro
      //digitalWrite(PINLEDR, LOW);
      //digitalWrite(PINLEDG, HIGH);
      digitalWrite(PINLEDB, HIGH);

      //mesures[index] = mesures[index] + NBSIG*ecartType;
      
    } else {
      //analogWrite(PINLEDR, abs(mesures[index]-moyenne)/(ecartType)*1024);
      //digitalWrite(PINLEDG, LOW);
      digitalWrite(PINLEDB, LOW);
    }
  };
  
  int getMoyenne() { return moyenne; };
  int getEcartType() { return ecartType; };
  
  String toString() {

    static float moymobile = 0;

    moymobile += mesures[index];
    moymobile /= 2;
    
    String res = "";
    res.concat("Mesure:");
    res.concat(abs(mesures[index]-moyenne));
    /*res.concat(",moymob:");
    res.concat(moymobile-moyenne);  */
    res.concat(",lim:");
    res.concat(NBSIG*ecartType);/*
    res.concat(",Tone:");
    res.concat(toneAct);*/
    return res;
  };
};

Mesures * mesures = new Mesures();

void setup() {
  Serial.begin(9600);

  //analogReference(INTERNAL);

  //pinMode(PINLEDR, OUTPUT);
  pinMode(PINLEDG, OUTPUT);
  pinMode(PINLEDB, OUTPUT);
  pinMode(PINSPEAKER, OUTPUT); 

  mesures->init();
}

void loop() {
  mesures->ajoutMesure(analogRead(PINMESURE));
}
