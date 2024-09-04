#include <avr/pgmspace.h>

void setup()
{
	Serial.begin(9600);

	uint32_t address;

	for (address = 0x0000; address < 0x8000; address += 256) {
		hexdump(address, 256);
		Serial.print("\r\n");
	}
}


void hexdump(uint32_t address, uint32_t bytes)
{
	#define bufsiz 32
	char *hexbuf = 0;
	char *ascbuf = 0;
	const char *datamask = "%02X ";
	const char *addrmask = "\r\n%04X: ";

	uint8_t data;
	uint16_t ptr;

	uint32_t x;

	hexbuf = (char *) malloc((bufsiz) * sizeof(char));
	ascbuf = (char *) malloc((bufsiz) * sizeof(char));

	if(!(hexbuf && ascbuf)) {
		Serial.print("Buffer malloc failed\n");
		return;
	}

	for (ptr = 0; ptr < bufsiz; ptr++) {
		hexbuf[ptr] = 0;
		ascbuf[ptr] = 0;
	}

	Serial.print("      +0 +1 +2 +3 +4 +5 +6 +7   +8 +9 +A +B +C +D +E +F");

	for(x = address; x < address + bytes; x++) {

		if ((x & 0x0f) == 0) {
			sprintf(hexbuf, addrmask, x);
			Serial.print(hexbuf);
			ptr = 0;
		}

		data = pgm_read_byte(x);

		ascbuf[ptr++] = ((data > 31) && (data < 128)) ? data : '.';

		sprintf(hexbuf, datamask, data);
		Serial.print(hexbuf);

		if ((((x + 1) & 0x07) == 0) && ((x & 0x0f) != 0x0f)) {
			Serial.print("- ");
		}

		if (ptr == 16) {
			Serial.print("  ");
			Serial.print(ascbuf);
		}
	}

	Serial.print("\r\n");

	free(hexbuf);
	free(ascbuf);
}

void loop()
{
}