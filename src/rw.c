#include <stdio.h>
#include <bcm2835.h>
#include "spi_mcp_rasp.h"
#include "mcp3901.h"
#include "iomapped_gpio.h"

int main(int argc, char* argv[])
{
	float norm = 856.2f/(1<<23);

	int j;
	reading readingBuf;
	cbuffer my_buffer;

	if(alloc_buffer(&my_buffer, BUFFER_SIZE) == 1)
		return EXIT_FAILURE;

	if(SPIinitialize())
		return EXIT_FAILURE;

	setMCP3901Config();	
	SPIReadContInit();
	while(buffer_is_full(&my_buffer) != 1)
	{
		// change 0x80 to MASK for current interrupt pin
		// e.g. 0x80 for gpio pin 7
		while((GPIO_GET & 0x040000)>0)
			;

		readingBuf = SPIReadCont();
		for (j = 0; j < 2 ; j++)
		{
			if((readingBuf.value[j] & (uint32_t)1 << 23) != 0)
				readingBuf.value[j] ^= 0xFF000000;
		}			
		add_buffer(&my_buffer, &readingBuf);
	}

	bcm2835_gpio_write(PIN_CS, HIGH);
	bcm2835_spi_end();

	while(buffer_is_empty(&my_buffer) != 1) {
		printf("%f,%f\n", ((int32_t)readingBuf.value[0])*norm,((int32_t)readingBuf.value[1])*norm);
		pop_buffer(&my_buffer, &readingBuf);
	}

	if(free_buffer(&my_buffer) == 1)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
