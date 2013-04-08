// MAPPING of SPI CMDs

// Register addresses
#define ADDR_DATA_CH0		0x00
#define ADDR_DATA_CH1		0x03
#define ADDR_MOD		0x06
#define ADDR_PHASE		0x07
#define ADDR_GAIN 		0x08
#define ADDR_STATUS 		0x09
#define ADDR_CONFIG1		0x0A
#define ADDR_CONFIG2		0x0B

// Register configurations
#define	CFG_PHASE		0x00
// Boost for both channel active
#define CFG_GAIN		0x18
#define CFG_STATUS		0xB0
#define CFG_STATUS_DEBUG	0xB4
#define CFG_CONFIG1		0x0C
#define CFG_CONFIG1_SLOW	0xFC
#define CFG_CONFIG2		0X0C

// ADC reset command
#define CMD_RESET_ADCS		0xCC
