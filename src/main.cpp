#include <Arduino.h>
// #include "M5Stack.h"
#include "M5CoreInk.h"
Ink_Sprite InkPageSprite(&M5.M5Ink);

void WriteLine(String line, int num)
{
  // M5.M5Ink.clear();
  Serial.println(line);
  InkPageSprite.drawString(35, (15 * num), line.c_str());
  InkPageSprite.pushSprite();
  delay(1000);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  M5.begin();
  M5.M5Ink.isInit();
  M5.M5Ink.clear();
  delay(1000);
  // creat ink refresh Sprite
  if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
  {
    Serial.printf("Ink Sprite creat failed");
  }
  else
  {
    Serial.printf("creatSprite success\n");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  WriteLine("Welcome", 1);
  WriteLine("To", 2);
  WriteLine("Logic", 3);
  WriteLine("We", 4);
  WriteLine("Build", 5);
  WriteLine("Cool", 6);
  WriteLine("Shit", 7);
  WriteLine("!!!", 8);
  delay(5000);
  M5.M5Ink.clear();
  if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
  {
    Serial.printf("Ink Sprite creat failed");
  }
  else
  {
    Serial.printf("creatSprite success\n");
  }
}