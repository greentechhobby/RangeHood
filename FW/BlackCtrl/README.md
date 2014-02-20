Step by step How To use Ti Capsense:
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
