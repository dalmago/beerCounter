#include <seg7.h>

int segs[] = {6, 5, 13, 9, 8, 7, 10, 11};
int digits[] = {12, 4, 3, A4};

Seg7 bebado(segs, digits);

bool lastState;

void setup() { 
  // push buttons
  DDRC &= ~(0b00001111); // set as input
  PORTC |= (0b00001111); // internal pull-up
  lastState = 0; // used to detect edge
  
  
  //bebado.setValue(0); // initial value
  //bebado.setMuxTime(3); // optional, default time=5
}

void loop() {
  if ((PINC & 0b00001111) != 0b00001111){
    bebado.show(); // debounce
    if ((PINC & 0b00001111) != 0b00001111){
      if (lastState == 0){
        lastState = 1;

        if ((PINC & 0b00000001) ^ 0b00000001)  // Litro
          bebado.setValue(bebado.getValue()+1);
        else if ((PINC & 0b00000010) ^ 0b00000010) // Long neck
          bebado.setValue(bebado.getValue()+0.355);
        else if ((PINC & 0b00000100) ^ 0b00000100) // Latao
          bebado.setValue(bebado.getValue()+0.473);
        else if ((PINC & 0b00001000) ^ 0b00001000)// Latinha
          bebado.setValue(bebado.getValue()+0.350);     
      }
    }
  } else{
    bebado.show(); 
    if ((PINC & 0b00001111) == 0b00001111){
      lastState = 0;
    }
  }
}
