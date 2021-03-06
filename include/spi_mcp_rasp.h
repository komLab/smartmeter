#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include "cbuffer.h"

// 0 -> pin gpio0 is defined in wirinPi pintable
#define PIN_RESET RPI_GPIO_P1_11
// has to be changed to use iomapped gpio pin
#define PIN_INTERRUPT 18
// 2 -> pin gpio3 is defined in wirinPi pintable
#define PIN_CS RPI_GPIO_P1_15

int SPIinitialize(void);

void SPIWrite(char, char);

void SPIWriteArray(char, char*, char);

uint32_t SPIRead(char, char);

void setMCP3901Config(void);

void SPIReadContInit(void);

reading SPIReadCont(void);

void printMCP3901Config(void);
