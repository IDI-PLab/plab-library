/*
 * MultiFileExample
 *
 * Show how multiple files can be used in a single Arduino project.
 * This file contain setup and loop.
 * ExtraFile contain sayHi, which is called from this file.
 *
 * No circuit is requred. The program prints to Serial output.
 */
void setup() {
  Serial.begin(9600);
}

void loop() {
  sayHi(); // sayHi() is defined in ExtraFile.ino
}
