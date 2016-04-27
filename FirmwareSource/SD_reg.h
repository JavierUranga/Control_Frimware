#ifndef SD_reg_H
#define SD_reg_H

#include <WProgram.h>
#include <SD.h>
#include "Definitions.h"

#define chipSelect_SD 52 


class SD_reg
{
public:  
  SD_reg();
  void PrintRegisterSD(void);
  void Save(uint8_t  *Imagen, byte RESOLUTION); //Habría que enviar también el path
  void CreateFolder (String path);
  String GetpathFolder (void);
  File myFile;
  // set up variables using the SD utility library functions:
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
  private:
  String FileName = "Img_";
  byte RESOLUTION;
  int NumImag = 6;
  String m_path = "";

  
};

#endif
