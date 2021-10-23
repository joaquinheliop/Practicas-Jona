/*Probar libreria CRC o terminar el calculo CRC*/
#include "CRC16.h"
#include "CRC.h"

CRC16 crc;

//Configuracion de tiempos
#define T 250 //period in micro seconds
#define D 120 //tolerance in micro seconds

// Configuracion de pines
#define SHD 3
#define RF_PIN 2

//Array de inicio o cabecera de RFID
const bool start[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}; //start

char datos[64], char CRC[16];

void setup()
{

  Serial.begin(9600);

  pinMode(SHD, OUTPUT);
  pinMode(RF_PIN, INPUT);

  digitalWrite(SHD, HIGH);
  delay(2000);
  digitalWrite(SHD, LOW);

  Serial.println("Serial correcto");
}

void loop()
{

  int contador = 0;

  comienzo_busqueda();
  leer_bits_datos(datos);
  leer_bits_crc(CRC);

  crc.restart();
  crc.setReverseIn(true);
  crc.setReverseOut(true);
  crc.add((uint8_t *)datos, 64);

  unsigned int crcCalc = crc.getCRC();
  long int crcObt = strtol(CRC, 0, 2);

  Serial.println(strCrcCalc);
  Serial.println(crcObtenido);

  if (strCrcCalc == crcObtenido)
  {
    Serial.println("Datos correctos");

    for (int j = 63; j >= 0; j--)
    {
      Serial.print(datos[j]);
      contador++;
      if (contador == 8)
      {
        Serial.print(" ");
        contador = 0;
      }
    }
  }

  /*
  for (int j = 0; j < 16; j++)
     Serial.print(CRC[j]);
    contador++;
    if (contador == 8) {
     Serial.print(" ");
     contador = 0;
    }
  Serial.println("");
}
*/
}

void comienzo_busqueda()
//Realiza la busqueda de la cabecera del codigo RFID
{
  Serial.println("Buscando...");
  int i = 0;
  do
  {
    if (leer_pin() == start[i])
    {
      i++;
    }
    else
      i = 0;
  } while (i < 11);
  return;
}

void leer_bits_datos(bool cargar_datos[])
//Lee todos los bits recibidos y los almacena en el array pasado por parametro
{

  int j = 0;

  for (int i = 0; i < 72; i++)
  {
    if (i == 8 || i == 17 || i == 26 || i == 35 || i == 44 || i == 53 || i == 62 || i == 71)
    {
    }
    else
    {
      cargar_datos[j] = leer_pin();
      j++;
    }
  }
}

void leer_bits_crc(char cargar_datos[])
//Lee todos los bits recibidos y los almacena en el array pasado por pArametro
{
  int j = 0;

  for (int i = 0; i < 18; i++)
  {
    if (i == 8 || i == 17)
    {
    }
    else
    {
      cargar_datos[j] = leer_pin();
      j++;
    }
  }
}

char leer_pin()
//Lee el pin rf_pin para encontrar los bits segun codificacion manchester
{
  boolean state = digitalRead(RF_PIN);
  long start = micros();
  long delta;
  long time_out = 2 * T + D;
  do
  {
    do
    {
      delta = micros() - start;
    } while (state == digitalRead(RF_PIN));

    if (delta >= time_out)
    {
      return 3;
    }
    else
    {
      state = !state;
    }

    if (delta <= (2 * T + D) && delta >= (2 * T - D)) //Aca estamos sincronizados
    {
      state = digitalRead(RF_PIN);
      start = micros();

      do
      {
        delta = micros() - start;
      } while (state == digitalRead(RF_PIN));

      if ((delta <= T + D) && (delta >= T - D)) //Cuando el tiempo es T
      {
        start = micros();

        do
        {
          delta = micros() - start;
        } while (state == digitalRead(RF_PIN));

        if ((delta <= T + D) && (delta >= T - D))
        //Cuando el tiempo es T por segunda vez
        {
          return state;
        }
        else
        {
          return 3;
        }

        else if (delta <= (2 * T + D) && delta >= (2 * T - D)) //Cuando el tiempo es 2T
        {
          return !state;
        }
        else // Cuando el tiempo no es T o 2T
        {
          return 3;
        }
      }
      else
      {
      }
    }
    while (true)
      ;
  }
