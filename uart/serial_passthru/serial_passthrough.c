#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define BAUD_RATE 115200
#define UART_ESP32 uart0
#define UART_ESP32_TX_PIN 0
#define UART_ESP32_RX_PIN 1

int main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    const uint ESP32_CS    = 13;
    const uint ESP32_RST   = 16;
    const uint ESP32_GPIO0 = 9;

    // Status LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // init STDIO over USB-CDC
    stdio_usb_init();

    // init UART0
    uart_init(UART_ESP32, BAUD_RATE);
    gpio_set_function(UART_ESP32_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_ESP32_RX_PIN, GPIO_FUNC_UART);
    sleep_ms(100);
    gpio_put(LED_PIN, 0);


    gpio_init(ESP32_CS);
    gpio_set_dir(ESP32_CS, GPIO_OUT);
    gpio_init(ESP32_RST);
    gpio_set_dir(ESP32_RST, GPIO_OUT);
    gpio_init(ESP32_GPIO0);
    gpio_set_dir(ESP32_GPIO0, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // manually put the ESP32 in upload mode
    gpio_put(ESP32_GPIO0, 0);

    gpio_put(ESP32_RST, 0);
    sleep_ms(100);

    gpio_put(ESP32_RST, 1);
    sleep_ms(100);
    gpio_put(LED_PIN, 0);

    //uart_puts(UART_ESP32, "starting bridge...\n");

    while (true) {
         // read USB, write UART
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            if (uart_is_writable(UART_ESP32)) {
                uart_putc_raw(UART_ESP32, c);
            }
        }

        // Read UART, write USB
        if (uart_is_readable(UART_ESP32)) {
            gpio_put(LED_PIN, 1);
            uint8_t ch = uart_getc(UART_ESP32); // read from UART
            putchar(ch); // write to USB
            gpio_put(LED_PIN, 0);
        }
    }

    return 0;
}