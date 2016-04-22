#ifndef DEFINITION_H
#define DEFINITION_H


/* **************************************************************
   ***                                                        ***
   ***                  CONFIG COMANDS                        ***
   ***                                                        ***
   **************************************************************/
#define M_GET_CAMERA_CONFIG			(uint8_t)   1
#define M_GET_ACQ_CONFIG            (uint8_t)   2
#define M_GET_WIFI_CONFIG           (uint8_t)   3
#define M_GET_LED_CONFIG            (uint8_t)   4
#define M_GET_SD_IFO                (uint8_t)   5
#define M_GET_SD_IMAGE              (uint8_t)   6
#define M_GET_LED_STATUS            (uint8_t)   7

#define M_SET_CAMERA_CONFIG         (uint8_t)  20
#define M_SET_ACQ_CONFIG            (uint8_t)  21
#define M_SET_WIFI_CONFIG           (uint8_t)  22
#define M_SET_LED_CONFIG            (uint8_t)  23
#define M_SET_LED_STATUS            (uint8_t)  24


#define P_GET_CAMERA_CONFIG			(uint8_t)   50
#define P_GET_ACQ_CONFIG            (uint8_t)   51
#define P_GET_WIFI_CONFIG           (uint8_t)   52
#define P_GET_LED_CONFIG            (uint8_t)   53
#define P_GET_SD_IFO                (uint8_t)   54
#define P_GET_SD_IMAGE              (uint8_t)   55
#define P_GET_LED_STATUS            (uint8_t)   56

#define P_SET_CAMERA_CONFIG         (uint8_t)  70
#define P_SET_ACQ_CONFIG            (uint8_t)  71
#define P_SET_WIFI_CONFIG           (uint8_t)  72
#define P_SET_LED_CONFIG            (uint8_t)  73
#define P_SET_LED_STATUS            (uint8_t)  74

/* **************************************************************
   ***                                                        ***
   ***                    SATE COMANDS                        ***
   ***                                                        ***
   **************************************************************/

#define G_GET_STATE                 (uint8_t)  100
#define G_SET_STATE                 (uint8_t)  101
#define G_WACHDOG                   (uint8_t)  102

#define M_START_AUTO                (uint8_t)  120
#define A_ABORT_AUTO                (uint8_t)  121
#define A_PAUSE_AUTO                (uint8_t)  122
#define P_ABORT_AUTO                (uint8_t)  123
#define M_RESUME                    (uint8_t)  124

/* **************************************************************
   ***                                                        ***
   ***                    MORE COMANDS                        ***
   ***                                                        ***
   **************************************************************/

#define P_GET_LAST_IMAGE            (uint8_t)  200
#define M_GET_IMAGE                 (uint8_t)  201


/* **************************************************************
   ***                                                        ***
   ***                   IMAGE RESOLUTIONS                    ***
   ***                                                        ***
   **************************************************************/

#define VGA                  (byte)   1
#define QVGA                 (byte)   2
#define QQVGA                (byte)   3

#define VGA_HEIGHT           (int)    480
#define VGA_WIDTH            (int)    640
#define VGA_LENGTH           (int)    307200

#define QVGA_HEIGHT          (int)    240
#define QVGA_WIDTH           (int)    320
#define QVGA_LENGTH          (int)    76800

#define QQVGA_HEIGHT         (int)    120
#define QQVGA_WIDTH          (int)    160
#define QQVGA_LENGTH         (int)    19200


#endif
