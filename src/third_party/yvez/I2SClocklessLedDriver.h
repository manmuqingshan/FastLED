

#ifndef ESP32
#error "This library is for ESP32 only"
#endif

#include "esp32-hal.h"


// All of our FastLED specific hacks will go in here.
#ifndef USE_FASTLED
#define USE_FASTLED 1
#endif


// esp32 dev
#if CONFIG_IDF_TARGET_ESP32S3
#include "third_party/yvez/I2SClockLessLedDriveresp32s3/I2SClockLessLedDriveresp32s3.h"
#elif CONFIG_IDF_TARGET_ESP32
#include "third_party/yvez/I2SClocklessVirtualLedDriver/I2SClocklessLedDriver.h"
#else
#error "This library is for ESP32 or ESP32-S3 only"
#endif

