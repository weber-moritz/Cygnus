#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <FastLED.h>

// --- SHARED CONSTANTS ---
#define PIXEL_PIN 2
#define NUM_PIXELS 12
#define NUM_SEGMENTS 4
const int PIXELS_PER_SEGMENT = NUM_PIXELS / NUM_SEGMENTS;

// --- Animation timing constants ---
const int PLACEMENT_DURATION = 8500;
const int BREATHING_PERIOD = 4000;
const int SPARK_DURATION = 50;
const int SPARK_PROBABILITY = 25;

// --- Constants for the "Energy Core" background of the final animation ---
const int FINAL_ANIM_FADE_IN_DURATION = 3000;
const int FINAL_ANIM_PULSE_SPEED = 20;
const float FINAL_ANIM_PULSE_WIDTH = 1.5;
const int FINAL_ANIM_FLICKER_CHANCE = 40;

// --- Constants for the "Digital Glitch" sparks on top of the final animation ---
const int FINAL_ANIM_SPARK_MIN_DELAY = 333;  // Minimum time between sparks (~3/sec)
const int FINAL_ANIM_SPARK_MAX_DELAY = 1000; // Maximum time between sparks (1/sec)

// --- The rest of your header file remains the same ---
extern CRGB leds[]; 
extern bool finalAnimationActive;
extern bool finalAnimationPending;
extern unsigned long finalAnimationStartTimeTrigger;

enum AnimationState {
  OFF,
  PLACEMENT,
  BREATHING
};

extern AnimationState segmentState[];

void np_setup();
void updateAnimations();
void startPlacementAnimation(int segmentIndex);
void startBreathingAnimation(int segmentIndex);
void startFinalAnimation();
void stopAnimation(int segmentIndex);

#endif