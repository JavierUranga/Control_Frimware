#include "System_Defs.h"
#include "Definitions.h"
#include "pwm_led.h"
#include "ov7670.h"
#include "SD_reg.h"
#include <Wire.h>

///////////////////////////////////////////////////////////////////////
int count1 = 0;
int task1;
int Time_Interval = 4000;


//******************************************************************************************************************
//                            I2C-EEPROM SETTINGS
//******************************************************************************************************************
TwoWire I2C;

//******************************************************************************************************************
//                            PROGRAM VARIABLES
//******************************************************************************************************************
// Camera Config
ov7670  Camera(I2C);
pwm_LED	LED_control();
SD_reg  SD_control();  // Quitar los (), probablemente en el pwm_Led haya que hacer los mismo.

#define TRIG  46   //Photo button

#define TRIG2 47  //SD save y LED dimming button
uint8_t  *Imagen; //VGA
byte RESOLUTION;
String path = "yep/";



void setup(){
  
   // Start serial for output
  Serial.begin(250000);
  Serial.println("Empieza Setup");
  Imagen=(uint8_t *)malloc(VGA_LENGTH*sizeof(uint8_t));
  RESOLUTION = VGA;

  // Trigger
  pinMode(TRIG, INPUT);
  pinMode(TRIG2, INPUT);
  
  //Dimming LED
 
   
  // PIN configuration
  TRISE = B11111111;  //Set port E to input
  TRISD = B11111111; //Set port D to input
  
  //I2C 
  I2C.begin(); 
  
  Camera.init();
  Camera.setSerial(&Serial);
  
  Serial.println("CAMERA CONFIG PRE");
  Camera.PrintRegister();
 
  //Camera.Reset();
   
  Camera.InitDefaultReg();

  Serial.print("Camera YUV Gray Scale \r\n");
  Camera.InitYUV(); 
  
  
  Camera.InitVGA();
  Serial.print("Camera VGA \r\n");

                  
          
  Serial.println("CAMERA CONFIG POST");
  Camera.PrintRegister();


  SD_control.PrintRegisterSD();
}


void loop (){
  
  String DataRead = "";

  if(digitalRead(TRIG)==HIGH)
  {
     Serial.println("Recibido Trigger");
     Camera.takeImageYUV(Imagen);
     delay(500);
  }
   DataRead = Serial_ReadLine();
   if (DataRead == "<CAPTURE_IMAGE_FIFO>")
   {
    Camera.takeImageYUV(Imagen);
    Serial.println("<CAPTURE_IMAGE_FIFO><ACK>");      
   }
   else if (DataRead == "<READ_IMAGE_FIFO>")
   {
     Serial.println("<READ_IMAGE_FIFO><ACK>");
     Image_RAM_to_Serial();    
    //SendImage_WIFI=true;
   }
    else if (DataRead == "<READ_CAMERA_CONFIG>")
   {
     Serial.println("<READ_CAMERA_CONFIG><ACK>");
     Camera.PrintRegister();      
   }
   //Guardar SD
   else if (DataRead == "<SAVE_IMAGE_SD>")
   {
    Serial.println("Se hace la funcion");
    SD_control.Save(Imagen, RESOLUTION);
 //   SaveImage();
   }
   else if (DataRead == "<AUTOMATIC_MODE>")
   {
    destroyTask(task1);
    //destroyTask(task2);
    count1 = 0;
    task1 = createTask(AutomaticTask, Time_Interval, TASK_ENABLE, NULL);

    //task2 = createTask(AutomaticTask1, 300, 20, NULL);

    //task1 = createTask(AutomaticTask, Time_Interval, TASK_ENABLE, NULL);

    //Automatic_Mode();
   }

   else if (DataRead == "<RESOLUTION_TYPE_VGA>")
   {
    Serial.print("Camera VGA \r\n");
    free(Imagen);
    Imagen=(uint8_t *)malloc(VGA_LENGTH*sizeof(uint8_t));
    RESOLUTION = VGA;
    //Camera.Reset();
    Camera.InitDefaultReg();
    Camera.InitVGA();
   }
   else if (DataRead == "<RESOLUTION_TYPE_QVGA>")
   {
    Serial.print("Camera QVGA \r\n");
    free(Imagen);
    Imagen=(uint8_t *)malloc(QVGA_LENGTH*sizeof(uint8_t));
    RESOLUTION = QVGA;
    Camera.InitDefaultReg();
    Camera.InitQVGA();
   }
   else if (DataRead == "<RESOLUTION_TYPE_QQVGA>")
   {
    Serial.print("Camera QQVGA \r\n");
    free(Imagen);
    Imagen=(uint8_t *)malloc(QQVGA_LENGTH*sizeof(uint8_t));
    RESOLUTION = QQVGA;
    Camera.InitDefaultReg();
    Camera.InitQQVGA();
   }

   else if (DataRead == "P_GET_SD_IFO")
   {
    SD_control.PrintRegisterSD();
   }
   
   else if (DataRead == "CreateFolder")
   {
    SD_control.CreateFolder(path);
   }

   else if (DataRead == "pathfolder")
   {
    String pathfolder;
    
    pathfolder = SD_control.GetpathFolder();
    Serial.print("Path: ");
    Serial.println(pathfolder);
   }

   delay(50);

   if (count1 >= 30)
   {
    destroyTask(task1);
   }


   
   //Save in SD
   if(digitalRead(TRIG2)==HIGH)
   {
    Serial.println("Se hace la función");
 //   SaveImage();
    delay(500);
   }
}


 void Image_RAM_to_Serial()
{
 //VGA
 if (RESOLUTION == VGA)         { Serial.write(&Imagen[0], 307200); } 
 //QVGA
 else if (RESOLUTION == QVGA)   { Serial.write(&Imagen[0], 76800); }
 //QQVGA
 else if (RESOLUTION == QQVGA)  { Serial.write(&Imagen[0], 19200); }
}

 String Serial_ReadLine ()
{
  String DataRead = "";
  char character;
  
  while(Serial.available()) 
  {
      character = Serial.read();
      if (!((character == '\r') || (character == '\n'))) DataRead.concat(character);
      //Serial.println(DataRead);
  }
  return DataRead;  
}

