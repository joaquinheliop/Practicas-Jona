//The receive pin must be 11

#include "manchester_rx.h"

#define SHD 3
#define MOD 11

bool payload[42];
Receiver RxD;

void setup() {
  Serial.begin(9600);
  pinMode(SHD, OUTPUT);
  digitalWrite(SHD, HIGH);
  delay(2000);
  digitalWrite(SHD, LOW);
  Serial.println("started");
}

void loop() {
  int contador = 0;
  //seeking for the start byte
  //this is a blocking function
  
  RxD.seek_start();
  Serial.println("start byte found");
  //read payload
  //payload is an array containing all the bits of the received byte
  RxD.read_byte(payload);


  for (int j = 0; j < 42; j++) {
    Serial.print(payload[j]);
    contador++;
    if (contador == 8) {
      Serial.print(" ");
     }
    if (contador == 9) {
      Serial.print(" ");
      contador = 0;
    }
  }




  Serial.println("");

}

byte ArrayToAscii(byte arr[])
/*utility to convert an array of 8 bits to ascii code*/
{
  byte ascii = 0;
  for (int i = 7; i >= 0; i--)
  {
    ascii = ascii << 1;
    ascii = arr[i] | ascii;
  }
  return ascii;
}
