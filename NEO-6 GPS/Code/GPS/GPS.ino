// # Editor     : Lauren from DFRobot 
// # Date       : 22.02.2012 
  
// # Product name: DFRduino GPS Shield-LEA-5H 
// # Product SKU : TEL0044 
// # Version     : 1.0 
  
// # Update the library and sketch to compatible with IDE V1.0 and earlier 
  
// # Description: 
// # The sketch for using the DFRduino GPS Shield-LEA-5H 
  
// # Connection: 
// #        Directly connect the shield to the Arduino controller 
// #        If you'd like to drive the shield via UART interface, you may need to connect the jumpers on the board. 
// #         
  
  
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h" 
#define WireSend(args) Wire.write(args) 
#define WireRead(args) Wire.read(args) 
#define printByte(args) Serial.write(args) 
#define printlnByte(args)  Serial.write(args),Serial.println() 
#else 
#include "WProgram.h" 
#define WireSend(args) Wire.send(args) 
#define WireRead(args) Wire.receive(args) 
#define printByte(args) Serial.print(args,BYTE) 
#define printlnByte(args)  Serial.println(args,BYTE) 
#endif 
  
#include <Wire.h> 
  
#define BUFFER_LENGTH 10//Define the buffer length 
  
int GPSAddress = 0x42;//GPS I2C Address 
  
double Datatransfer(char *data_buf,char num)//Data type converter：convert char type to float 
{                                           //*data_buf:char data array ;num:float length 
  double temp=0.0; 
  unsigned char i,j; 
  
  if(data_buf[0]=='-')//Negative situation
  { 
    i=1; 
    //The array of character data converted to an integer and accumulate
    while(data_buf[i]!='.') 
      temp=temp*10+(data_buf[i++]-0x30); 
    for(j=0;j<num;j++) 
      temp=temp*10+(data_buf[++i]-0x30); 
    //Integer converted into a floating-point number
    for(j=0;j<num;j++) 
      temp=temp/10; 
    //Converted into negative
    temp=0-temp; 
  } 
  else//Positive cases
  { 
    i=0; 
    while(data_buf[i]!='.') 
      temp=temp*10+(data_buf[i++]-0x30); 
    for(j=0;j<num;j++) 
      temp=temp*10+(data_buf[++i]-0x30); 
    for(j=0;j<num;j++) 
      temp=temp/10 ; 
  } 
  return temp; 
} 
void rec_init()//initial GPS 
{ 
  Wire.beginTransmission(GPSAddress); 
  WireSend(0xff);//Address where to send data    
  Wire.endTransmission();  
  
  Wire.beginTransmission(GPSAddress); 
  Wire.requestFrom(GPSAddress,10);//It requires 10 bytes read from a GPS device
} 
char ID()//Receiving data ID
{ 
  char i = 0; 
  char value[7]={ 
    '$','G','P','G','G','A',','      };//Content ID to receive GPS data
  char buff[7]={ 
    '0','0','0','0','0','0','0'      }; 
  
  while(1) 
  { 
    rec_init();//Receiving data initialization    
    while(Wire.available())    
    {  
      buff[i] = WireRead();//To receive serial data  
      if(buff[i]==value[i])//Contrast is the right ID 
      { 
        i++; 
        if(i==7) 
        { 
          Wire.endTransmission();//Receiving end
          return 1;//Been received returns
        } 
      } 
      else
        i=0; 
    } 
    Wire.endTransmission();//Receiving end
  } 
} 
void UTC()//Get time information
{ 
  char i = 0,flag=0; 
  char value[7]={ 
    '$','G','P','G','G','A',','   }; 
  char buff[7]={ 
    '0','0','0','0','0','0','0'       }; 
  char time[9]={ 
    '0','0','0','0','0','0','0','0','0'    };//Data storage time 
  double t=0.0; 
  
  while(1) 
  { 
    rec_init();     
    while(Wire.available())    
    {  
      if(!flag) 
      {  
        buff[i] = WireRead(); 
        if(buff[i]==value[i]) 
        { 
          i++; 
          if(i==7) 
          { 
            i=0; 
            flag=1; 
          } 
        } 
        else
          i=0; 
      } 
      else
      { 
        time[i] = WireRead(); 
        i++; 
        if(i==9) 
        { 
          t=Datatransfer(time,2);//Converted into floating-point data
          t=t+80000.00;//Converts time into GMT
          Serial.println(t);//Data output time 
          Wire.endTransmission(); 
          return; 
        } 
      } 
    } 
    Wire.endTransmission();  
  } 
} 
void rec_data(char *buff,char num1,char num2)//Receiving data Functions
{                                            //*buff：num1; storing the received data array: number comma; num2: array length.
  char i=0,count=0; 
  
  if(ID()) 
  { 
    while(1) 
    { 
      rec_init();     
      while(Wire.available())    
      {  
        buff[i] = WireRead(); 
        if(count!=num1) 
        {   
          if(buff[i]==',') 
            count++; 
        } 
        else
        { 
          i++; 
          if(i==num2) 
          { 
            Wire.endTransmission(); 
            return; 
          } 
        } 
      } 
      Wire.endTransmission(); 
    } 
  } 
} 
void latitude()//Get latitude information
{ 
  char lat[10]={ 
    '0','0','0','0','0','0','0','0','0','0' };//Latitude data storage
  rec_data(lat,1 ,10);//Latitude data received
  Serial.println(Datatransfer(lat,5),5);//To convert latitude data into floating-point data and outputs
} 
void lat_dir()//Get latitude information
{ 
  char dir[1]={'0'};//Latitude data storage
  rec_data(dir,2,1);//Latitude data reception direction
  printlnByte(dir[0]);//Latitude direction information output
} 
void  longitude()//Get longitude information
{ 
  char lon[11]={ 
    '0','0','0','0','0','0','0','0','0','0','0' };//Longitude data storage
  rec_data(lon,3,11);//Longitude data received
  Serial.println(Datatransfer(lon,5),5);//Convert longitude data into floating-point data and outputs
} 
void lon_dir()//Get longitude direction information
{ 
  char dir[1]={'0'}; 
  rec_data(dir,4,1); 
  printlnByte(dir[0]);//Latitude direction information output
} 
void altitude()//Get elevations
{ 
  char i=0,count=0; 
  char alt[8]={ 
    '0','0','0','0','0','0','0','0' }; 
  
  if(ID()) 
  { 
    while(1) 
    { 
      rec_init();     
      while(Wire.available())    
      {  
        alt[i] = WireRead(); 
        if(count!=8) 
        {   
          if(alt[i]==',') 
            count++; 
        } 
        else
        { 
          if(alt[i]==',') 
          { 
            Serial.println(Datatransfer(alt,1),1); 
            Wire.endTransmission(); 
            return; 
          } 
          else
            i++; 
        } 
      } 
      Wire.endTransmission(); 
    } 
  } 
} 
void setup() 
{ 
  Wire.begin();//I2C initialization
  Serial.begin(9600);//Set the baud rate
  Serial.println("DFRobot DFRduino GPS Shield v1.0"); 
  Serial.println("$GPGGA statement information: "); 
} 
void loop() 
{ 
  while(1) 
  { 
    Serial.print("UTC:"); 
    UTC(); 
    Serial.print("Lat:"); 
    latitude(); 
    Serial.print("Dir:"); 
    lat_dir(); 
    Serial.print("Lon:"); 
    longitude(); 
    Serial.print("Dir:"); 
    lon_dir(); 
    Serial.print("Alt:"); 
    altitude(); 
    Serial.println(' '); 
    Serial.println(' '); 
  } 
}


