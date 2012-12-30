/*
Copyright (c) 2012, "David Hilton" <dhiltonp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#include <hexbright.h>

#include <Wire.h>

// number of milliseconds between updates
#define OFF_MODE 0
#define BLINKY_MODE 1
#define CYCLE_MODE 2

int mode = 0;


#define MS 10
hexbright hb(MS);

void setup() {
  hb.init_hardware();
} 

void loop() {
  hb.update();
  static int brightness_level = 1;

  //// Button actions to recognize, one-time actions to take as a result
  if(hb.button_released()) {
    if(hb.button_held()<1) {
      // ignore, could be a bounce
    } else if(hb.button_held()<300) { //<300 milliseconds
      mode = CYCLE_MODE;
      int levels[] = {1,500,1000};
      brightness_level = (brightness_level+1)%3;
      hb.set_light(CURRENT_LEVEL, levels[brightness_level], 100);
    } else if (hb.button_held() < 700) {
      mode = BLINKY_MODE;
    }
  }
  if(hb.button_held()>700) { // if held for over 700 milliseconds (whether or not it's been released), go to OFF mode
    mode = OFF_MODE;
    // in case we are under usb power, reset state
    hb.set_light(0, 0, 1);
    brightness_level = 4;
  }
}
