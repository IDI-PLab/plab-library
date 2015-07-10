#include "PLabLiquidCrystal.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).



PLabLiquidCrystal::PLabLiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
				 uint8_t i2caddr)
{
  init(rs, rw, enable, d0, d1, d2, d3, i2caddr);
}

PLabLiquidCrystal::PLabLiquidCrystal(uint8_t rs,  uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
				 uint8_t i2caddr)
{
  init(rs, 255, enable, d0, d1, d2, d3, i2caddr);
}

void PLabLiquidCrystal::init(uint8_t rs, uint8_t rw, uint8_t enable,
			 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			 uint8_t i2caddr)
{
  _rs_pin = rs;
  _rw_pin = rw;
  _enable_pin = enable;
  
  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3;

  _i2c_addr = i2caddr;
  
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
//  begin(16, 1);  
}

void PLabLiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {

  // Init port expander
  mcp.begin(_i2c_addr);

  mcp.pinMode(_rs_pin, OUTPUT);
  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
  if (_rw_pin != 255) { 
    mcp.pinMode(_rw_pin, OUTPUT);
  }
  mcp.pinMode(_enable_pin, OUTPUT);
  mcp.pinMode(_data_pins[0], OUTPUT);
  mcp.pinMode(_data_pins[1], OUTPUT);
  mcp.pinMode(_data_pins[2], OUTPUT);
  mcp.pinMode(_data_pins[3], OUTPUT);

  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delayMicroseconds(50000); 
  // Now we pull both RS and R/W low to begin commands
  mcp.digitalWrite(_rs_pin, LOW);
  mcp.digitalWrite(_enable_pin, LOW);
  if (_rw_pin != 255) { 
    mcp.digitalWrite(_rw_pin, LOW);
  }
  
  //put the LCD into 4 bitmode
  // this is according to the hitachi HD44780 datasheet
  // figure 24, pg 46

  // --- we start in 8bit mode, try to set 4 bit mode
  write4bits(0x03, LOW);
  delayMicroseconds(4500); // wait min 4.1ms

  // second try
  write4bits(0x03, LOW);
  delayMicroseconds(4500); // wait min 4.1ms
    
  // third go!
  write4bits(0x03, LOW); 
  delayMicroseconds(150);

  // finally, set to 4-bit interface
  write4bits(0x02, LOW); 
  // --- 4 bit mode set

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

/*
   in some 16x4 LCD when line 3 and 4 are not placed correctly you may try:
     setRowOffsets(0x00, 0x40, 0x14, 0x54)
   or
     setRowOffsets(0x00, 0x40, 0x10, 0x50)
 */
void PLabLiquidCrystal::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void PLabLiquidCrystal::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void PLabLiquidCrystal::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void PLabLiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void PLabLiquidCrystal::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PLabLiquidCrystal::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void PLabLiquidCrystal::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PLabLiquidCrystal::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void PLabLiquidCrystal::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PLabLiquidCrystal::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void PLabLiquidCrystal::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void PLabLiquidCrystal::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void PLabLiquidCrystal::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void PLabLiquidCrystal::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void PLabLiquidCrystal::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void PLabLiquidCrystal::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void PLabLiquidCrystal::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void PLabLiquidCrystal::command(uint8_t value) {
  send(value, LOW);
}

inline size_t PLabLiquidCrystal::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void PLabLiquidCrystal::send(uint8_t value, uint8_t mode) {
	/*
  mcp.digitalWrite(_rs_pin, mode);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) { 
    mcp.digitalWrite(_rw_pin, LOW);
  }
  */
  write4bits(value>>4, mode);
  write4bits(value, mode);
}

void PLabLiquidCrystal::write4bits(uint8_t value, uint8_t mode) {
  // To speed up i2c commands, we build the output manually
  uint8_t out = mcp.readGPIO();
  if (mode == HIGH) {
	  out |= 0x1 << _rs_pin;
  }
  else {
	  out &= ~(0x1 << _rs_pin);
  }

  if (_rw_pin != 255){
	  out &= ~(0x1 << _rw_pin);
  }

  for (int i = 0; i < 4; i++) {
	  // Zero out old value
	  out &= ~(1 << _data_pins[i]);
	  // Set new value
	  out |= ((value >> i) & 0x1) << _data_pins[i];
  }

  // Pulse enable
  uint8_t enableMask = 0x1 << _enable_pin;
  uint8_t outDisable = out & ~enableMask;
  uint8_t outEnable = out | enableMask;
  mcp.writeGPIO(outDisable);
  delayMicroseconds(2);
  mcp.writeGPIO(outEnable);
  delayMicroseconds(2);  // enable pulse must be >450ns
  mcp.writeGPIO(outDisable);
  delayMicroseconds(150);  // commands need > 37us to settle
}


// Exposing ports:
void PLabLiquidCrystal::pinMode(uint8_t p, uint8_t d) { mcp.pinMode(p, d); }
void PLabLiquidCrystal::digitalWrite(uint8_t p, uint8_t d) { mcp.digitalWrite(p, d); }
void PLabLiquidCrystal::pullUp(uint8_t p, uint8_t d) { mcp.pullUp(p, d); }
uint8_t PLabLiquidCrystal::digitalRead(uint8_t p) { return mcp.digitalRead(p); }