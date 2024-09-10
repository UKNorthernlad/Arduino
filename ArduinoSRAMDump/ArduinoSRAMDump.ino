// ATMega328P has 3 types of memory:
// Flash memory for programs - 32k
// SDRAM for volitile data - 2k
// EEPROM for storage of non-volitile user data - 1k

#define SDRAM_END 0x7ff

void setup() {
  HexDumpSDRAM();
}

void HexDumpSDRAM() {
  byte *addr = 0;  // address in SDRAM
  byte i, j, c;
  byte buffer[16];

  // A local variable we would expect to see on the stack.
  char stuff[] = "0123456789ABCDEF";
  
  // Some data allocated on the heap
  char* message = (char*) malloc(20 * sizeof(char)); // Allocate memory for a string of 20 characters
  // Check if the memory has been allocated successfully
  if (message == NULL) {
    Serial.println("Memory allocation failed");
    return;
  }
  // Write a string of data into the allocated memory
  strncpy(message, "Hello, Arduino!",20);

  Serial.begin(9600);

  Serial.println(stuff);
  Serial.println();
  Serial.println("SDRAM DUMP:");
  
  // Loop through the whole of SDRAM, 16 bytes at a time.
  do {       
    /* ------ Print the address padded with spaces ------ */
    if (addr < 0x10)   {Serial.print('0'); }  // space padding
    if (addr < 0x100)  {Serial.print('0'); }  // space padding
    if (addr < 0x1000) {Serial.print('0');}   // space padding
    Serial.print((int)addr, HEX);           // starting address for line
    Serial.print("  ");                     // seperator, could be replaced with a tab ("\t")

    /* Read 16 characters from SDRAM into the buffer */  
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

  } while (addr < SDRAM_END);  // SDRAM_END = 0x7ff = end of SDRAM
}

void loop() {}