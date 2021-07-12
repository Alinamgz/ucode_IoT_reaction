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

float raw_lux;
float raw_temperature;

float processed_lux;
float processed_temperature;

//uint8_t cur_value;

enum tresholds{
    min_lux = 15,
    max_lux = 25,
    min_tmp = 30,
    max_tmp = 34
};

/***** Functions *****/
extern s32 bme280_data_readout_template(I2C_Handle i2cHndl);

static inline void say_error(char *where_err);
static inline void config_display(void);
static inline void config_I2C(void);
static inline void config_sensors(void);

static float moving_avg_filter(float raw_value, float *buf, uint8_t buf_len);
static void check_filtered_data(float cur_lux, float cur_tmp);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0) {

    float buf_lux_filter[BUF_LEN] = {0};
    float buf_tmp_filter[BUF_LEN] = {0};

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
        if (!OPT3001_getLux(opt3001Handle, &raw_lux)) {
            say_error("Reading from the OPT3001");
        }

        processed_lux = moving_avg_filter(raw_lux, buf_lux_filter, BUF_LEN);
        Display_printf(display, 0, 0, "\nRAW Lux: %f    \tPRC Lux: %f", raw_lux, processed_lux);

        if (BME280_INIT_VALUE == bme280_read_uncomp_temperature(&uncomp_temperature)) {
            raw_temperature = (float)bme280_compensate_temperature_double(uncomp_temperature);
        }
        else {
            say_error("Reading from the bme280");
        }

        processed_temperature = moving_avg_filter(raw_temperature, buf_tmp_filter, BUF_LEN);
        Display_printf(display, 0, 0, "RAW tmp: %f    \tPRC tmp: %f", raw_temperature, processed_temperature);

        check_filtered_data(processed_lux, processed_temperature);

        sleep(SAMPLE_TIME);
    }
}

//==================================================================================
//==================================================================================

//================================ ERRORS ==========================================
static inline void say_error(char *where_err) {
    Display_printf(display, 0, 0, "!!!\t%s ERR !!!\n\r");

    while (1) {
        GPIO_toggle(CONFIG_GPIO_LED_Red);
        usleep(25000);
    }
}


//================================ DISPLAY =========================================

static inline void config_display(void) {
    display = Display_open(Display_Type_UART, NULL);

    if (display == NULL) {
        while (1) {
            GPIO_toggle(CONFIG_GPIO_LED_Red);
            usleep(25000);
        }
    }
}

//================================== I2C ============================================
static inline void config_I2C(void) {
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cHandle = I2C_open(CONFIG_I2C_SENSORS, &i2cParams);

    if (i2cHandle == NULL) {
        say_error("Initializing I2C");
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
        say_error("OPT3001 Open");
    }

    sleep(1);

/* Initialize the BME Sensor */
    if(BME280_INIT_VALUE != bme280_data_readout_template(i2cHandle)) {
        say_error("Initializing bme280");
    }

    bme280_set_power_mode(BME280_NORMAL_MODE);
}

//==================================================================================
//==================================================================================
//================================ FILTER ==========================================
static float moving_avg_filter(float raw_value, float *buf, uint8_t buf_len) {
    uint8_t i = 0;
    float rslt_value = 0;

    buf[buf_len - 1] = raw_value;

    for (i = 0; i < buf_len; i++) {
        rslt_value += buf[i];
        buf[i] = (1 + i) < buf_len ? buf[1 + i] : buf[i];
    }

    rslt_value = rslt_value / buf_len;

    return rslt_value;
}

//======================== CHECK value & SWITCH LED ==================================
static void check_filtered_data(float cur_lux, float cur_tmp) {
//    if (cur_lux >= min_lux && cur_lux <= max_lux) {
//        GPIO_write(CONFIG_GPIO_LED_Green, CONFIG_GPIO_LED_ON);
//        Display_printf(display, 0, 0, "------    \tLux hobba!\t------\n\n\r");
//    }
//    else {
//        GPIO_write(CONFIG_GPIO_LED_Green, CONFIG_GPIO_LED_OFF);
//    }
//
//    if (cur_tmp <= max_tmp && cur_tmp >= min_tmp) {
//        Display_printf(display, 0, 0, "------\tTemperature hobba!\t------\n\n\r");
//        GPIO_write(CONFIG_GPIO_LED_Red, CONFIG_GPIO_LED_ON);
//    }
//    else {
//        GPIO_write(CONFIG_GPIO_LED_Red, CONFIG_GPIO_LED_OFF);
//    }
    if (cur_lux < min_lux) {
        GPIO_write(CONFIG_GPIO_LED_Green, CONFIG_GPIO_LED_ON);
        Display_printf(display, 0, 0, "------    \tLux hobba!\t------\n\n\r");
    }
    else if (cur_lux > max_lux) {
        GPIO_write(CONFIG_GPIO_LED_Green, CONFIG_GPIO_LED_OFF);
    }

    if (cur_tmp < min_tmp) {
        GPIO_write(CONFIG_GPIO_LED_Red, CONFIG_GPIO_LED_OFF);
    }
    else if (cur_tmp > max_tmp) {
        Display_printf(display, 0, 0, "------\tTemperature hobba!\t------\n\n\r");
        GPIO_write(CONFIG_GPIO_LED_Red, CONFIG_GPIO_LED_ON);
    }
}
