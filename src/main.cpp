#include <Arduino.h>
#include <M5Stack.h>

void WriteLine(String line)
{
  Serial.println(line);
  M5.Lcd.print(line); 
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  M5.Lcd.begin();  //Initialize M5Stack
  M5.Power.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WriteLine("Welcome");
  WriteLine("To");
  WriteLine("Logic");
  WriteLine("We");
  WriteLine("Build");
  WriteLine("Cool");
  WriteLine("Shit");
  delay(5000);
}