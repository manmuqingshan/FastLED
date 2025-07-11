/// @file    Overclock.ino
/// @brief   High performance LED display example
/// @example Overclock.ino
///
/// This sketch is fully compatible with the FastLED web compiler. To use it do the following:
/// 1. Install Fastled: `pip install fastled`
/// 2. cd into this examples page.
/// 3. Run the FastLED web compiler at root: `fastled`
/// 4. When the compiler is done a web page will open.

/// @brief   Demonstrates how to overclock a FastLED setup

#include <FastLED.h>
#if !SKETCH_HAS_LOTS_OF_MEMORY
// To effectively test the overclock feature we need
// a large enough dataset to test against. Unfortunately
// the avr platforms don't have enough memory so this example
// is disabled for these platforms
void setup() {}
void loop() {}
#else


#define FASTLED_OVERCLOCK 1.1 // Overclocks by 10%, I've seen 25% work fine.

#include "fx/2d/noisepalette.h"
#include "fx/fx.h"


using namespace fl;

#define LED_PIN 3
#define BRIGHTNESS 96
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define MATRIX_WIDTH 22
#define MATRIX_HEIGHT 22
#define GRID_SERPENTINE 1

#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

// This example combines two features of FastLED to produce a remarkable range
// of effects from a relatively small amount of code.  This example combines
// FastLED's color palette lookup functions with FastLED's Perlin noise
// generator, and the combination is extremely powerful.
//
// You might want to look at the "ColorPalette" and "Noise" examples separately
// if this example code seems daunting.
//
//
// The basic setup here is that for each frame, we generate a new array of
// 'noise' data, and then map it onto the LED matrix through a color palette.
//
// Periodically, the color palette is changed, and new noise-generation
// parameters are chosen at the same time.  In this example, specific
// noise-generation values have been selected to match the given color palettes;
// some are faster, or slower, or larger, or smaller than others, but there's no
// reason these parameters can't be freely mixed-and-matched.
//
// In addition, this example includes some fast automatic 'data smoothing' at
// lower noise speeds to help produce smoother animations in those cases.
//
// The FastLED built-in color palettes (Forest, Clouds, Lava, Ocean, Party) are
// used, as well as some 'hand-defined' ones, and some proceedurally generated
// palettes.

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.
// The higher the value of scale, the more "zoomed out" the noise iwll be.  A
// value of 1 will be so zoomed in, you'll mostly see solid colors.
#define SCALE 20

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward. Try
// 1 for a very slow moving effect, or 60 for something that ends up looking
// like water.
#define SPEED 30

CRGB leds[NUM_LEDS];
XYMap xyMap(MATRIX_WIDTH, MATRIX_HEIGHT, GRID_SERPENTINE);
NoisePalette noisePalette(xyMap);


void setup() {
    delay(1000); // sanity delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip).setScreenMap(xyMap);
    FastLED.setBrightness(96);
    noisePalette.setSpeed(SPEED);
    noisePalette.setScale(SCALE);
}

void loop() {
    EVERY_N_MILLISECONDS(5000) { noisePalette.changeToRandomPalette(); }
    noisePalette.draw(Fx::DrawContext(millis(), leds));
    FastLED.show();
}

#endif
