#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

#include <WProgram.h>
#include "definitions.h"

#define TO_RXBuffer(DATA) if (this->Rxptn < sizeof(RxBuffer))
						  	  {this->RxBuffer[this->Rxptn++] = (DATA);}
						  else
							  {this->STX_01_ctrl = false; this->STX_02_ctrl = false;}

typedef struct{
	bool 	Procesed;
	uint8_t	CMD;
	void* 	PARAM;
	int		ParamSize
}COMMAND;

class SerialComms
{
public:
	SerialComms();
	void Init();

	//SEND
	bool 	SendCommand(); 										// SET Procesed to TRUE
	void	Set_TXCommand(uint8_t CMD, void* PARAM, int Size);	// SET Procesed to FALSE

private:
	COMMAND 	TxCommand;
	COMMAND 	RxCommand;
	uint8_t		RxBuffer[100];
	int			Rxptn;
	bool		STX_01_ctrl, STX_02_ctrl;
	bool		ETX_01_ctrl;

	void Serial_ReadData (uint8_t *buffer);
	bool CheckRXBufferData(void);
	void RXBuffer_to_RXCommand(void);
};

#endif
