/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, GTSV Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of GTSV Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/


We have rewritten the original Ti Capsense Libraray to reduce code side. 
Step by step How To re-use our library:(TICAPS.c and TICAPS.h)
============

1. Define all the single channel key obj:
   ```cpp
      TICAPS_Sckey_Obj gTimer_key = {
         .inputPxselRegister = (uint8_t *)&P3SEL,
         .inputPxsel2Register = (uint8_t *)&P3SEL2,
         .inputBits = BIT0,
         
         .threshold = TIMER_THRESHOLD,
      };
  
      TICAPS_Sckey_Obj gLight_key = {
         .inputPxselRegister = (uint8_t *)&P2SEL,
         .inputPxsel2Register = (uint8_t *)&P2SEL2,
         .inputBits = BIT0,
         
         .threshold = LIGHT_THRESHOLD,
      };

      TICAPS_Sckey_Obj gPlus_key = {
         .inputPxselRegister = (uint8_t *)&P1SEL,
         .inputPxsel2Register = (uint8_t *)&P1SEL2,
         .inputBits = BIT5,
         
         .threshold = PLUS_THRESHOLD,
      };
  ```
  
2. Define an array of pointers to all sckey obj with NULL terminators:
```cpp
      TICAPS_Sckey_Obj* gSckeys[] = {
         &gTimer_key,
         &gLight_key,
         &gMinus_key,
         &gPlus_key,
         &gAuto_key,
         NULL   // terminator
      };
```

3.. call the init method:
```cpp
      TICAPS_sckey_init(gSckeys);
```

4.. Periodically call the run method in a loop or ISR:
```cpp
       if(gMain.initialized == 1){
         TICAPS_sckey_run(gSckeys);
       }
```

5.. Check if any buttons is detected then debounce or do anything:
```cpp
      if(gTimer_key.detected){
         LED_TIMER_TOGGLE;
         bt_detected = CTRL_Buttons_Timer;
      }
      if(gLight_key.detected){
         LED_LAMP_TOGGLE;
         bt_detected = CTRL_Buttons_Lamp;
      }
      if(gPlus_key.detected){
         LED_PLUSMINUS_TOGGLE;
         bt_detected = CTRL_Buttons_Plus;
      }
```
Contact me for more info: jacksmtv at gmail dot com
