#define DEBUG 1


#include "BtnDebounced.h"

const int debounce_time = 100;
BtnDebounced btn1(6, debounce_time);
BtnDebounced btn2(7, debounce_time);
BtnDebounced btn3(8, debounce_time);
BtnDebounced btn4(9, debounce_time);


#include <FastLED.h>
#include "animations.h"

// DEFINE the FastLED array
CRGB leds[NUM_PIXELS];

// DEFINE your state-tracking arrays and variables
AnimationState segmentState[NUM_SEGMENTS];
bool prevHoldState[NUM_SEGMENTS] = {false, false, false, false}; // <-- THIS WAS THE MISSING LINE
bool finalAnimationActive = false;
bool finalAnimationPending = false;
unsigned long finalAnimationStartTimeTrigger = 0;



void setup() {
  Serial.begin(115200);

  dfpm_setup(10);  //volume 10-30
  np_setup();
}


void loop() {

  bool currentHoldState[NUM_SEGMENTS];
  currentHoldState[0] = btn1.readBtnHold();
  currentHoldState[1] = btn2.readBtnHold();
  currentHoldState[2] = btn3.readBtnHold();
  currentHoldState[3] = btn4.readBtnHold();


  int ballsCurrentlyPresent = 0;

  // --- SINGLE BALLS ---
  // Now, loop through our saved data to find what has changed since the last loop.
  for (int i = 0; i < NUM_SEGMENTS; i++) {

    // Condition 1: A ball was JUST placed.
    // (The button is pressed now, but it wasn't on the last loop cycle).
    if (currentHoldState[i] && !prevHoldState[i]) {
      dfpm_play_track(i + 1);

      startPlacementAnimation(i);
      Serial.print("BTN pressed: ");
      Serial.println(i + 1);
    }

    // Condition 2: A ball was JUST removed.
    // (The button is not pressed now, but it was on the last loop cycle).
    if (!currentHoldState[i] && prevHoldState[i]) {
      stopAnimation(i);
    }

    // Count how many balls are present right now.
    if (currentHoldState[i]) {
      ballsCurrentlyPresent++;
    }
  }


  // --- ALL BALLS ---
  if (ballsCurrentlyPresent == NUM_SEGMENTS) {
    // This block only runs ONCE when the 4th ball is first placed.
    if (!finalAnimationActive && !finalAnimationPending) {
      Serial.println("All balls there");
      finalAnimationPending = true;
      finalAnimationStartTimeTrigger = millis() + PLACEMENT_DURATION;
    }
  } else {  // If fewer than 4 balls are present...
    if (finalAnimationActive || finalAnimationPending) {
      finalAnimationActive = false;
      finalAnimationPending = false;

      // Restore the correct state for any remaining balls using our saved data.
      for (int i = 0; i < NUM_SEGMENTS; i++) {
        if (currentHoldState[i]) {
          startBreathingAnimation(i);
        } else {
          stopAnimation(i);
        }
      }
    }
  }


  //play pending animation
  if (finalAnimationPending && millis() >= finalAnimationStartTimeTrigger) {
    finalAnimationPending = false;
    dfpm_play_track(5);

    startFinalAnimation();
    Serial.println("Playing final anim");
  }


  updateAnimations();

  //store state for next loop to detect changes
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    prevHoldState[i] = currentHoldState[i];
  }
}
