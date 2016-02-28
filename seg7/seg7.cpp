/*
 * Seg7.cpp
 *
 * Created: 2/28/2016 11:38:42 AM
 *  Author: Matheus Dal Mago
 */

#define COMMON_ANODE 0
#define COMMON_CATHODE 1
#define DECIMAL_DIGITS 2

// common cathode
char pins[10][7] = {{1, 1, 1, 1, 1, 1, 0},
				 {0, 1, 1, 0, 0, 0, 0},
				 {1, 1, 0, 1, 1, 0, 1},
				 {1, 1, 1, 1, 0, 0, 1},
				 {0, 1, 1, 0, 0, 1, 1},
				 {1, 0, 1, 1, 0, 1, 1},
				 {1, 0, 1, 1, 1, 1, 1},
				 {1, 1, 1, 0, 0, 0, 0},
				 {1, 1, 1, 1, 1, 1, 1},
				 {1, 1, 1, 1, 0, 1, 1}};

#include "seg7.h"
#include <Arduino.h>

Seg7::Seg7(int segments[], int digits[]){    
	for (int i=0; i<8; i++){
		segs[i] = segments[i];
		
		pinMode(segs[i], OUTPUT);
		
		// all segments on
		#if COMMON_ANODE
		digitalWrite(segments[i], 0);
		#elif COMMON_CATHODE
		digitalWrite(segments[i], 1);
		#endif 
	}
    
    // turn dot off
    #if COMMON_ANODE
    digitalWrite(segments[7], 1);
    #elif COMMON_CATHODE
    digitalWrite(segments[7], 0);
    #endif
	
	for (int i=0; i<4; i++){
		(*this).digits[i] = digits[i];
        
        pinMode(digits[i], OUTPUT);
		
		// all digits on
        #if COMMON_ANODE
        digitalWrite(digits[i], 1);
        #elif COMMON_CATHODE
        digitalWrite(digits[i], 0);
        #endif
	}
	
    (*this).setMuxTime(5);
	(*this).setValue(0);
}

void Seg7::setValue(float v){
    if (v >= pow(10, 4-DECIMAL_DIGITS))
        value = v-pow(10, 4-DECIMAL_DIGITS);
    else
        value = v;
    
    dig3 = ((int)(v*pow(10, DECIMAL_DIGITS)))%10;
    dig2 = ((int)(v*pow(10, DECIMAL_DIGITS-1)))%10;
    dig1 = ((int)(v*pow(10, DECIMAL_DIGITS-2)))%10;
    dig0 = ((int)(v*pow(10, DECIMAL_DIGITS-3)))%10;
    
}

float Seg7::getValue(){
    return (value);
}

void Seg7::setMuxTime(char time){
    muxTime = time;
}

void Seg7::printValue(){
    Serial.begin(19200);
    Serial.print("value: "); Serial.println(value);
    Serial.end();
}

void Seg7::printPins(){
    Serial.begin(19200);
    Serial.print("segment pins: ");
    for (int i=0; i<8; i++){
        Serial.print((int)segs[i]);
        Serial.print(' ');
    }
    Serial.println();
    Serial.print("digits pins: ");
    for (int i=0; i<4; i++){
        Serial.print((int)digits[i]);
        Serial.print(' ');
    }
    Serial.println();
    Serial.end();
}

void Seg7::show(){
	char dig;
	for (int i=0; i<4; i++){
		switch(i){
			case 0:
				dig = dig0;
				break;
			case 1:
				dig = dig1;
				break;
			case 2:
				dig = dig2;
				break;
			case 3:
				dig = dig3;
				break;
			default:
				dig = 0;
		}
        
		for (int j=0; j<7; j++){
			#if COMMON_ANODE
			digitalWrite(segs[j], ! pins[dig][j]);
			#elif COMMON_CATHODE
			digitalWrite(segs[j], pins[dig][j]);
			#endif
		}
        
        // turn on dot
        #if COMMON_ANODE
        digitalWrite(segs[7], (i==(3-DECIMAL_DIGITS) ? 0 : 1));
        #elif COMMON_CATHODE
        digitalWrite(segs[7], (i==(3-DECIMAL_DIGITS) ? 1 : 0));
        #endif
		
		// turn digit on
		#if COMMON_ANODE
		digitalWrite(digits[i], 1);
		#elif COMMON_CATHODE
		digitalWrite(digits[i], 0);
		#endif
		
		delay(muxTime);
		
		// turn digit off
		#if COMMON_ANODE
		digitalWrite(digits[i], 0);
		#elif COMMON_CATHODE
		digitalWrite(digits[i], 1);
		#endif
	}
}