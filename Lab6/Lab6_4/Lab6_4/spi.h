#ifndef SPI_H_
#define SPI_H_
/*
 * spi.h
 *
 *  Last updated: 10/21/2019 3:17 PM
 *  Author: Dr. Schwartz
 */ 

#include <avr/io.h>

#define SCK			PIN7_bm
#define MISO		PIN6_bm
#define MOSI		PIN5_bm

#define SSA			PIN3_bm
#define SENSOR_SEL	PIN2_bm

/* initializes the SPI module of Port F to communicate with the LSM330 */
void spif_init(void);

/* writes a single byte of data to the SPIF data register */
void spif_write(uint8_t data);

/* attempts to read a byte of data from device connected to SPIF */
uint8_t spif_read(void);
#endif /* SPI_H_ */