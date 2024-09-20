/*
    Copyright @falconite400
    CC1101 driver for the Raspberry Pi Pico using the C SDK

*/

#include "cc1101.h"


// FIFO addresses
#define SINGLE_BYTE_TX_FIFO 0x3f
#define BURST_ACCESS_TX_FIFO 0x7f
#define SINGLE_BYTE_RX_FIFO 0xbf
#define BUSRT_ACCESS_RX_FIFO 0xff

// command strobe addresses
#define RESET_CHIP 0x30
#define ENABLE_RX 0x34
#define ENABLE_TX 0x35

// status register addresses
#define PARTNUM_ADDR 0x30
#define VERSION_ADDR 0x31


CC1101 *cc1101_init(char sck, char tx, char rx, char cs, int num) {

    CC1101 *cc1101 = malloc(sizeof(CC1101));
    if (num == 0) {
        cc1101->spi_bus = spi0;
    } else if (num == 1) {
        cc1101->spi_bus = spi1;
    } else {
        return NULL;
    }

    spi_init(cc1101->spi_bus, 115200);
    
    // spi interface, 8 data bits per transfer, CPOL
    // polarity and CPHA phase both 0, must be SPI_MSB_FIRST
    spi_set_format(cc1101->spi_bus, 8, 0, 0, SPI_MSB_FIRST);

    // initialize the other SPI pins
    gpio_set_function(sck, GPIO_FUNC_SPI);
    gpio_set_function(tx, GPIO_FUNC_SPI);
    gpio_set_function(rx, GPIO_FUNC_SPI);

    // set the CS pin high
    cc1101->cs_pin = cs;
    gpio_init(cc1101->cs_pin);
    gpio_set_dir(cc1101->cs_pin, GPIO_OUT);
    gpio_put(cc1101->cs_pin, 1);

    return cc1101;
}


void write_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t **data, size_t data_size, bool burst) {

    // header byte consists of R/W bit, burst access bit, and 6-bit address
    uint8_t header_byte;


    
    // ensure that the first two bits of reg_addr are 0 but leave other bits alone
    reg_addr &= 0b00111111;

    // pull CS low
    gpio_put(cc1101->cs_pin, 0);

    if (burst) {
        // set burst address bit to 1 and combine with register address byte
        header_byte = 0b01000000 | reg_addr;

        // write header byte
        spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
        // write data in burst mode (register address automatically advances)
        for (int i = 0; i < data_size; i++) {
            printf("%d %c\n", i, *data[0]);
            spi_write_blocking(cc1101->spi_bus, data[i], 1);
        }
    } else {
        //second bit is zero to indicate single byte access instead of burst access
        header_byte = 0b00000000 | reg_addr;
        // write the header byte and singular data byte
        spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
        spi_write_blocking(cc1101->spi_bus, data[0], 1);
    }

    // pull chip select high
    gpio_put(cc1101->cs_pin, 1);
}

void read_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t **read_data, size_t read_size, bool burst) {

    uint8_t header_byte;

    // ensure that the first two bits of reg_addr are 0 but leave other bits alone
    reg_addr &= 0b00111111;

    // pull CS low
    gpio_put(cc1101->cs_pin, 0);

    if (burst) {
        // set read bit (1) and burst access bit (1), then combine with reg_addr
        header_byte = 0b11000000 | reg_addr;
        // write the header byte
        spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
        // read bytes and store them into the output array
        for (int i = 0; i < read_size; i++) {
            spi_read_blocking(cc1101->spi_bus, 0, *read_data, 1);
            read_data++;
        }

    } else {
        // set R/W to 1 (read) and burst access to 0, combine with reg_addr
        header_byte = 0b10000000 | reg_addr;
        // write the header byte
        spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
        // read a single byte to the output buffer
        spi_read_blocking(cc1101->spi_bus, 0, *read_data, 1);
    }

    // pull chip select high
    gpio_put(cc1101->cs_pin, 1);
}

void transmit(CC1101 *cc1101, int frequency, uint8_t data) {

}

void get_part_num(CC1101 *cc1101, uint8_t **output) {
    uint8_t addr = 0x30;
    read_reg(cc1101, addr, output, 1, 0);
}