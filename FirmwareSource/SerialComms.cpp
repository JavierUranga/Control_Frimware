void SerialComms::Init ()
{
	this->TxCommand.Procesed = 1;
	this->RxCommand.Procesed = 1;
}

void SerialComms::Serial_ReadCommand ()
{
	 uint8_t data;

  while(Serial.available())
  {
      data = (uint8_t) Serial.read();

      //STX
      if (data == (uint8_t)(STX>>8)  && (this->STX_01_ctrl == false) && (this->STX_02_ctrl == false))
      {
    	  this->STX_01_ctrl = true;  this->STX_02_ctrl = false;
    	  this->ETX_01_ctrl = false; this->ETX_01_ctrl = false;
      }
      else if (data == (uint8_t)STX)
      {
    	  if ( (this->STX_01_ctrl == true) && (this->STX_02_ctrl == false) )
    	  {
    		  this->STX_01_ctrl = true;  this->STX_02_ctrl = true;
    		  this->ETX_01_ctrl = false; this->ETX_01_ctrl = false;
    		  this->Rxptn = 0;
    		  TO_RXBuffer((uint8_t)(STX>>8));
    		  TO_RXBuffer(data);
    	  }
    	  else
    	  {
    		  this->STX_01_ctrl = false; this->ETX_01_ctrl = false;
    	  }

      }

      //RTX
      else if (data == (uint8_t)(RTX>>8))
	   {
    	  if ( (this->STX_01_ctrl == true) && (this->STX_02_ctrl == true) && (this->ETX_01_ctrl == false) )
    	  	  {this->ETX_01_ctrl = true; this->ETX_02_ctrl = false;}
    	  TO_RXBuffer(data);

	   }
	   else if ( (data == (uint8_t)RTX))
	   {
		   if (this->ETX_01_ctrl == true)
		   {
			   this->STX_01_ctrl = false; this->STX_02_ctrl = false; this->ETX_01_ctrl = false;
			   TO_RXBuffer(data);
			   if (this->CheckRXBufferData()) this->RXBuffer_to_RXCommand();
			   else SERIAL_DEBUG("RXBuffer DATA ERROR!!!");
		   }
		   else TO_RXBuffer(data);
	   }

       //DATA...
	   else
	   {
		   if ( (this->STX_01_ctrl == true) && (this->STX_02_ctrl == true) && (this->ETX_01_ctrl == false) ) TO_RXBuffer(data);
	   }

  }

  return;
}

bool SerialComms::CheckRXBufferData(void)
{
	//SIZE
	if (this->Rxptn < 5) return false; // STX(2) + CMD(1) + ETX(2)
	//STX
	if (this->RXBuffer[0] <> (uint8_t)(STX>>8)) return false;
	if (this->RXBuffer[1] <> (uint8_t)STX) 		return false;
	//ETX
	if (this->RXBuffer[this->Rxptn -2] <> (uint8_t)(ETX>>8)) 	return false;
	if (this->RXBuffer[this->Rxptn -1] <> (uint8_t)ETX) 		return false;
	return true;
}

void SerialComms::RXBuffer_to_RXCommand(void)
{
	//COMMAND
	this->RxCommand.CMD = this->RXBuffer[CMD_POSITION];

	//ParamSize
	this->RxCommand.ParamSize = this->Rxptn -5;

	//DATA
	this->RxCommand.CMD = (uint8_t *)malloc(this->RxCommand.ParamSize);
	for (int ii=2; ii<(this->Rxptn -2); ii++)
	{
		this->RxCommand.CMD[ii-2] = this->RXBuffer[ii];
	}
	this->RxCommand.Procesed=0;
}
