#ifndef PWM_LED_H
#define PWM_LED_H

#include <WProgram.h>

#define LED_PIN 10    //OC9

class pwm_LED
{
public:  
  pwm_LED();
  void SetBrightness(int Brightness_ratio);	//From 0% to 100%
	int GetBrightness(void);
  
  private:
	int m_brightness;			//from 0 to 255
	int m_brightness_ratio;		//From 0% to 100%
  
};

#endif
