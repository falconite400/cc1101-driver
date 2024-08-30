/*
    CC1101 driver for the Raspberry Pi Pico using the C SDK

*/
/*
All transactions on the SPI interface start with
a header byte containing a R/W¯ bit, a burst
access bit (B), and a 6-bit address (A5 – A0).
*/
#include "cc1101.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdlib.h>


CC1101 *cc1101_init(char sck, char tx, char rx, char cs) {

    CC1101 *cc1101 = malloc(sizeof(CC1101));
    cc1101->spi_bus = spi0;
    spi_init(cc1101->spi_bus, 500000);
    
    // spi0 interface, 15 data bits per transfer, CPOL
    // polarity and CPHA phase both 0, must be SPI_MSB_FIRST
    spi_set_format(cc1101->spi_bus, 15, 0, 0, SPI_MSB_FIRST);

    // initialize the other SPI pins
    gpio_set_function(sck, GPIO_FUNC_SPI);
    gpio_set_function(tx, GPIO_FUNC_SPI);
    gpio_set_function(rx, GPIO_FUNC_SPI);

    // set the CS pin high
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);

    return cc1101;
}


void write_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t *data, bool consecutive) {
    uint8_t header_byte;
    /* Set the header byte. First bit is R/W (1/0), second is burst access
    (used to access consecutive registers more efficiently), next six are
    address bits
    */
    if (consecutive) {
        header_byte = 0b00000000 | reg_addr;
    } else {
        header_byte = 0b
    
    spi_write_blocking(cc1101->spi_bus, &reg_addr, 1);
    spi_write_blocking(cc1101->spi_bus, data, 1);

}

void read_reg(CC1101 *cc1101, const uint8_t *reg_addr, uint8_t *output, bool consecutive) {
    // write to the address, which is one byte in lengths
    spi_write_blocking(cc1101->spi_bus, reg_addr, 1);
    // reapeated_tx_data is 0
    spi_read_blocking(cc1101->spi_bus, 0, output, 1);
}

void get_part_num(CC1101 *cc1101, uint8_t *output) {
    const uint8_t addr = 0x30;
    read_reg(cc1101, &addr, output);
}