#define SHD 3
#define MOD 2

//byte numero_muestras = 10;
//unsigned long periodo = 270;//uS
//unsigned long muestreo = periodo / numero_muestras;

//bool captura_bit[10];
bool primera_captura, bandera = 0;
unsigned long inicio, fin;

void setup() {

  Serial.begin(9600);
  pinMode(MOD, INPUT);
  pinMode(SHD, OUTPUT);
  digitalWrite(SHD, HIGH);
  delay(4000);
  digitalWrite(SHD, LOW);
  
  attachInterrupt(digitalPinToInterrupt(SHD), capturaDato, CHANGE);
}

void capturaDato() {
  
   primera_captura = digitalRead(SHD);
  if (!bandera) {
    bandera = 1;
  } else {
    bandera = 0;
  }
  conteo();
}

void conteo() {
  int i;
  if (bandera) {
    Serial.print("1|23123");
    i++;
  } 
  Serial.println("Tiempo: "+ (String)i);
  loop();
  }

void loop() {
  Serial.println("loop");
}
