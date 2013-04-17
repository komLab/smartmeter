#include <stdio.h>
#include "spi_mcp_rasp.h"
#include "mcp3901.h"
#include <bcm2835.h>

int main(int argc, char* argv[])
{
#ifdef BURST
	int i;
	uint32_t readingBuf;
#endif
	if (SPIinitialize())
	{
		printf("ERROR: SPI device is not initialized\n");
		return -1;
	}
#ifdef CONFIG
	setMCP3901Config();	
//	printMCP3901Config();
#endif
#ifdef READCONFIG
	printMCP3901Config();
#endif
#ifdef BURST
	bcm2835_gpio_afen(PIN_INTERRUPT);

	i = 1000;
	while(i > 0)
	{
		if(bcm2835_gpio_eds(PIN_INTERRUPT))
		{
			bcm2835_gpio_set_eds(PIN_INTERRUPT);
			readingBuf = SPIReadCont();
			if((readingBuf & (uint32_t)1 << 23) != 0)
			{
				readingBuf ^= 0xFF000000;
			}
			printf("%d\n", (int32_t)readingBuf);
			i--;
		}
	}
#endif
	bcm2835_spi_end();
	return 0;
}
