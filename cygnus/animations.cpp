#include "animations.h"
#include <math.h>

// --- Private global variables ---
static unsigned long animationStartTime[NUM_SEGMENTS];
static int sparkPixel[NUM_SEGMENTS];
static unsigned long sparkStartTime[NUM_SEGMENTS];
static unsigned long finalAnimationStartTime;

// Timer for the timed glitch sparks
static unsigned long nextSparkTime = 0;


// --- np_setup and trigger functions remain exactly the same ---
void np_setup() {
  FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(leds, NUM_PIXELS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    segmentState[i] = OFF;
    sparkPixel[i] = -1;
  }
}
void startPlacementAnimation(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= NUM_SEGMENTS) return;
    if (segmentState[segmentIndex] == PLACEMENT || segmentState[segmentIndex] == BREATHING) return;
    segmentState[segmentIndex] = PLACEMENT;
    animationStartTime[segmentIndex] = millis();
    sparkPixel[segmentIndex] = -1;
}
void startBreathingAnimation(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= NUM_SEGMENTS) return;
    segmentState[segmentIndex] = BREATHING;
    animationStartTime[segmentIndex] = millis();
}
void startFinalAnimation() {
    finalAnimationActive = true;
    finalAnimationStartTime = millis();
    // When the animation starts, schedule the first spark
    nextSparkTime = millis() + random(FINAL_ANIM_SPARK_MIN_DELAY, FINAL_ANIM_SPARK_MAX_DELAY);
}
void stopAnimation(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= NUM_SEGMENTS) return;
    segmentState[segmentIndex] = OFF;
}

void updateAnimations() {
  unsigned long currentTime = millis();

  if (finalAnimationActive) {
    // --- COMBINED FINAL ANIMATION: "ENERGY CORE" + "DIGITAL GLITCH" ---
    unsigned long finalElapsedTime = currentTime - finalAnimationStartTime;

    // --- Part 1: Draw the "Energy Core" background pulse ---
    uint8_t masterBrightness = 255;
    if (finalElapsedTime < FINAL_ANIM_FADE_IN_DURATION) {
      masterBrightness = map(finalElapsedTime, 0, FINAL_ANIM_FADE_IN_DURATION, 0, 255);
    }
    
    uint8_t wave = sin8(finalElapsedTime / FINAL_ANIM_PULSE_SPEED);
    float pulseRadius = map(wave, 0, 255, 0, PIXELS_PER_SEGMENT / 1.5);

    for (int i = 0; i < NUM_SEGMENTS; i++) {
      int startPixel = i * PIXELS_PER_SEGMENT;
      int endPixel = startPixel + PIXELS_PER_SEGMENT;
      float centerOfSegment = startPixel + (PIXELS_PER_SEGMENT / 2.0) - 0.5;

      for (int p = startPixel; p < endPixel; p++) {
        CRGB baseColor = CRGB(0, 0, 25); 
        if (random8() < FINAL_ANIM_FLICKER_CHANCE) {
          baseColor = CRGB(20, 50, 100);
        }
        
        float distFromCenter = abs(p - centerOfSegment);
        float pulseBrightnessFactor = max(0.0f, FINAL_ANIM_PULSE_WIDTH - abs(pulseRadius - distFromCenter));
        uint8_t pulseBrightness = pulseBrightnessFactor * (255.0 / FINAL_ANIM_PULSE_WIDTH);
        CRGB pulseColor = CRGB::Cyan;
        leds[p] = blend(baseColor, pulseColor, pulseBrightness);
        leds[p].nscale8(masterBrightness);
      }
    }
    
    // --- Part 2: Check timer and add a "Digital Glitch" spark on top ---
    if (currentTime >= nextSparkTime) {
      int sparkPos = random(NUM_PIXELS); // Pick a random pixel on the whole strip
      
      // Overwrite whatever color was there with a bright white spark for this one frame.
      leds[sparkPos] = CRGB::White; 

      // Schedule the next spark for a random time in the future.
      unsigned long randomDelay = random(FINAL_ANIM_SPARK_MIN_DELAY, FINAL_ANIM_SPARK_MAX_DELAY + 1);
      nextSparkTime = currentTime + randomDelay;
    }
    
  } else {
    // --- INDIVIDUAL SEGMENT ANIMATION LOGIC (UNCHANGED) ---
    for (int i = 0; i < NUM_SEGMENTS; i++) {
      int startPixel = i * PIXELS_PER_SEGMENT;
      int endPixel = startPixel + PIXELS_PER_SEGMENT;
      unsigned long elapsedTime = currentTime - animationStartTime[i];
      float brightnessFloat = 0.0;
      CRGB segmentColor = CRGB::Black;

      switch (segmentState[i]) {
        case PLACEMENT:
          {
            if (elapsedTime < 1000) brightnessFloat = 1.0;
            else if (elapsedTime < 1500) brightnessFloat = 1.0 - (float)(elapsedTime - 1000) / 500.0 * 0.8;
            else if (elapsedTime < 4500) brightnessFloat = 0.2 + (float)(elapsedTime - 1500) / 3000.0 * 0.8;
            else if (elapsedTime < PLACEMENT_DURATION) brightnessFloat = 1.0 - (float)(elapsedTime - 4500) / (PLACEMENT_DURATION - 4500);
            else startBreathingAnimation(i);
            segmentColor = CRGB(225, 200, 255);

            if (sparkPixel[i] != -1 && currentTime - sparkStartTime[i] > SPARK_DURATION) sparkPixel[i] = -1;
            if (random(SPARK_PROBABILITY) == 0 && sparkPixel[i] == -1) {
              sparkPixel[i] = startPixel + random(PIXELS_PER_SEGMENT);
              sparkStartTime[i] = currentTime;
            }
          }
          break;
        case BREATHING:
          {
            float breath = (sin(elapsedTime * 2.0 * PI / BREATHING_PERIOD) + 1.0) / 2.0;
            brightnessFloat = 0.1 + breath * 0.4;
            segmentColor = CRGB(0, 150, 225); 
          }
          break;
        case OFF:
          brightnessFloat = 0.0;
          segmentColor = CRGB::Black;
          break;
      }

      uint8_t brightness = brightnessFloat * 255;
      for (int p = startPixel; p < endPixel; p++) {
        leds[p] = segmentColor;
        leds[p].nscale8(brightness);
      }
      if (segmentState[i] == PLACEMENT && sparkPixel[i] != -1) {
        leds[sparkPixel[i]] = CRGB::White;
      }
    }
  }

  FastLED.show();
}