/* #####################################################################################################################

               """          main.c
        -\-    _|__
         |\___/  . \        Created on 30 Jul. 2024 at 23:35
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdint.h>
#include <stdio.h>

#define MISO 16
#define CS 17
#define SCLK 18
#define MOSI 19
#define INTR 20

#define SPI_PORT spi0

#define INT_NH 0b0001
#define INT_D 0b0100
#define INT_L 0b1000

#define G_INDORE 0b00100100  // [5:1]
#define G_OUTDORE 0b00011100 // [5:1]

void set_gain(bool indore) { // true for indore, false for outdor
    uint8_t data[2];
    data[0] = 0x40 | 0x00; // Asking to read 0x00
    gpio_put(CS, 0);       // begin writing command
    spi_write_blocking(SPI_PORT, data, 1);
    spi_read_blocking(SPI_PORT, 0, data + 1, 1);
    gpio_put(CS, 1);
    uint8_t res = 0b10000001 & data[1];
    data[0]     = 0x00 | 0x00; // asking to write to 0x00
    data[1]     = res | (indore ? G_INDORE : G_OUTDORE);
    gpio_put(CS, 0); // begin writing command
    spi_write_blocking(SPI_PORT, data, 2);
    gpio_put(CS, 1);
}

void read_event() {
    uint8_t data[2];
    data[0] = 0x40 | 0x03; // Asking to read 0x03
    printf("Flag 1.\n");
    gpio_put(CS, 0);       // begin writing command
    printf("Flag 2.\n");
    spi_write_blocking(SPI_PORT, data, 1);
    printf("Flag 3.\n");
    spi_read_blocking(SPI_PORT, 0, data + 1, 1);
    printf("Flag 4.\n");
    gpio_put(CS, 1);
    printf("Flag 5.\n");
    uint8_t res = 0x0f & data[1];
    printf("Flag 6.\n");
    if (res == INT_NH)
        printf("Noise detected.\n");
    if (res == INT_D)
        printf("Disturber detected.\n");
    if (res == INT_L)
        printf("Lightning detected !!\n");
    else
        printf("Other event...\n");
}

void gpio_callback(uint gpio, uint32_t events) {
    printf("Event detected.\n");
    sleep_ms(10);
    read_event();
}

int main() {
    stdio_init_all(); // Initialise I/O for USB Serial

    spi_init(SPI_PORT, 200000); // Initialise spi0 at 500kHz

    // Initialise GPIO pins for SPI communication
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);

    // Configure Chip Select
    gpio_init(CS);              // Initialise CS Pin
    gpio_set_dir(CS, GPIO_OUT); // Set CS as output
    gpio_put(CS, 1);            // Set CS High to indicate no currect SPI communication
    // Configure interrupt
    gpio_set_dir(INTR, GPIO_IN);
    /* gpio_pull_down(INTR); */

    gpio_set_irq_enabled_with_callback(INTR, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    uint8_t i = 0;
    printf("ayo\n");
    sleep_ms(5000);
    printf("Init done, running in loop\n");
    set_gain(true);
    uint8_t ping = 5;
    while (1) {
        sleep_ms(ping * 1000);
        printf("ping %d\n", i++);
        if (i > 9) {
            /* ping = 60; */
            i = 0;
        }
    }
    // Write Operation Example! Set oversampling and power on chip
    /* uint8_t data[2]; // Array to store data to be sent */
    /* data[0] = 0xF4 & 0x7F; // Remove first bit to indicate write operation */
    /* data[1] = 0x27; // Data to be sent */
    /* gpio_put(CS, 0); // Indicate beginning of communication */
    /* spi_write_blocking(SPI_PORT, data, 2); // Send data[] */
    /* gpio_put(CS, 1); // Signal end of communication */

    /* while(1){ */
    /*     reg = 0xFA | 0X80; */
    /*     gpio_put(CS, 0); */
    /*     spi_write_blocking(SPI_PORT, &reg, 1); */
    /*     spi_read_blocking(SPI_PORT, 0, buffer, 3); */
    /*     gpio_put(CS, 1); */
}
