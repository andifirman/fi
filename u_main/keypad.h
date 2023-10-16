#include "Wire.h"
#include "I2CKeyPad.h"
#include "PCF8574.h"


PCF8574 pcf8574(0x20);
const uint8_t KEYPAD_ADDRESS = 0x20;

I2CKeyPad keyPad(KEYPAD_ADDRESS);
char keymap[19] = "DCBA#9630852*741NF";  // N = NoKey, F = Fail

String inputString, pad;
long inputInt;
char ch, choicesKey;
