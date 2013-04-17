#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

// 0 -> pin gpio0 is defined in wirinPi pintable
#define PIN_RESET RPI_GPIO_P1_11
// 1 -> pin gpio1 is defined in wirinPi pintable
#define PIN_INTERRUPT RPI_GPIO_P1_12
// 2 -> pin gpio3 is defined in wirinPi pintable
#define PIN_CS RPI_GPIO_P1_15

struct reading{ uint32_t value[2]; };

int SPIinitialize(void);

void SPIWrite(char, char);

void SPIWriteArray(char, char*, char);

uint32_t SPIRead(char, char);

void setMCP3901Config(void);

void SPIReadContInit(void);

uint32_t SPIReadCont(void);

void printMCP3901Config(void);
