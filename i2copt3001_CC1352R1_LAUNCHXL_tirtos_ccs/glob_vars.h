#pragma once

//=============================================================
// -------------------------- Handles -------------------------
//=============================================================
Display_Handle display_handle;
OPT3001_Handle opt3001_handle;
I2C_Handle      i2c_handle;

//=============================================================
// -------------------------- Params --------------------------
//=============================================================
OPT3001_Params opt3001_params;
I2C_Params      i2c_params;

//=============================================================
// ---------------------- for recvd data ----------------------
//=============================================================
s32 g_s32_actual_temp;
float lux;

//=============================================================
// ----------------------- OPT3001 setup-----------------------
//=============================================================
//sem_t opt3001Sem;

/* Please check <ti/sail/opt3001/opt3001.h> file to know more about OPT3001_HWAttrs and OPT3001_Config structures */
OPT3001_Object OPT3001_object[CONFIG_OPT3001_COUNT];

const OPT3001_HWAttrs OPT3001_hwAttrs[CONFIG_OPT3001_COUNT] = {
    {
#ifdef CONFIG_I2C_OPT_BOOSTXL_SENSORS_OPT3001_ADDR // BOOSTXL-SENSORS
        .slaveAddress = OPT3001_SA4,
#else // BOOSTXL-BASSENSORS
//        .slaveAddress = OPT3001_SA1,
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


//=============================================================
// -----------------------              -----------------------
//=============================================================
