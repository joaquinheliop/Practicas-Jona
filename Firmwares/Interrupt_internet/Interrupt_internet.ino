//#include <LED.h>

#define CLK 2
#define OUT 3
#define MOD 8
#define SHD 7

//LED led = LED(13);
boolean val = false;
unsigned char count,bitno,i;
char sequ[10];
char* cadena;


//Init() function
//
void setup()
{
// global variables
count = bitno = 0;
cadena = sequ;
for (i=0;i<10;i++) sequ=0;*

// i/o setting*
pinMode(CLK,INPUT);*
pinMode(OUT,INPUT);*
pinMode(MOD,OUTPUT);*
pinMode(SHD,OUTPUT);*
// em4095*
digitalWrite(MOD,HIGH);*
digitalWrite(SHD,HIGH);*
delay(100);*
digitalWrite(SHD,LOW);*
digitalWrite(MOD,LOW);*
// led*
led.blink(500,2);*
// UART*
Serial.begin(9600);*
//Serial.println(“em4095 RFid reader for ARDUINO”);*
// interrupts*
attachInterrupt(0,rf_clock,RISING);*
}
void loop()
{
// process header*
if (!(sequ[0] & 0xff) || !(sequ[1] & 0x01))*
{*
detachInterrupt(0);*
count = bitno = 0;*
cadena = sequ;*
_ for (i=0;i<10;i++) sequ = 0;_
* led.on();*
* Serial.print("Bad header: ");*
* Serial.print(sequ[0],HEX);*
* Serial.print(",");*
* Serial.print(sequ[1],HEX);*
* Serial.println();*
* delay(100);*
* led.off();*
* attachInterrupt(0,rf_clock,RISING);
_ }_
_ // process frame*_
* if (count >= 64)*
* {*
* detachInterrupt(0);*
* cadena = sequ;*
* count = bitno = 0;*
* for (i=0;i<8;i++)*
* {*
_ Serial.print(sequ*,DEC);
Serial.print(",");
}
Serial.println(); *_
* led.blink(500,2);*

_ for (i=0;i<10;i++) sequ = 0;_

* delay(1000);*
* attachInterrupt(0,rf_clock,RISING);
_ }
}
//
* interrupt routine*
* loads the bits array*/
void rf_clock()
{
val = !digitalRead(OUT);
bitWrite(cadena,bitno,val);
count++;
* bitno++;
if (bitno == 8)
{
cadena ++;
bitno = 0;
}
}
[/quote]*_
