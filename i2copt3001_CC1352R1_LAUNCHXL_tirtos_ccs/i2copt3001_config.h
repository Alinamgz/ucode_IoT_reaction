#pragma once

/* Please check <ti/sail/opt3001/opt3001.h> file to know more about OPT3001_HWAttrs and OPT3001_Config structures */
OPT3001_Object OPT3001_object[CONFIG_OPT3001_COUNT];

const OPT3001_HWAttrs OPT3001_hwAttrs[CONFIG_OPT3001_COUNT] = {
    {
#ifdef CONFIG_I2C_OPT_BOOSTXL_SENSORS_OPT3001_ADDR // BOOSTXL-SENSORS
        .slaveAddress = OPT3001_SA4,
#else // BOOSTXL-BASSENSORS
        .slaveAddress = OPT3001_SA4,
#endif
        .gpioIndex = CONFIG_GPIO_OPT3001_INT,
    },
};

const OPT3001_Config OPT3001_config[] = {
    {
        .hwAttrs = &OPT3001_hwAttrs[0],
        .object  = &OPT3001_object[0],
    },
    {NULL, NULL},
};