String Serial_ReadLine1 ()
{
  String DataRead1 = "";
  char character1;
  while(!Serial.available() ) {}
  delay(1); //Necesario un delay porque sino no lee todo.
  while(Serial.available()) 
  {
      character1 = Serial.read();
      //Serial.println(Serial.available());
      if (!((character1 == '\r') || (character1 == '\n'))) DataRead1.concat(character1);
      //Serial.println(DataRead1);
  }
  Serial.println(DataRead1);
  //Serial.print("Available: ");
  //Serial.println(Serial.available());
  return DataRead1;  
}

void Serial_ReadBytes (uint8_t *buffer, int length)
{
  uint8_t character;
  int cnt = 0;
  
  while(!Serial.available() ) {}
  while(Serial.available() ) 
  {
      character = Serial.read();
      if (!((character == '\r') || (character == '\n')))
      {
        buffer[cnt] = character;
        cnt++;
      }
  }
}

//int Serial_ReadNum ()
//{
//  byte buffer[4];
//  uint8_t character;
//  int cnt = 0;
//  int Result = 0;
//  
//  while(!Serial.available() ) {}
//  while(Serial.available() ) 
//  {
//      character = Serial.read();  
//      if ( !((character == '\r') || (character == '\n')) || cnt<4)
//      {
//        buffer[cnt] = character;
//        cnt++;
//      }
//  }
//   memcpy(&Result, buffer, 4);
//   return Result;
//}

int Serial_ReadNum ()
{
  uint8_t character;
  int cnt = 0;
  int Result = 0;
  
  while(!Serial.available() ) {}
  while(Serial.available() ) 
  {
       character = Serial.read();
      //if (!((character == '\r') || (character == '\n'))) character = character;
  }
   Serial.println(character);
   return character;
}

//void SaveImage()
//{
//  String NomImag = "Photo_";
//  NomImag.concat(NImag);
//  //NomImag.concat(".jpg");    // Para guardarla como jpg hay que ponerle un header.
//  char NombreImagen[NomImag.length()+1];
//  NomImag.toCharArray(NombreImagen, sizeof(NombreImagen));
//  Serial.println(NombreImagen);
//  if (SD.exists(NombreImagen)) 
//  {
//    Serial.println("Borrando archivo con mismo nombre");
//    SD.remove(NombreImagen);
//  }
//  myFile = SD.open(NombreImagen, FILE_WRITE);
//  if (myFile)
//  {
//    Serial.println("Se guarda");
//    if (RESOLUTION == VGA)
//    {
//     for (int jj=0; jj<VGA_HEIGHT; jj++)
//     {
//       myFile.write(&Imagen[jj*VGA_WIDTH],VGA_WIDTH);
//     }
//    }
//    else if (RESOLUTION == QVGA)
//    {
//     for (int jj=0; jj<QVGA_HEIGHT; jj++)
//     {
//       myFile.write(&Imagen[jj*QVGA_WIDTH],QVGA_WIDTH);
//     }
//    }
//    else if (RESOLUTION == QQVGA)
//    {
//     for (int jj=0; jj<QVGA_HEIGHT; jj++)
//     {
//       myFile.write(&Imagen[jj*QQVGA_WIDTH],QQVGA_WIDTH);
//     }
//    }
//  }
//  else
//  {
//       Serial.println("No se guarda");
//  }
//  
//  myFile.close();
//  NImag++;
//}


