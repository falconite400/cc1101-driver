/*
    Copyright @falconite400
    Example code for the cc1101 driver
*/

#include "cc1101.h"

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    CC1101 *cc1101_0 = cc1101_init(2, 3, 4, 5, 0);
    CC1101 *cc1101_1 = cc1101_init(18, 19, 16, 17, 1);

    if (cc1101_0 == NULL) {
        printf("Failed to allocate memory for cc1101_0\n");
        return -1;
    } else if (cc1101_1 == NULL) {
        printf("Failed to allocate memory for cc1101_1\n");
        return -1;
    }

    uint8_t *cc1101_part_num[1];

    while (true) {

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        get_part_num(cc1101_0, cc1101_part_num);
        printf("Part num is %u\n", **cc1101_part_num);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}