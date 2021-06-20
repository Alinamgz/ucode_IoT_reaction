#pragma once

/*    ======== i2copt3001.c ======== */

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
