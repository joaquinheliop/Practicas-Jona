#include  <Arduino.h>
#define DEMOD_OUT    1
#define SHD 2
#define pinBuzzer 3
#define DELAYVAL    320  //384 //standard delay for manchester decode
#define TIMEOUT     1000  //standard timeout for manchester decode

byte tagData[5];         //Holds the ID numbers from the tag
byte tagDataBuffer[5];  //A Buffer for verifying the tag data
int readCount = 0;        //the number of times a tag has been read
boolean verifyRead = false;     //true when a tag's ID matches a previous read, false otherwise
boolean tagCheck = false;       //true when a tag has been read, false otherwise
char  tmp[2];
byte  checksum;

bool decodeTag(unsigned char *buf);
void transferToBuffer(byte *tag_data, byte *tag_data_buffer);
bool compareTagData(byte *tag_data_1,byte *tag_data_2);



//Manchester decode 
bool  decodeTag(unsigned char *buf)  
{  
    unsigned char i=0; 
    unsigned short timeCount;  
    unsigned char timeOutFlag=0; 
    unsigned char row,col;    
    unsigned char row_parity;  
    unsigned char col_parity[5];  
    unsigned char dat;      
    unsigned char searchCount=0;      
    unsigned char j;  
    while(1)  
    {                 
        timeCount=0;      
        while(0==digitalRead(DEMOD_OUT))  //watch for DEMOD_OUT to go low
        {  
            
            if(timeCount>=TIMEOUT)  //if we pass TIMEOUT milliseconds, break out of the loop
                break;
            else   
                timeCount++;        
        }     
  
        if (timeCount>=600)           
             return false;  
        else   
                timeCount=0;  
                              
        delayMicroseconds(DELAYVAL);      
        if(digitalRead(DEMOD_OUT)) 
        {             
                for(i=0;i<10;i++)  // 11 header bits
                {    
                  timeCount=0;  //restart counting
                  while(1==digitalRead(DEMOD_OUT))//while DEMOD out is high
                  {
                    if(timeCount==TIMEOUT)  
                    {       
                      timeOutFlag=1;                                      
                      break;   
                    }  
                    else   
                    timeCount++;  
                  }                                 
                                                         
                  if(timeOutFlag) 
                  {  
                     break;  
                  }  
                  else  
                  {                            
                     delayMicroseconds(DELAYVAL);       
                      if( 0==digitalRead(DEMOD_OUT) )                                                                                                          
                           break;  
                  }                    
                }//end for loop

                if(timeOutFlag)
                {       
                        timeOutFlag=0;  
                        return false;  
                }  
      
                if(i==10)  //Receive the data
                {     
                    timeOutFlag=0;  
                    timeCount=0;     
                    while(1==digitalRead(DEMOD_OUT))
                    {            
                            if(timeCount==TIMEOUT)  
                            {                         
                                    timeOutFlag=1;  
                                    break;      
                            }  
                            else  
                            {  
                                    timeCount++;  
                            }     
                                              
                            if(timeOutFlag)      
                            {  
                                    timeOutFlag=0;                                            
                                    return false;  
                            }  
                    }  
                  
                    col_parity[0]=col_parity[1]=col_parity[2]=col_parity[3]=col_parity[4]=0;   
                    for(row=0;row<14;row++)   
                    {  
                            row_parity = 0;  
                            j = row>>1;
                            for(col=0,row_parity=0;col<5;col++) 
                            {  
                                 delayMicroseconds(DELAYVAL); 
                                 if(digitalRead(DEMOD_OUT))  
                                        dat=1;  
                                    else  
                                        dat=0; 
                             
                                  if(col<4&&row<10)   
                                  {  
                                      buf[j] <<= 1;  
                                      buf[j] |= dat;                                                
                                  }  
                              
                                  row_parity += dat;      
                                  col_parity[col] += dat;  
                                  timeCount=0;                 
                                  while(digitalRead(DEMOD_OUT)==dat)  
                                  { 
                                       if(timeCount==TIMEOUT)       
                                       {     
                                         timeOutFlag=1;  
                                         break;            
                                       }  
                                       else                                                     
                                         timeCount++;  
                                              
                                    }                                         
                                    if(timeOutFlag)                                 
                                      break;   
                            }  
                           
                            if(row<10)
                            {  
                               if((row_parity&0x01)||timeOutFlag) //Row parity 
                                    {
                                            timeOutFlag=1;   
                                            break;          
                                    }  
                            }      
                    }  
                      
                                   
                    if( timeOutFlag||(col_parity[0]&0x01)||(col_parity[1]&0x01)||(col_parity[2]&0x01)||(col_parity[3]&0x01) )  //Column parity 
                    {                                                  
                            timeOutFlag=0;                               
                            return false;  
                    }   
                    else   
                    {                             
                            return true;  
                    }  
                }//end if(i==8)             
                 return false;  
        }//end if(digitalRead(DEMOD_OUT))  
    }
};

bool compareTagData(byte *tag_data_1,byte *tag_data_2)  
{    
  for(int j=0;j<5;j++)                    
    {
    if (tag_data_1[j] != tag_data_2[j])
        {
         return false; //if any of the ID numbers are not the same, return a false    
        }   
  }
      return true;  //all id numbers have been verified
}


void transferToBuffer(byte *tag_data,byte *tag_data_buffer)  
{    
  for(int j=0;j<5;j++)                      
    {
    tag_data_buffer[j] = tag_data[j];
    }
}

void exito () 
{
  digitalWrite(pinBuzzer,HIGH);
  delay (500);
  digitalWrite(pinBuzzer,LOW);

}


void setup() {
  // put your setup code here, to run once:
  //OSCCAL = 0x95;
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  tagCheck = decodeTag(tagData); //run the decodetag to check for the tag  
    if (tagCheck==true)   //if a 'true' is returned from the decodetag function, a tag was succesffuly scanned 
    { 
      readCount++;      //increase count since we've seen a tag
            
      if(readCount==1)//if have read a tag only one time, proceed
      {
        transferToBuffer(tagData, tagDataBuffer);  //place the data from the current tag read into the buffer for the next read
      }
      
      else if(readCount==2)//if we see a tag a second time, proceed
      {
        verifyRead = compareTagData(tagData, tagDataBuffer); //run the compareTagData function to compare the data in the buffer (the last read) with the data from the current read
        if (verifyRead == true)  //if a 'true' is returned by compareTagData, the current read matches the last read
        {
           Serial.write(2); //Send a STX
           checksum = 0;
           for(int n=0;n<5;n++)
           {
             checksum ^= tagData[n];    //Calcula el checksum
             sprintf(tmp,"%.2X",tagData[n]);  //Convierte los bytes en valor ASCII HEX
             Serial.print(tmp); //Envia el dato por el puerto serial
           }
           sprintf(tmp,"%.2X",checksum);  //Convierte el valor del checksum en ASCII HEX
           Serial.print(tmp); //Envia el valor del checksum por el puerto serial
           Serial.print("\n\r");  //return character for next line
           Serial.write(3); //Send an ETX
             
          exito();
          
        }
        readCount=0;//because a tag has been  read twice, reset the readCount to '0' for the next tag
      }        
    }     

}
