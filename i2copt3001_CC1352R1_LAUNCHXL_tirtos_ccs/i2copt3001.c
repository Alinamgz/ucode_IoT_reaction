/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


//=============================== Includes & Defines ===============================
#include "includes_n_defines.h"
#include "i2copt3001_config.h"


/***** Variables *****/
static Display_Handle display;

static I2C_Handle      i2cHandle;
static I2C_Params      i2cParams;

static OPT3001_Handle opt3001Handle;
static OPT3001_Params opt3001Params;

s32 uncomp_temperature   = 0;

float lux;
float comp_temperature;

/***** Functions *****/
extern s32 bme280_data_readout_template(I2C_Handle i2cHndl);
static inline void config_display(void);
static inline void config_I2C(void);
static inline void config_sensors(void);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0) {
    /* Call driver init functions */
    GPIO_init();
    I2C_init();
    OPT3001_init();


    config_display();
    config_I2C();
    config_sensors();

    Display_print0(display, 0, 0, "Ama ready \r\n");

    /* Begin infinite task loop */
    while(1) {

        /* Get Lux */
        if (!OPT3001_getLux(opt3001Handle, &lux)) {
            Display_print0(display, 0, 0, "OPT3001 sensor read failed");
        }

        Display_print1(display, 0, 0, "Lux: %f\n\n", lux);

        if (BME280_INIT_VALUE == bme280_read_uncomp_temperature(&uncomp_temperature)) {
            comp_temperature = (float)bme280_compensate_temperature_double(uncomp_temperature);
        }
        else {
            Display_print0(display, 0, 0, "Error reading from the bme280 sensor\n");
        }

//               sleep(1);
        sleep(SAMPLE_TIME);
    }
}

//==================================================================================
//================================ DISPLAY =========================================

static inline void config_display(void) {
    display = Display_open(Display_Type_UART, NULL);

    if (display == NULL) {
        while (1) {
            GPIO_toggle(CONFIG_GPIO_LED_Red);
            usleep(25000);
        }
    }
    else {
        Display_printf(display, 0, 0, "...\n\r");
    }
}

//================================== I2C ============================================
static inline void config_I2C(void) {
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cHandle = I2C_open(CONFIG_I2C_SENSORS, &i2cParams);

    if (i2cHandle == NULL) {
        Display_print0(display, 0, 0, "Error Initializing I2C\n");
        while (1) {
            GPIO_toggle(CONFIG_GPIO_LED_Red);
            usleep(25000);
        }
    }
    else {
        Display_print0(display, 0, 0, "I2C Initialized!\n");
    }
}

//================================ SENSORS ==========================================
static inline void config_sensors(void) {

    /* Initialize the OPT3001 Sensor */
    OPT3001_Params_init(&opt3001Params);
    opt3001Handle = OPT3001_open(CONFIG_OPT3001_LIGHT, i2cHandle, &opt3001Params);

    if(opt3001Handle == NULL) {
        Display_print0(display, 0, 0, "OPT3001 Open Failed!");
        while(1) {
            GPIO_toggle(CONFIG_GPIO_LED_Red);
            usleep(25000);
        }
    }

    sleep(1);

/* Initialize the BME Sensor */
    if(BME280_INIT_VALUE != bme280_data_readout_template(i2cHandle)) {
        Display_print0(display, 0, 0, "Error Initializing bme280\n");
    }
    bme280_set_power_mode(BME280_NORMAL_MODE);
}
