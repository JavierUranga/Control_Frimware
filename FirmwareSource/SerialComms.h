#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

#include <WProgram.h>
#include "definitions.h"



class SerialComms
{
public:
	SerialComms();
	void SetBrightness(int Brightness_ratio);
	int GetBrightness(void);

 private:
	int m_brightness;
	int m_brightness_ratio;

};

#endif
