#include <stdio.h>
#include "spi_mcp_rasp.h"
#include "mcp3901.h"
#include "iomapped_gpio.h"
#include <bcm2835.h>

int main(int argc, char* argv[])
{
	//float norm = 500.0f/(2^23);

#ifdef BURST
	int j;
	struct reading readingBuf;
#endif
	if (SPIinitialize())
	{
		printf("ERROR: SPI device is not initialized\n");
		return -1;
	}
#ifdef CONFIG
	setMCP3901Config();	
#endif
#ifdef BURST
	SPIReadContInit();
	while(1)
	{
		// change 0x80 to MASK for current interrupt pin
		// e.g. 0x80 for gpio pin 7
		while((GPIO_GET & 0x040000)>0)
			;

		readingBuf = SPIReadCont();
		for (j = 0; j < 2 ; j++)
		{
			if((readingBuf.value[j] & (uint32_t)1 << 23) != 0)
			{
				readingBuf.value[j] ^= 0xFF000000;
			}
		}			
		printf("%d,%d\n", (int32_t)readingBuf.value[0],(int32_t)readingBuf.value[1]);
	}
	bcm2835_gpio_write(PIN_CS, HIGH);
#endif
	bcm2835_spi_end();
	return 0;
}
