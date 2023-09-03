/*
   _____ _   _ ______  _____     _____ _____ _____   
  / ____| \ | |  ____|/ ____|   |_   _/ ____|  __ \  
 | (___ |  \| | |__  | (___ ______| || |  __| |__) | 
  \___ \| . ` |  __|  \___ \______| || | |_ |  _  /  
  ____) | |\  | |____ ____) |    _| || |__| | | \ \  
 |_____/|_| \_|______|_____/    |_____\_____|_|  \_\ for Atmega 168P / 328P (nano) V2.00
                Vajskids Consoles 2023
                twitter: @vajskid

                Normal Reset: L + R + SEL + START
                Enable lockout: L + R + START + RIGHT
                Disable lockout: L + R + START + LEFT
                NTSC Mode (PAL60): L + R + START + DOWN
                PAL MODE (50hz): L + R + START + UP

 */
#include <EEPROM.h>
#define latch 3
#define clock 2
#define CIC 6             // to pin 4 of lockout chip (float it),  guides advise on 2k2 series resistor on this output
#define modeToggle 5      // Pin 30 of PPU 2 and Pin 24 of PPU1 (float them or cut traces), guides advise on 2k2 series resistor on this output
#define serialdata 19  // PC0 / A0
#define pressed (bitRead(PINC, 0) == 0)
#define rst bitWrite (PORTD, 4, 1), delay(00), bitWrite (PORTD, 4, 0)
#define reset 4

int buttonbitcounter = 0;
uint16_t capturedCombo =  0x0000;
bool lockout = 0;


void setup() {

  attachInterrupt(digitalPinToInterrupt(latch), latchdelay, CHANGE);
  attachInterrupt(digitalPinToInterrupt(clock), igr, CHANGE);
  pinMode(latch, INPUT);
  pinMode(clock, INPUT);
  pinMode(serialdata, INPUT);
  pinMode (reset, OUTPUT);
  pinMode (CIC, OUTPUT);
  pinMode (modeToggle, OUTPUT);
  bitSet (PORTD, 5);  // initial boot mode PAL 50hz
  bitClear (PORTD, 4); // Reset Low
  Serial.begin(57600);  // only for dev monitoring

    //***************************************************************************************************************************************************************
  if (EEPROM.read(0) == 0x80 || EEPROM.read(0) == 0x40) {     // 0x80 lockout enabled, 0x40 lockout disabled
    goto notfirstboot;                                        // If mode is set, jump to 'notfirstboot', don't waste write cycles on the EEPROMs finite life.
  }
  else {
    EEPROM.write(0, 0x80);                                    // First ever use, enable lock-out chip by writing 0x80 to flash, must be disabled in a game that doesn't require the lock out chip, so you can then relaunch the region locked game
  }
notfirstboot:
  //*****************************************************************************************************************************************************************

// set lockout chip to enable or disable setting stored in flash
if (EEPROM.read(0) == 0x80){
  bitSet(PORTD, 6);
  }
else if (EEPROM.read(0) == 0x40){
  bitClear(PORTD, 6);
  } 

}

void switchVideoHigh(){
  if (!bitRead (PORTD, 5)) {
  bitSet (PORTD, 5);
  }
}

void switchVideoLow(){
  if bitRead (PORTD, 5){
  bitClear (PORTD, 5);
  }
}

void lockoutChipEnable(){
  if (!bitRead (PORTD, 6)) {
  bitSet (PORTD, 6);
  EEPROM.write(0, 0x80);  
  rst; //help user validate mode change with a reset
  }
}

void lockoutChipDisable(){
  if bitRead (PORTD, 6){
  bitClear (PORTD, 6);
  EEPROM.write(0, 0x40);  
  rst; //help user validate mode change with a reset
  }
}

void latchdelay() {
  // F3F8 = reset, F3E3 reset and change video mode

  Serial.print(capturedCombo, HEX);
  Serial.print ("\n");
  buttonbitcounter = 0;

  if (capturedCombo == 0xF3E3) {          // L + R + START + UP
    switchVideoHigh();
      delay(1000);
  }
  if (capturedCombo == 0xF3F8) {          // L + R + START + SELECT 
    rst;
    delay(1000);
  }
  else if (capturedCombo == 0xF3DB) {
  switchVideoLow();
  delay(1000);
}
  else if (capturedCombo == 0xF3BB) {     // L + R + START + LEFT
  lockoutChipEnable();
  delay(1000);
}
  else if (capturedCombo == 0xF37B) {     // L + R + START + RIGHT
  lockoutChipDisable();
  delay(1000);
  }
capturedCombo =  0x0000;
}

void igr() {

  if(pressed) {
    bitClear(capturedCombo, buttonbitcounter);
  }
  else bitSet(capturedCombo, buttonbitcounter);
  
  buttonbitcounter++;
}

void loop() {

}
