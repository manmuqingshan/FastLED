/// @file    VideoTest.ino
/// @brief   Demonstrates a video test using NoisePalette to generate content
/// @example VideoTest.ino

#include <FastLED.h>
#include "fx/2d/video.hpp"
#include "fx/2d/noisepalette.hpp"
#include "fx/fx_engine.h"
#include "ptr.h"

#define LED_PIN 2
#define BRIGHTNESS 96
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define MATRIX_WIDTH 22
#define MATRIX_HEIGHT 22
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

#define SCALE 20
#define SPEED 30

CRGB leds[NUM_LEDS];

VideoFxPtr videoFx;
NoisePalettePtr noisePalette;
FxEngine fxEngine(NUM_LEDS);

void setup() {
    delay(1000); // sanity delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    // Create and initialize XYMap
    XYMap xymap(MATRIX_WIDTH, MATRIX_HEIGHT);

    // Create and initialize NoisePalette object
    noisePalette = NoisePalettePtr::New(xymap);
    noisePalette->lazyInit();
    noisePalette->setSpeed(SPEED);
    noisePalette->setScale(SCALE);

    // Create and initialize VideoFx object
    videoFx = VideoFxPtr::New(xymap, noisePalette);
    
    // Add the video effect to the FxEngine
    fxEngine.addFx(videoFx);
}

void loop() {
    EVERY_N_MILLISECONDS(5000) { noisePalette->changeToRandomPalette(); }

    // Draw the frame
    fxEngine.draw(millis(), leds);

    // Debug output
    //std::cout << "First LED: R=" << (int)leds[0].r << " G=" << (int)leds[0].g << " B=" << (int)leds[0].b << std::endl;
    //std::cout << "Last LED: R=" << (int)leds[NUM_LEDS-1].r << " G=" << (int)leds[NUM_LEDS-1].g << " B=" << (int)leds[NUM_LEDS-1].b << std::endl;

    // Show the LEDs
    FastLED.show();

    FastLED.delay(1000 / 60); // 60 fps
}