// ATMega328P has 3 types of memory:
// Flash memory for programs - 32k
// SRAM for volitile data - 2k
// EEPROM for storage of non-volitile user data - 1k

// Reference: https://docs.arduino.cc/learn/programming/memory-guide/
// Reference: https://www.seeedstudio.com/blog/2021/04/26/managing-arduino-memory-flash-sram-eeprom/

#define SRAM_END 0x08ff

void setup() {
  HexDumpSRAM();
}

void HexDumpSRAM() {
  // A local variable we would expect to see on the stack.
  char stuff[] = "0123456789ABCDEF";
  
  // Some data allocated on the heap
  char* message = (char*) malloc(200 * sizeof(char)); // Allocate memory for a string of 200 characters
  // Check if the memory has been allocated successfully
  if (message == NULL) {
    Serial.println(F("Memory alloc failed")); // This hardcoded string won't appear in SRAM as the F() macro ensures it's written to flash to save memory in SRAM
    return;
  }
  // Write a string of data into the allocated memory
  strncpy(message, "This is data allocated on the heap. You will probably also see it in the initialized data section as this string is hardcoded into the binary.",200);

  Serial.begin(9600);
  Serial.println("32 general-purpose CPU registers (R0-R31):");
  DisplayDump((byte *)0x0, (byte *)0x001F);
  Serial.println();

  Serial.println("Standard I/O registers:");
  DisplayDump((byte *)0x0020, (byte *)0x005F);
  Serial.println();

  Serial.println("Extended I/O registers:");
  DisplayDump((byte *)0x0060, (byte *)0x00FF);
  Serial.println();

  Serial.println("Internal SRAM (2 KB):");
  DisplayDump((byte *)0x0100, (byte *)0x08FF);
  Serial.println();
}


void DisplayDump(byte *startAddr, byte *endAddr) {
  
  byte i, j, c;
  byte buffer[16];
  byte *addr = startAddr;  // address in SRAM
  
  // Loop through the whole of provided range, 16 bytes at a time.
  do {       
    /* ------ Print the address padded with spaces ------ */
    if (addr < 0x10)   {Serial.print('0'); }  // space padding
    if (addr < 0x100)  {Serial.print('0'); }  // space padding
    if (addr < 0x1000) {Serial.print('0');}   // space padding
    Serial.print((int)addr, HEX);           // starting address for line
    Serial.print("  ");                     // seperator, could be replaced with a tab ("\t")

    /* Read 16 characters from SRAM into the buffer */  
    for (i = 0; i < 16; ++i)                // 16 bytes at a time
    {
      c= *(addr + i);  // read one byte
      buffer[i] = c;    // stuff the character in the display buffer

      /*--- print HEX value ---*/
      if (c < 0x10) { Serial.print('0'); }  // pad value less than 16 (0x10) with a leading 0, ie, 5 becomes 05, C becomes 0C
      Serial.print(c, HEX);                 // using HEX tells Serial how to format the number

      /*--- print bytes is blocks of 4 --*/
      ++j;                                  // increment index for separator
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

    Serial.println((char *)buffer);  // begin with a new line

    addr = addr + 16;

  } while (addr < endAddr); // Loop through all bytes in the provided range.
}

void loop() {}