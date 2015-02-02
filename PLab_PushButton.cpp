#include "Arduino.h"
#include "PLab_PushButton.h"

#define DEBOUNCE 20  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

PLab_PushButton::PLab_PushButton(int pin)
{
  _pin = pin;
  _pressed = 0;
  justpressed = 0;
  justreleased = 0;
  previousstate = 0;
  currentstate = 0;
  lasttime = millis();
  pinMode(_pin, INPUT);
}

void PLab_PushButton::update()
{
  justreleased = 0;
  justpressed = 0; 
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  currentstate = digitalRead(_pin);   // read the button
    
    if (currentstate == previousstate) {
      if ((_pressed == LOW) && (currentstate == LOW)) {
          // just pressed
          justpressed = 1;
      }
      else if ((_pressed == HIGH) && (currentstate == HIGH)) {
          // just released
          justreleased = 1;
      }
      _pressed = !currentstate;  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate = currentstate;   // keep a running tally of the buttons
}


boolean PLab_PushButton::isDown()
{
  return _pressed;
}

boolean PLab_PushButton::pressed()
{
  return justpressed;
}

boolean PLab_PushButton::released()
{
  return justreleased;
}