//void saveImage()
//{
//   if (SD.exists("Imagen.txt")) {
//    Serial.println("Imagen.txt exists and removing it.");
//    SD.remove("Imagen.txt");
//  }
//  else {
//    Serial.println("test.txt doesn't exist.");
//  }
//  if (SD.exists("Imagenes/Fotos/Imagen.txt")) {
//    Serial.println("Imagen2.txt exists and removing it.");
//    SD.remove("Imagenes/Fotos/Imagen.txt");
//    SD.rmdir("Imagenes/Fotos");
//  }
//  else {
//    Serial.println("test.txt doesn't exist.");
//  }
//   myFile = SD.open("Imagen.txt", FILE_WRITE);
//   Serial.println(myFile);
//
//   
//   if (myFile) {
//    Serial.print("Writing to test.txt...");
//
//    long t_start=millis();
//     for (int jj=0; jj<480; jj++) 
//  {
//   //for (int ii=0; ii<640; ii++)
////    {
//   myFile.write(Imagen[jj],640);
//  
//  // delay(1);
//  //  }
//  }
//   myFile.flush();
// long t_end=millis();
//
//long tiempo = t_end - t_start;
//Serial.print("Tiempo: ");
//Serial.println(tiempo, DEC);
// 
// // close the file:
//    myFile.close();
//    Serial.println("done.");
//  } 
//   else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }
////   uint8_t Imagen2[240][320];;
//
//
//   SD.mkdir("Imagenes/Fotos");
//   myFile = SD.open("Imagenes/Fotos/Imagen.txt", FILE_WRITE);
//   Serial.println(myFile);
//    long t_start=millis();
//   myFile.write(*Imagen, 480*640);
//    long t_end=millis();
//    long tiempo = t_end - t_start;
//    Serial.print("Tiempo: ");
//    Serial.println(tiempo, DEC);
//   myFile.flush();
//   myFile.close();
//
//
//   
////  // re-open the file for reading:
////  myFile = SD.open("Imagen.txt");
////  if (myFile) {
////    Serial.println("Imagen.txt:");
////    
//    // read from the file until there's nothing else in it:
////    while (myFile.available()) {
////      Serial.write(myFile.read());
////    }
////    // close the file:
////    myFile.close();
////  } else {
////    // if the file didn't open, print an error:
////    Serial.println("error opening test2.txt");
////  }
//  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
//  root.openRoot(volume);
//  
//  // list all files in the card with date and size
//  root.ls(LS_R | LS_DATE | LS_SIZE);
//}

 

//void Automatic_Mode()
//{
//  //double Total_Time = 5*60; // Double, Necesario para que haga bien el redondeo.
//  int Time_Between = 15;
//  //String x = Serial_ReadLine1 (); //Comentado cuando enviamos los datos por puerto serie y no por la interface
//  //int y = x.toInt();
//  //Serial.println(y);
//  float Total_Time = (3)*60;
//  Serial.print("Total Time: ");
//  Serial.println (Total_Time);
//  float Num_Imagenes = Total_Time/Time_Between;
//  Serial.print("Numero de Imagenes: ");
//  Serial.println (Num_Imagenes);
//  //Serial.println (Num_Imagenes);
//  Num_Imagenes = round(Num_Imagenes);
//  Serial.println (Num_Imagenes);
//  for (int ii=0; ii<Num_Imagenes; ii++)
//  {
//    Camera.takeImageYUV(Imagen);
//   SaveImage();
//   delay((Time_Between-3)*1000);
//  }
//  Serial.println("Imagenes tomadas");
//}


void AutomaticTask(int id, void * tptr) 
{
  Serial.println("HOLA");
  Camera.takeImageYUV(Imagen);
  SD_control.Save(Imagen, RESOLUTION);
  count1++;
}

void AutomaticTask1(int id, void * tptr) 
{
  Serial.println("Automatic");
}


