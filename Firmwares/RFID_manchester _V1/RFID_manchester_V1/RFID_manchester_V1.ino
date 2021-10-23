/*Probar libreria CRC o terminar el calculo CRC*/



//Configuracion de tiempos
#define T 500 //period in micro seconds
#define D 250 //tolerance in micro seconds

// Configuracion de pines
#define SHD 3
#define RF_PIN 2

//Array de inicio o cabecera de RFID
const bool start[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}; //start


bool datos[79], CRC[16], CRC_calculado[16], original[79], resto[16];


void setup() {

  Serial.begin(9600);

  pinMode(SHD, OUTPUT);
  pinMode(RF_PIN, INPUT);

  digitalWrite(SHD, HIGH);
  delay(2000);
  digitalWrite(SHD, LOW);

  Serial.println("Serial correcto");

}

void loop() {
  int contador = 0;

  completar_ceros(datos);

  //Comenzamos la busqueda de cabecera (000000000001)
  comienzo_busqueda();

  leer_bits_datos(datos);

  leer_bits_crc(CRC);


  
  if (operacion_xor(datos, CRC)) {
    Serial.println("Datos correctos");

    for (int j = 78; j >= 15; j--)
    {
      Serial.print(original[j]);
      contador++;
      if (contador == 8) {
        Serial.print(" ");
        contador = 0;
      }
    }
  }


  /* for (int j = 0; j < 16; j++)
     Serial.print(CRC[j]);
    contador++;
    if (contador == 8) {
     Serial.print(" ");
     contador = 0;
    }*/
  //Serial.println("");
}



void comienzo_busqueda()
//Realiza la busqueda de la cabecera del codigo RFID
{
  //Serial.println("Buscando...");
  int i = 0;
  do {
    if (leer_pin() == start[i]) {
      i++;
    }
    else i = 0;
  } while (i < 11);

  //Serial.println("Se encontro la cabecera");
  return;
}


void leer_bits_datos(bool cargar_datos[])
{

  int j = 15;

  //Lee todos los bits recibidos y los almacena en el array pasado por parametro
  for (int i = 0; i < 72; i++)
  {
    if (i == 8 || i == 17 || i == 26 || i == 35 || i == 44 || i == 53 || i == 62 || i == 71) {

    } else {

      cargar_datos[j] = leer_pin();
      j++;

    }
  }
}


void leer_bits_crc(bool cargar_datos[])
{
  int j = 0;
  //Lee todos los bits recibidos y los almacena en el array pasado por porametro
  for (int i = 0; i < 18; i++)
  {
    if (i == 8 || i == 17) {

    } else {

      cargar_datos[j] = leer_pin();
      j++;

    }
  }
}

/*bool operacion (bool datos[], bool crc[]) {
  for (int j=79; j>=0; j--){
  for (int i=15; i>=0; i--){
  datos[j] = datos[j]^crc[i]

  }*/

bool leer_pin()
//Lee el pin rf_pin para encontrar los bits segun codificacion manchester
{
  
  long delta;
  long time_out = T + D; //if we do not detect 2 valids edges in this interval someting's wrong
  boolean state = digitalRead(RF_PIN);
  long start = micros();
  
  do
  {
    do
    {
      delta = micros() - start;
    } while ( (state == digitalRead(RF_PIN)));

    if (delta >= time_out) {
      return 3;
    } else {
      state = !state; //it is not a time out so the state of the ligne has changed
    }


    if ( (delta <= T + D) && (delta >= T - D) )
    {
      return !state;
    }
    else
    {
    }
  }
  while (true);
}

void completar_ceros(bool cargar_datos[]) {
  //Agregamos los ultimos 15 ceros para la operacion XOR
  for (int k = 0; k < 15; k++) {
    cargar_datos[k] = 0;
  }
}

bool operacion_xor(bool datoXor[], bool crcXor[]) {

  //Copiar vector
  for (int i = 0; i < 79; i++) {
    original[i] = datoXor[i];
  }

  int j = 14;
  for (int i = 78; i > 14; i--) {
    if (datoXor[i] == 1) {
      int k = i;
      do {
        datoXor[k] = datoXor[k] ^ crcXor[j];
        j--;
        k--;
      }  while (j >= 0);
      j = 14;
    }
  }


  //Agrego resto al vector original
  for (int i = 14 ; i >= 0; i--) {
    original[i] = datoXor[i];
  }

  j = 14;
  for (int i = 78; i > 14; i--) {
    if (datoXor[i] == 1) {
      int k = i;
      do {
        datoXor[k] = original[k] ^ crcXor[j];
        j--;
        k--;
      }  while (j >= 0);
      j = 14;
    }
  }
/*
  //Mostramos vector
 for (int j = 78; j >= 0; j--) {
    Serial.print(datoXor[j]);

  }
  Serial.println("Fin xor");
*/

  //Check si todo el resultado es 0
  for (int i = 0; i < 79; i++) {
    if (datoXor[i] == 1) {
      return false;
    }
  }
  return true;

}
