/*
  PushButton.h - Library for Button with cebounce.
  Created by Dag Svanæs, 2015.
  Released into the public domain.
*/

#ifndef PLab_PushButton_h
#define PLab_PushButton_h

#include "Arduino.h"


#define DEBOUNCE 20  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

class PLab_PushButton
{
  public:
    PLab_PushButton(int pin);
    void update();
    boolean isDown();
    boolean pressed(); 
    boolean released();     
  private:
    int _pin;
    byte _pressed, justpressed, justreleased;
    byte previousstate;
    byte currentstate;
    long lasttime;
};

#endif