#include <EEPROM.h>
#include <seg7.h>

#define EEPROM_BYTE_VALIDATOR 250

float value;

int segs[] = {6, 5, A5, 9, 8, 7, 10, 11};
int digits[] = {12, 4, 3, A4};

Seg7 bebado(segs, digits);

bool lastState;

void setup() {
  if (EEPROM.read(0) == EEPROM_BYTE_VALIDATOR) {
    EEPROM.get(1, value);
  } else {
    EEPROM.write(0, EEPROM_BYTE_VALIDATOR);
    value = 0;
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

  // push buttons
  DDRC &= ~(0b00001111); // set as input
  PORTC |= (0b00001111); // internal pull-up
  lastState = 0; // used to detect edge


  bebado.setValue(value); // initial value
  //bebado.setMuxTime(3); // optional, default time=5
}

void loop() {
  if ((PINC & 0b00001111) != 0b00001111) {
    bebado.show(); // debounce
    if ((PINC & 0b00001111) != 0b00001111) {
      if (lastState == 0) {
        lastState = 1;

        if ((PINC & 0b00001111) == 0b00000110) { // litro and latinha
          while (1) {
            bebado.show();
            if ((PINC & 0b00001111) == 0b00001111) { // no button pressed
              bebado.show();
              if ((PINC & 0b00001111) == 0b00001111) { // no button pressed
                while (1) {
                  digitalWrite(13, 1);
                  bebado.show();
                  if ((PINC & 0b00001111) == 0b00001001) { // long and latao
                    bebado.setValue(0);
                    EEPROM.put(1, 0.0);
                    value = 0;
                    break;
                  } else if ((PINC & 0b00001111) == 0b00000110) { // undo
                    bebado.setValue(value);
                    break;
                  }
                }
                digitalWrite(13, 0);
                break;
              }
            }
          }
        }
        else if ((PINC & 0b00000001) ^ 0b00000001) { // Litro
          bebado.setValue(value += 1);
          EEPROM.put(1, value);
        } else if ((PINC & 0b00000010) ^ 0b00000010) { // Long neck
          bebado.setValue(value += 0.355);
          EEPROM.put(1, value);
        } else if ((PINC & 0b00000100) ^ 0b00000100) { // Latao
          bebado.setValue(value += 0.473);
          EEPROM.put(1, value);
        } else if ((PINC & 0b00001000) ^ 0b00001000) { // Latinha
          bebado.setValue(value += 0.350);
          EEPROM.put(1, value);
        }
      }
    }
  } else {
    bebado.show();
    if ((PINC & 0b00001111) == 0b00001111) {
      lastState = 0;
    }
  }
}

