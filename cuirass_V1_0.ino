/*Code for Home brewed EXPERIMENTAL!!!! Cuirass ventilator for Arduino Atmega 328P 16Mhz (Arduino Nano etc) 
 by: github/jps2000 March 23, 2020
 The software controls a solid state relay (SSR) on / off in the rhythm of the respiratory rate. The relay switches the mains of a vacuum cleaner (VC)
 The relay  is AC coupled to avoid fatal error in case the controller hangs. This is achieved by  controlling the relay via a PWM and a coupling capacitor (Elcap 10uF + to controller) and a clamping diode (1N4148 parallel SSR input kathode to +). So static  H of the controller does not switch the relay off.
 I / E time is 25%
 Respiratory rate can be set within limits by UP / DOWN buttons
 The vacuum cleaner should allow control of vacuum strenght electrically or by bypass air. 
 As cuirass a transparent plastic box having a size of about 40x30x15cm with rounded opening is recommended. (Like IKEA Samla 39x28x14)
 Connect the VC hose to it. Use a soft towel between the box and the chest. However fit should be improved!
 Fixation on the chest is done by 2 velcros  to achieve little pressure.
 When the VC is on, The chest and belly is sucked in the box and cause inspiration or at least increase functional residual capacity.

 Details of this kind ov ventilation is outlined here:
 www.hayekmedical.com

 Disclaimer:
 Freeware:  The code is provided "AS IS",without warranty of any kind. License CC0 for desgins and GPL 3 for code
 The author can not be made legible for any harm caused by this device and its application.
 !!!!!!!!!!!! Do not use without supervision and medical advise.!!!!!!!!!!!!!!
 Working with Mains voltage without experience may cause fatal outcome.
 The author declares no relation or any other contacts to hayekmedical.
 The VC should have an air bypass to adjust suction strength.
  
*/
#define btn_UP              (9)         // tact switch to GND
#define btn_DOWN            (12)        // tact switch to GND
#define PWM_OUT             (6)         // connect to solid state relay via 10uF elcap. + to controller; 1N4148 Diode antiparallel to  relay input
                                        // valid pins Nano: 5,6 (960Hz)
#define LED                 (13)        // control led for relay (Arduino Nano onboard LED)
#define lower_limit         4500        // time in ms
#define upper_limit         10500       // time in ms
#define STEP                500         // step width to increment or decrement time per button press

float   I_E                 = 1.0/4.0;  // I/E ratio = 1:3 = 1 / 1+3 depends on behavior of VC
unsigned long timer         = millis();
int RP                      = 7500;                        // respiratory period 8/min

void setup() 
{
 // pin definitions
  pinMode(LED, OUTPUT);
  pinMode(PWM_OUT, OUTPUT);
  pinMode(btn_DOWN, INPUT_PULLUP);
  pinMode(btn_UP, INPUT_PULLUP); 
}

void loop() 
{
  if(millis() - timer > RP) timer = millis();
  
  if(millis() - timer  <  RP * (1 - I_E))                       //on
  {
  analogWrite(PWM_OUT,0);                             
  digitalWrite(LED, LOW);
  }
  else
  {  
  analogWrite(PWM_OUT,200);                                 //off
  digitalWrite(LED, HIGH);
  }

  //tact switch handling
  if(digitalRead(btn_DOWN) == LOW ){                        
    delay(100);
    while(digitalRead(btn_DOWN) == LOW);                  // wait for button release
    if (RP > lower_limit) RP -= STEP;
    }
  if(digitalRead(btn_UP) == LOW ){                        // button pressed  stores actual phi as mean value and saves actual calibration to flash
    delay(100);
    while(digitalRead(btn_UP) == LOW);                    // wait for button release
    //action comes here
    if (RP < upper_limit) RP += STEP;
    }
}
