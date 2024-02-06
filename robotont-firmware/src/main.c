#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "cmd.h"
#include "ioif.h"
#include "peripheral.h"
#include "system_hal.h"
#include "timerif.h"
#include "usbif.h"
#include "i2cif.h"


int main(void)
{
    system_hal_init();
    peripheral_init();
    ioif_init();
    timerif_init();
    i2cif_init();

    cmd_init();

    uint32_t counter = 0; // for debugging purposes
    uint32_t duty = 0;    // for debugging purposes

    uint32_t last_tick = HAL_GetTick();
    uint32_t current_tick;

    counter = 1;
    duty = 50;

    IoPinType led_green;
    led_green.pin_number = PIN_LED_G_Pin;
    led_green.ptr_port = PIN_LED_G_GPIO_Port;

    IoPinType led_red;
    led_red.pin_number = PIN_LED_R_Pin;
    led_red.ptr_port = PIN_LED_R_GPIO_Port;
    //ioif_togglePin(&led_green);
    ioif_writePin(&led_green, 0);
    ioif_writePin(&led_red, 0);

    MX_I2C1_Init();
    uint16_t slave_address = 0x12;
    //uint16_t mem_address = 0x0000;
    //uint16_t mem_size = 4; // Adjust based on your ATtiny88 configuration
    uint8_t data_buffer[4]; // Adjust the size based on mem_size

    while (true)
    {
        //i2cif_memoryRead(I2C_HANDLER_1, slave_address, mem_address, mem_size, data_buffer, sizeof(data_buffer), I2CIF_DEFAULT_TIMEOUT_MS);
        i2cif_masterRead(I2C_HANDLER_1, slave_address, data_buffer, sizeof(data_buffer), I2CIF_DEFAULT_TIMEOUT_MS);

        if (data_buffer[0] == 'X')
        {
            // If the received data matches 'X', toggle green LED
            ioif_writePin(&led_green, 1);
            ioif_writePin(&led_red, 0);
        }
        else
        {
            // If the received data doesn't match 'X', toggle red LED
            ioif_writePin(&led_green, 0);
            ioif_writePin(&led_red, 1);
        }

            printf("Hello!");
            system_hal_delay(5000);
        }
}
