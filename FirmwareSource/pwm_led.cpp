#include "pwm_led.h"

  pwm_LED::pwm_LED()
{
	//Dimming LED
	pinMode(LED_PIN, OUTPUT); 
	this->SetBrightness(0);
}

void pwm_LED::SetBrightness(int Brightness_ratio)
{
	if (Brightness_ratio<=0) this->m_brightness_ratio = 0;
	else if (Brightness_ratio>=100) this->m_brightness_ratio = 100;
	else this->m_brightness_ratio = Brightness_ratio;
	
	this->m_brightness = (int) ((this->m_brightness_ratio/100)*255);
	analogWrite(LED_PIN, this->m_brightness);
}


int  pwm_LED::GetBrightness(void)
{
	return this->m_brightness_ratio;
}
