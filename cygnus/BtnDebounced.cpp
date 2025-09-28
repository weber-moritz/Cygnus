#include "HardwareSerial.h"
#include "Arduino.h"       //nessesery for using pinmode etc.
#include "BtnDebounced.h"  //to reference the class

BtnDebounced::BtnDebounced(int pin, int debounceDelay) {  //constructor for the class
  pinMode(pin, INPUT);                                    //each time a object is created, the pin gets set to input with a pulldown resistor enabled.

  _pin = pin;
  _debounceDelay = debounceDelay;
}


int BtnDebounced::readBtn() {

  //read the currrent state of the pin
  int reading = digitalRead(_pin);

  int return_value = false;  //send the return or not (to make the return a one shot)


  //check if the state has changed and reset the timer
  if (reading != _lastButtonState) {
    // reset the debouncing timer
    _lastDebounceTime = millis();
  }

  //if the timer is up, check if the state has changed
  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // if the button state has changed:
    if (reading != _buttonState) {
      //reset the button state to not trigger again
      _buttonState = reading;

      //return true and print!
      if (_buttonState == HIGH) {
#if DEBUG
        Serial.println("btn = HIGH");
#endif
        return_value = true;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  _lastButtonState = reading;

  //sending a "1" before resetting return value to 0. this is to only trigger the btn once, not multiple times.
  return return_value;
}

int BtnDebounced::readBtnHold() {

  //read the currrent state of the pin
  int reading = digitalRead(_pin);

  //check if the state has changed and reset the timer
  if (reading != _lastButtonState) {
    // reset the debouncing timer
    _lastDebounceTime = millis();
  }

  //if the timer is up, check if the state has changed
  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // if the button state has changed:
    if (reading != _buttonState) {
      //reset the button state to not trigger again
      _buttonState = reading;

#if DEBUG
      Serial.print("readBtnHold ");
      Serial.print(_pin);
      Serial.println(reading);
#endif
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  _lastButtonState = reading;

  //sending a "1" before resetting return value to 0. this is to only trigger the btn once, not multiple times.
  return (_buttonState == HIGH);
}