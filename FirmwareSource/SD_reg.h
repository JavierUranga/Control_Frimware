#ifndef SD_reg_H
#define SD_reg_H

#include <WProgram.h>
#include <SD.h>
#include "Definitions.h"

#define chipSelect_SD 52 
#define FileName "Img_"

class SD_reg
{
public:  
  SD_reg();
  String  PrintRegisterSD_txt(void);
  void    PrintRegisterSD(void);
  bool    Save(uint8_t  *Imagen, byte RESOLUTION);
  void    CreateFolder (String path);
  String  GetpathFolder (void);
  int     GetImageNumber(void);
 
  
private:
  int NumImag;
  String m_path = "";
  File myFile;
  // set up variables using the SD utility library functions:
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
};

#endif
