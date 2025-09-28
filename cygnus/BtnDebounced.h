#ifndef BtnDebounced_h
#define BtnDebounced_h

class BtnDebounced {
public:
  BtnDebounced(int pin, int debounceDelay);  //constructor
  int readBtn();                             // initiating the read_btn function
  int readBtnHold();

private:
  int _pin;                           //pin of the btn
  unsigned long _debounceDelay = 30;  //the time for the debounce, how long the sate unknown is

  int _buttonState;            //current state
  int _lastButtonState = LOW;  //last stat

  unsigned long _lastDebounceTime = 0;  //last time the btn was debounced
};

#endif