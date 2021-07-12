#pragma once

//=============================== Includes ===============================
#include <stdint.h>
#include <stddef.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Module Header */
#include <ti/sail/opt3001/opt3001.h>
#include <ti/sail/bme280/bme280.h>

/* Driver configuration */
#include "ti_drivers_config.h"

//=============================== Defines ===============================
#define SAMPLE_TIME      1        /*In seconds*/
#define HIGH_LIMIT       25000.0F
#define LOW_LIMIT        100.0F

#define OPT_TASK_STACK_SIZE   768

#define CONFIG_OPT3001_LIGHT 0
#define CONFIG_OPT3001_COUNT 1

#define BUF_LEN 10
