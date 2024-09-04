// https://docs.arduino.cc/learn/programming/eeprom-guide/
// ATMega328P has 3 types of memory:
// Flash memory for programs
// SDRAM for volitile data
// 1k EEPROM for storage of non-volitile user data.

#include <EEPROM.h>

void setup() {
  EraseEEPROM();
  WriteString();
  //ReadString();
  PrintHexDump();
}

void EraseEEPROM()
{
    for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void WriteString()
{
  char message[] = "Hello World";
  int address = 0;

  byte len = sizeof(message);

  for (int i = 0; i < len; i++)

  {
    EEPROM.write(address + i, message[i]);
  }
}

// A basic way to loop through all EEPROM addresses and print out their value.
void ReadString()
{
  int index = 0;

  Serial.begin(9600);
  Serial.println();
  Serial.println();

  while (index < EEPROM.length()) {
    // Example output
    // 0 = 68 h
    Serial.print(index, HEX);
    Serial.print(" = ");
    Serial.print(EEPROM[index], HEX);
    Serial.print(" ");
    Serial.println(char(EEPROM[index]));
    index++;
  }

  Serial.println("END");
}

// Hex dump version of ReadString()
// Taken from https://forum.arduino.cc/t/eeprom-hexdump-strange-characters-at-beginning-of-serial-output-solved/179066
void PrintHexDump()
{ 
  unsigned long addr = 0;                              // used fro address in EEPROM
  byte i,j,c;                                          // i=1 to 16 bytes, j=HEX index, c= single character
  char buffer[22];   

  Serial.begin(9600);                            // initalize serial port
  buffer[0] = ' ';                               // put space at beginning of char array
  buffer[1] = ' ';                               // put second space at beginning of char array
  buffer[18] =0;                                 // string termination for buffer
  buffer[19] =0;                                 // belts and suspenders
  Serial.println();
  Serial.println();
  Serial.println (F("EEPROM DUMP:"));            // information, notice the F() wrapper
  do
  { /* Read 16 characters from the EEprom into our buffer */
    /* ------ Print the address padded with spaces ------ */
    //if (addr<1000) {Serial.print(' ');}          // space padding, char(32) is a space
    if (addr<256) {Serial.print(' ');}           // space padding
    if (addr<16) {Serial.print(' ');}            // space padding
    Serial.print (addr, HEX);                    // starting address for line
    Serial.print ("  ");                         // seperator, could be replaced with a tab ("\t")
    for (i=0; i<16; ++i)                         // 16 bytes at a time
      {  c= EEPROM.read(addr + i);               // read one byte
         buffer[i+2]=char(c);                      // stuff the character in our buffer
         /*--- print HEX value ---*/
         if (c < 0x10) {Serial.print('0');}      // pad HEX value with 0, 0x10 = F0 = 16
         Serial.print (c, HEX);                  // using HEX tells Serial how to format the number
         ++j;                                    // increment index for serperator
         if (j == 4)                             // if we have print 4 values
           {  Serial.print(' ');                 // print a seperator 
              j=0;                               // set our index to zero to start over
           }
         /*--- Evaluate character ---*/
         if (c < 32) {buffer[i+2]='.';}          // do not print control codes
         if (c ==92) {buffer[i+2]='.';}          // do not print "\"
         if (c >126) {buffer[i+2]='.';}          // do not print 'extended' characters
      }
    Serial.println(buffer);                      // begin wiht a new line
    addr=addr +16;
  } while (addr < E2END);   // E2END is the end of the EEPROM
  /*  used to check ending address
  Serial.println(addr,DEC);
  Serial.println(E2END,DEC);
  */ 
}

void loop() {}