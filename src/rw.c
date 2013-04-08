#include <stdio.h>
#include "spi_mcp_rasp.h"
#include "mcp3901.h"
#include <bcm2835.h>

int main(int argc, char* argv[])
{
#ifdef BURST
	int i,j;
	uint8_t firstDR, DR;
	struct reading readingBuf;
#endif
	if (SPIinitialize())
	{
		printf("ERROR: SPI device is not initialized\n");
		return -1;
	}
#ifdef CONFIG
	setMCP3901Config();	
	printMCP3901Config();
#endif
#ifdef READCONFIG
	printMCP3901Config();
#endif
#ifdef BURST
	SPIReadContInit();
	firstDR = 1;
	DR = 0;

	i = 1000;
	while(i > 0)
	{
		//printf("i = %d\n", i);
		DR = bcm2835_gpio_lev(PIN_INTERRUPT);
		//printf("DR = %d\n", DR);
		//printf("fristDR = %d\n", firstDR);
		if((DR == 0) && firstDR)
		{
			firstDR = 0;
			readingBuf = SPIReadCont();
			for (j = 0; j < 2 ; j++)
			{
				if((readingBuf.value[j] & (uint32_t)1 << 23) != 0)
				{
					readingBuf.value[j] ^= 0xFF000000;
				}
			}			
			printf("%d,%d\n", (int32_t)readingBuf.value[0],(int32_t)readingBuf.value[1]);
			i--;
		}
		//printf("fristDR = %d\n", firstDR);
		if(DR && (firstDR == 0))
		{
			firstDR = 1;
		}		
	}
	bcm2835_gpio_write(PIN_CS, HIGH);
#endif
	bcm2835_spi_end();
	return 0;
}
