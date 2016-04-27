#include "SD_reg.h"

  SD_reg::SD_reg()
{
	pinMode(chipSelect_SD, OUTPUT);
  digitalWrite(chipSelect_SD, HIGH);
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  SD.begin(chipSelect_SD);
}

void SD_reg::PrintRegisterSD(void)
{
    if (!card.init(SPI_HALF_SPEED, chipSelect_SD))
   {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else
  {
   Serial.println("Wiring is correct and a card is present."); 
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  double volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  //root.ls(LS_R | LS_SIZE);
 // sum(root.ls(LS_SIZE));
}

void SD_reg::Save(uint8_t  *Imagen, byte RESOLUTION)
{
  String ini_path = "";
  ini_path = this->m_path + this->FileName + this->NumImag;
//  ini_path = this->m_path;
//  ini_path.concat(this->FileName);
//  Serial.println(ini_path);
//  ini_path.concat(this->NumImag);
  Serial.println(ini_path);
  //NomImag.concat(".jpg");    // Para guardarla como jpg hay que ponerle un header.
  char NombreImagen[ini_path.length()+1];
  ini_path.toCharArray(NombreImagen, sizeof(NombreImagen));
  Serial.println(NombreImagen);
  if (SD.exists(NombreImagen)) 
  {
    Serial.println("Borrando archivo con mismo nombre");
    SD.remove(NombreImagen);
  }
  myFile = SD.open(NombreImagen, FILE_WRITE);
  if (myFile)
  {
    Serial.println("Se guarda");
    if (RESOLUTION == VGA)
    {
     for (int jj=0; jj<VGA_HEIGHT; jj++)
     {
       myFile.write(&Imagen[jj*VGA_WIDTH],VGA_WIDTH);
     }
    }
    else if (RESOLUTION == QVGA)
    {
     for (int jj=0; jj<QVGA_HEIGHT; jj++)
     {
       myFile.write(&Imagen[jj*QVGA_WIDTH],QVGA_WIDTH);
     }
    }
    else if (RESOLUTION == QQVGA)
    {
     for (int jj=0; jj<QVGA_HEIGHT; jj++)
     {
       myFile.write(&Imagen[jj*QQVGA_WIDTH],QQVGA_WIDTH);
     }
    }
    NumImag++;
  }
  else
  {
       Serial.println("No se guarda");
  }
  
  myFile.close();
}

void SD_reg::CreateFolder(String path)
{
   char pathname[path.length()+1];
   path.toCharArray(pathname, sizeof(pathname));
   //Es probable que sea necesario añadir un "/" al final, en el caso en el que el path escrito no lo lleve.
   SD.mkdir(pathname);
   this->NumImag = 6;
   this->m_path = path;
}

String SD_reg::GetpathFolder(void)
{
  if (this->m_path)
  {
  return this->m_path;
  }
  else 
  return "Directorio Principal";
}


