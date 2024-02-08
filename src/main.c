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
    ioif_writePin(&led_green, 1);
    ioif_writePin(&led_red, 0);

    MX_I2C1_Init();
    uint8_t slave_address = 8;
    //uint16_t mem_address = 0x0000;
    //uint16_t mem_size = 4; // Adjust based on your ATtiny88 configuration
    uint8_t data_buffer[1]; // Adjust the size based on mem_size
    HAL_StatusTypeDef status;
    while (true)
    {
        //i2cif_memoryRead(I2C_HANDLER_1, slave_address, mem_address, mem_size, data_buffer, sizeof(data_buffer), I2CIF_DEFAULT_TIMEOUT_MS);
        //i2cif_masterRead(I2C_HANDLER_1, slave_address, data_buffer, sizeof(data_buffer), I2CIF_DEFAULT_TIMEOUT_MS);
        //HAL_I2C_Master_Transmit(I2C_HANDLER_1, slave_address, data_buffer, sizeof(data_buffer), I2CIF_DEFAULT_TIMEOUT_MS);
        uint8_t data;
        status = HAL_I2C_Master_Transmit(&hi2c1, 0x08 << 1, NULL, 0, HAL_MAX_DELAY); // Send address of ATtiny88
        printf("Requesti status %d \n", status);
        
        HAL_I2C_Master_Receive(&hi2c1, 0x08 << 1, &data, 1, HAL_MAX_DELAY); // Receive data from ATtiny88
        printf("Receive status %d \n", status);

        if (data == 0x55)
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

            //printf("data: %d, %d, %d, %d \n", data_buffer[0], data_buffer[1], data_buffer[2], data_buffer[3]);
            printf("data, %d \n" , data);
            system_hal_delay(100);
        }
}
