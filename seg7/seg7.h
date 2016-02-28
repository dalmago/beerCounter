/*
 * seg7.h
 *
 * Created: 2/28/2016 10:57:15 AM
 *  Author: Matheus Dal Mago
 */


#ifndef SEG7_H_
#define SEG7_H_

class Seg7{
private:
    char dig0, dig1, dig2, dig3; // from left to right
    char segs[8]; // A-G, dot at the end
    char digits[4];
    float value;
    char muxTime;
    
public:
    
    // first vector: segments A to G and DOT
    // second vector: control pins, from left to right
    Seg7(int[], int[]);
    
    void setValue(float=0);
    float getValue();
    void setMuxTime(char);
    
    void printValue();
    void printPins();
    
    // must be called periodically
    void show();
};

#endif /* SEG7_H_ */