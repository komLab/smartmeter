#include "spi_mcp_rasp.h"
#include "mcp3901.h"
#include "iomapped_gpio.h"
#include <bcm2835.h>

int SPIinitialize(void)
{
	// spi mode 0 is set in wiringPiSPI.c
	// todo: change the setup function to pass over the spi mode
	if (!bcm2835_init())
	{
		printf("ERROR: SPI device is not initialized\n");
		return -1;
	}
	
	// Set the pins to be an output
	bcm2835_gpio_fsel(PIN_RESET, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_CS, BCM2835_GPIO_FSEL_OUTP);

	// set ~RESET Pin always high
	bcm2835_gpio_write(PIN_RESET, HIGH);

	// initalize ~CS PIN high
	bcm2835_gpio_write(PIN_CS, HIGH);

	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);    // The default 65536
	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);

	// setup iomapped interrupt pin
	setup_io();

	return 0;
}


void SPIWrite(char addr, char value)
{
	char spiData[2];
	// ensure to use only valid addresses
	addr = addr << 1;
	addr &= 0x3E;
	
	spiData[0] = addr;
	spiData[1] = value;
	bcm2835_gpio_write(PIN_CS, LOW);
	bcm2835_spi_transfern(spiData, sizeof(spiData));
	bcm2835_gpio_write(PIN_CS, HIGH);
}

void SPIWriteArray(char addr, char* value, char len)
{
	char spiData[len + 1];
	int i;

	// ensure to use only valid addresses
	addr = addr << 1;
	addr &= 0x3E;
	
	spiData[0] = addr;
	for(i = 1; i <= len; i++)
	{
		spiData[i]= value[i - 1];
		//printf("%02X\n", spiData[i]);
	}
	bcm2835_gpio_write(PIN_CS, LOW);
	bcm2835_spi_transfern(spiData, sizeof(spiData));
	bcm2835_gpio_write(PIN_CS, HIGH);
}

uint32_t SPIRead(char addr, char len)
{
	char spiData[len + 1];
	uint32_t result = 0;
	int i;

	// ensure to use only valid addresses
	// addr is now our command(8bit)
	addr = addr << 1;
	addr ^= 0x01;
	addr &= 0x3F;

	spiData[0] = addr;
	for(i = 1; i < len; i++)
	{
		spiData[i]= 0x00;
	}

	bcm2835_gpio_write(PIN_CS, LOW);
	bcm2835_spi_transfern(spiData, sizeof(spiData));
	bcm2835_gpio_write(PIN_CS, HIGH);
	for(i = 1; i <= len; i++)
	{
		result ^= (spiData[i] << ((len - i) * 8));
	}
	return result;
}

void SPIReadContInit(void)
{
	char spiData[7]= {1, 0, 0, 0, 0, 0, 0};

	bcm2835_gpio_write(PIN_CS, LOW);
	bcm2835_spi_transfern(spiData, sizeof(spiData));
}

struct reading SPIReadCont() //TODO! z.Z. nur ein Channle: ch0
{
	int i;
	struct reading result;
	char spiData[6] = {0, 0, 0, 0, 0, 0};	

	result.value[0] = 0;
	result.value[1] = 0;
	
	bcm2835_spi_transfern(spiData, sizeof(spiData));
	for(i = 0; i < 6; i++)
	{
		if ( i > 2)
		{
			result.value[1] ^= (spiData[i] << ((5 - i) * 8));
//			printf("%02X\n", spiData[i]);
		}
		else {
			result.value[0] ^= (spiData[i] << ((2 - i) * 8));
//			printf("%02X\n", spiData[i]);
		}
	}
	return result;
}

void setMCP3901Config(void)
{
	// define configuration sequence
	char config_seq[5] = {CFG_PHASE, CFG_GAIN, CFG_STATUS, CFG_CONFIG1_SLOW, CFG_CONFIG2};
	SPIWrite(ADDR_CONFIG2, CMD_RESET_ADCS);
	SPIWriteArray(ADDR_PHASE, config_seq, 5);
}

void printMCP3901Config()
{
	printf("MOD: \t\t%02X\n", SPIRead(ADDR_MOD, 1));	
	printf("PHASE: \t\t%02X\n", SPIRead(ADDR_PHASE, 1));
	printf("GAIN: \t\t%02X\n", SPIRead(ADDR_GAIN, 1));
	printf("STATUS: \t%02X\n", SPIRead(ADDR_STATUS, 1));
	printf("CONFIG1: \t%02X\n", SPIRead(ADDR_CONFIG1, 1));
	printf("CONFIG2: \t%02X\n", SPIRead(ADDR_CONFIG2, 1));
}
