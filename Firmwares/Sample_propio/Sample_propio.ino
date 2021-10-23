#define SHD 3
#define MOD 2

byte numero_muestras = 10;
unsigned long periodo = 270;//uS
unsigned long muestreo = periodo / numero_muestras;

bool captura_bit[10];


void setup() {

  Serial.begin(9600);
  pinMode(MOD, INPUT);
  pinMode(SHD, OUTPUT);
  digitalWrite(SHD, HIGH);
  delay(4000);
  digitalWrite(SHD, LOW);
}

void loop() {
  muestra();
  delay(1000);
}

void muestra() {
  unsigned int i = 0;

  for (i = 0; i < numero_muestras; i++) {
    delayMicroseconds (muestreo);
    captura_bit[i] = digitalRead(MOD);
  }

  if (captura_bit[0]==captura_bit[1]==captura_bit[2]==captura_bit[3]==captura_bit[4]==0){
    
  }

 /* for (i = 0; i < numero_muestras; i++) {
    Serial.print(captura_bit[i]);

  }*/
  Serial.println("1");
}
