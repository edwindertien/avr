#include <Arduino.h>
/***********************************************************************
 * RT-AVR board source, ATmega32 @ 8 MHz (also 16MHz versions)
 * Hardware: 
 *   - 8 analog in
 *   - 8 digital out, open collector with ULN2803
 *   - 2 motor output with L293
 *   - LCD display on PORT C
 *
 * Also uses Arduino's standard LiquidCrystal Library
 * The circuit:
 * LCD RS pin to digital pin 16
 * LCD Enable pin to digital pin 18
 * LCD D4 pin to digital pin 20
 * LCD D5 pin to digital pin 21
 * LCD D6 pin to digital pin 22
 * LCD D7 pin to digital pin 23
 * LCD R/W pin to digital pin 17
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
// Usefull utility functions to work with ports outside the standard arduino functions
// outb(), inb(), inw(), outw(), BV(), sbi(), cbi()
#define	outb(addr, data)	addr = (data)
#define	inb(addr)			(addr)
#define	outw(addr, data)	addr = (data)
#define	inw(addr)			(addr)
#define BV(bit)			(1<<(bit))
#define cbi(reg,bit)	reg &= ~(BV(bit))
#define sbi(reg,bit)	reg |= (BV(bit))
#define limit(number,boundary) ( ((number)>(boundary))?(boundary):( ((number)<(-boundary))?(-boundary):(number)) )
#define limit2(number,min,max) ( ((number)>(max))?(max):( ((number)<(min))?(min):(number)) )
#define toggle(port,nr) if(port & 1<<nr) cbi(port,nr); else sbi(port,nr)
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(16, 18, 20, 21, 22, 23);
void vu(int data);

void setup() {
  // set up the LCD's number of columns and rows: 
  pinMode(17,OUTPUT); // R/W for the display
  for(int n=4; n<16; n++){
    pinMode(n,OUTPUT);
  }
  digitalWrite(A5,HIGH); // pullup for button
  digitalWrite(A6,HIGH);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RT-AVR board");
  Serial.begin(9600);
  Serial.println("RT-AVR board");
}
void loop() {
  int value = analogRead(7)/4;
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  if(value<10){lcd.print("  ");}
  else if(value<100){lcd.print(" ");}
  lcd.print(value);
  vu(value);
  analogWrite(4,value);
  analogWrite(5,value);
  analogWrite(7,value);
  if(analogRead(5)<100){
    lcd.setCursor(8,1);
    lcd.print("left");
  }
  else if (analogRead(6)<100){
    lcd.setCursor(8,1);
    lcd.print("right");
  }
  else{
    lcd.setCursor(8,1);
    lcd.print("     ");
  }    
  delay(100);
  Serial.println(value);
}
void vu(int data){
if (data>223) outb(PORTB,255);
else if (data>191) outb(PORTB,127);
else if (data>159) outb(PORTB,63);
else if (data>127) outb(PORTB,31);
else if (data>95) outb(PORTB,15);
else if (data>63) outb(PORTB,7);
else if (data>31) outb(PORTB,3);
else if (data>0) outb(PORTB,1);
else outb(PORTB,0);
}