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
  HexDumpEEPROM();
}

void EraseEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }
}

void WriteString() {
  String message = "Hello World";
  int address = 0;

  for (int i = 0; i < (message.length())+1; i++) // returns the string size with terminating null
  {
    EEPROM.write(address + i, message[i]);
  }
}

// A basic way to loop through all EEPROM addresses and print out their value.
void ReadString() {
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
void HexDumpEEPROM() {
  unsigned long addr = 0;  // used fro address in EEPROM
  byte c = 0;              // i=1 to 16 bytes, j=HEX index, c= single character
  short j = 0;
  char buffer[16];

  Serial.begin(9600);
  Serial.println("EEPROM DUMP:");
  do { /* Read 16 characters from the EEprom into our buffer */
    /* ------ Print the address padded with spaces ------ */
    if (addr < 1000) { Serial.print('0'); }  // space padding, char(32) is a space
    if (addr < 256) { Serial.print('0'); }   // space padding
    if (addr < 16) { Serial.print('0'); }    // space padding
    Serial.print(addr, HEX);                 // starting address for line
    Serial.print("  ");                      // seperator, could be replaced with a tab ("\t")
    for (short i = 0; i < 16; ++i)           // 16 bytes at a time
    {
      c = EEPROM.read(addr + i);  // read one byte
      buffer[i] = char(c);        // stuff the character in our buffer
      /*--- print HEX value ---*/
      if (c < 0x10) { Serial.print('0'); }  // pad HEX value with 0, 0x10 = F0 = 16
      Serial.print(c, HEX);                 // using HEX tells Serial how to format the number
      ++j;                                  // increment index for serperator
      if (j == 4)                           // if we have print 4 values
      {
        Serial.print(' ');  // print a seperator
        j = 0;              // set our index to zero to start over
      }
      /*--- Evaluate character ---*/
      if (c < 32) { buffer[i] = '.'; }   // do not print control codes
      if (c == 92) { buffer[i] = '.'; }  // do not print "\"
      if (c > 126) { buffer[i] = '.'; }  // do not print 'extended' characters
    }
    Serial.println(buffer);  // Print out the 16byte buffer in ASCII format
    addr = addr + 16;
  } while (addr < E2END);  // E2END is the end of the EEPROM
  /*  used to check ending address
  Serial.println(addr,DEC);
  Serial.println(E2END,DEC);
  */
}

void loop() {}