/*
   _____ _   _ ______  _____     _____ _____ _____   
  / ____| \ | |  ____|/ ____|   |_   _/ ____|  __ \  
 | (___ |  \| | |__  | (___ ______| || |  __| |__) | 
  \___ \| . ` |  __|  \___ \______| || | |_ |  _  /  
  ____) | |\  | |____ ____) |    _| || |__| | | \ \  
 |_____/|_| \_|______|_____/    |_____\_____|_|  \_\ for Atmega 168P / 328P (nano)
                Vajskids Consoles 2022
                twitter: @vajskid
                Reset Combination: L + R + SEL 
 */

#define latch 3
#define clock 2
#define serialdata 19  // PC0 / A0
#define pressed (bitRead(PINC, 0) == 0)
#define rst bitWrite (PORTD, 4, 1), delay(500), bitWrite (PORTD, 4, 0)
#define reset 4
bool resetbuttonone = 1;
bool resetbuttontwo = 1;
bool resetbuttonthree = 1;

int buttonbitcounter = 0;


void setup() {

  attachInterrupt(digitalPinToInterrupt(latch), latchdelay, RISING);
  attachInterrupt(digitalPinToInterrupt(clock), igr, RISING);
  pinMode(latch, INPUT);
  pinMode(clock, INPUT);
  pinMode(serialdata, INPUT);
  pinMode (reset, OUTPUT);
  
  Serial.begin(57600);  // only for dev monitoring
}


void resetbools() {
resetbuttonone = 1, resetbuttontwo = 1, resetbuttonthree = 1;
}


void latchdelay() {
  if ((resetbuttonone == 0) && resetbuttontwo == 0 && resetbuttonthree == 0) rst, Serial.print ("RESET");
  resetbools();
  Serial.print ("\n");
  buttonbitcounter = 0;
}


void igr() {

  switch(pressed) {
  case true:
    Serial.print("0");
    if (buttonbitcounter == 1) resetbuttonone = 0;
    else if (buttonbitcounter == 6) resetbuttontwo = 0;
    else if (buttonbitcounter == 7) resetbuttonthree = 0;
    break;
  case false: 
    Serial.print("1");
    break;
}

buttonbitcounter++;

}


void loop() {

 delay (60);
    
}
