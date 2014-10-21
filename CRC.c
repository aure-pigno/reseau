/*
 * The width of the CRC calculation and result.
 * Modify the typedef for a 16 or 32-bit CRC standard.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t crc;

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8

crc
crcSlow(uint8_t const message[], int nBytes)
{
    crc  remainder = 0;	


    /*
     * Perform modulo-2 division, a byte at a time.
     */
     int byte;
    for (byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        remainder ^= (message[byte] << (WIDTH - 8));

        /*
         * Perform modulo-2 division, a bit at a time.
         */
         uint8_t bit;
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (remainder);

}   /* crcSlow() */

int main(int argc, char** argv){
	const uint8_t msg = 0xFF;
	printf("%d a donne comme CRC : %d\n",msg,crcSlow(&msg,1));
	return 0;
}
