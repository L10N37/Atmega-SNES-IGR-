# Atmega-SNES-IGR
In Game RESET for SNES / Super Famicom using arduino 168P / 328P

## Reset combination L + R + SEL (instant reset) Developed on a cheap clone!

##  6 wires 

  - 5v to VIN (grabbed it off live side of reset)- 
  - D4 to Low/ idle side of reset
  - D3 to controller latch
  - D2 to controller clock
  - A0 (19/PC0) to Controller serial.
  - Gnd to Ground

 # How's it work?

The app counts clock pulses during latch pulses.
On each clock pulse during a latch pulse the app 
checks the state of the serial line. The serial 
line idles high and is pulled low with a button press.
Each clock pulse during a latch pulse corresponds to
a different button being pressed.

Between latch pulses the app checks which clock
pulses had the serial line pulled low during that 
clock pulse. 

It's not 100%. I was going to use Start as part 
of the combo but adding any code drops that bit being
ccaptured. There could be a a few early buttons 
missing but it wasn't worth coding around. It took a few 
hours to write.
