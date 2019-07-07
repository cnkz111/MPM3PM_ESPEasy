/* Library for reading MPM3PM 120/220/230/630 Modbus Energy meters.
*  Reading via Hardware or Software Serial library & rs232<->rs485 converter
*  2016-2018 Reaper7 (tested on wemos d1 mini->ESP8266 with Arduino 1.9.0-beta & 2.4.1 esp8266 core)
*  crc calculation by Jaime García (https://github.com/peninquen/Modbus-Energy-Monitor-Arduino/)
*/
//------------------------------------------------------------------------------
#ifndef MPM3PM_h
#define MPM3PM_h
//------------------------------------------------------------------------------
#include <Arduino.h>
#include <MPM3PM_Config_User.h>
#ifdef USE_HARDWARESERIAL
  #include <HardwareSerial.h>
#else
  #include <ESPeasySerial.h>
//  #include <SoftwareSerial.h>
#endif
//------------------------------------------------------------------------------
//DEFAULT CONFIG (DO NOT CHANGE ANYTHING!!! for changes use MPM3PM_Config_User.h):
//------------------------------------------------------------------------------
#ifndef MPM3PM_UART_BAUD
  #define MPM3PM_UART_BAUD                     9600                                //default baudrate
#endif

#ifndef DERE_PIN
  #define DERE_PIN                          NOT_A_PIN                           //default digital pin for control MAX485 DE/RE lines (connect DE & /RE together to this pin)
#endif

#ifdef USE_HARDWARESERIAL

  #ifndef MPM3PM_UART_CONFIG
    #define MPM3PM_UART_CONFIG              SERIAL_8N1                          //default hardware uart config
  #endif

  #ifndef SWAPHWSERIAL
    #define SWAPHWSERIAL                    0                                   //(only esp8266) when hwserial used, then swap uart pins from 3/1 to 13/15 (default not swap)
  #endif

#endif

#ifndef MAX_MILLIS_TO_WAIT
  #define MAX_MILLIS_TO_WAIT                500                                 //default max time to wait for response from MPM3PM
#endif
//------------------------------------------------------------------------------
//#define FRAMESIZE                           9                                   //size of out/in array
#define FRAMESIZE_OUT                         9                                   //size of out/in array
#define FRAMESIZE_IN                          9                                   //size of out/in array
#define MPM3PM_REPLY_BYTE_COUNT                0x04                                //number of bytes with data
//#define MPM3PM_REPLY_BYTE_COUNT                0x02                                //number of bytes with data

#define MPM3PM_B_01                            0x01                                //BYTE 1 -> slave address (default value 1 read from node 1)
#define MPM3PM_B_02                            0x04                                //BYTE 2 -> function code (default value 0x04 read from 3X input registers)
                                                                                //BYTES 3 & 4 (BELOW)

//MPM3PM registers modified for MPM3P Modbus meter
#define MPM3PM_VOLTAGE1                     0X0008 //0.1V 
#define MPM3PM_VOLTAGE2                     0x000A //0.1V 
#define MPM3PM_VOLTAGE3                     0x000C //0.1V
#define MPM3PM_CURRENT1                     0x000E //0.01A 
#define MPM3PM_CURRENT2                     0x0010 //0.01A
#define MPM3PM_CURRENT3                     0x0012 //0.01A 
#define MPM3PM_POWER1                       0x0014 //0.01W
#define MPM3PM_POWER2                       0x0016 //0.01W
#define MPM3PM_POWER3                       0x0018 //0.01W
#define MPM3PM_POWERTOTAL                   0x0026 //0.01W
#define MPM3PM_VOLT_AMPS1                   0x5B24 //0.01VA 
#define MPM3PM_VOLT_AMPS2                   0x5B26 //0.01VA 
#define MPM3PM_VOLT_AMPS3                   0x5B28 //0.01VA 
#define MPM3PM_VOLT_AMPS_TOTAL              0x5B24 //0.01VA 
#define MPM3PM_VOLT_AMPS_REACTIVE1          0x001A //0.01var
#define MPM3PM_VOLT_AMPS_REACTIVE2          0x001C //0.01var
#define MPM3PM_VOLT_AMPS_REACTIVE3          0x001E //0.01var
#define MPM3PM_VOLT_AMPS_REACTIVE_TOTAL     0x0028 //0.01var
#define MPM3PM_POWER_FACTOR1                0x0020 //0.001 
#define MPM3PM_POWER_FACTOR2                0x0022 //0.001 
#define MPM3PM_POWER_FACTOR3                0x0024 //0.001 
#define MPM3PM_POWER_FACTOR_TOTAL           0x002A //0.001 
#define MPM3PM_FREQUENCY                    0x002C //0.01Hz
#define MPM3PM_IMPORT_ACTIVE_ENERGY         0X0002 //0.01kWh
#define MPM3PM_EXPORT_ACTIVE_ENERGY         0X0004 //0.01kWh
#define MPM3PM_TOTAL_ACTIVE_ENERGY			0x0000 //0.01kWh 


#define MPM3PM_B_05                            0x00                                //BYTE 5
#define MPM3PM_B_06                            0x02                                //BYTE 6
//------------------------------------------------------------------------------
#define MPM3PM_ERR_NO_ERROR                    0                                   //no error
#define MPM3PM_ERR_CRC_ERROR                   1                                   //crc error
#define MPM3PM_ERR_WRONG_BYTES                 2                                   //bytes b0,b1 or b2 wrong
#define MPM3PM_ERR_NOT_ENOUGHT_BYTES           3                                   //not enough bytes from MPM3PM
#define MPM3PM_ERR_TIMEOUT                     4                                   //timeout

//------------------------------------------------------------------------------
class MPM3PM {
  public:
#ifdef USE_HARDWARESERIAL
    MPM3PM(HardwareSerial& serial, long baud = MPM3PM_UART_BAUD, int dere_pin = DERE_PIN, int config = MPM3PM_UART_CONFIG, bool swapuart = SWAPHWSERIAL);
#else
    MPM3PM(ESPeasySerial& serial, long baud = MPM3PM_UART_BAUD, int dere_pin = DERE_PIN);
#endif
    virtual ~MPM3PM();

    void begin(void);
    int32_t readVal(uint16_t reg, uint8_t node = MPM3PM_B_01);                       //read value from register = reg and from deviceId = node
    uint16_t getErrCode(bool _clear = false);                                   //return last errorcode (optional clear this value, default false)
    uint16_t getErrCount(bool _clear = false);                                  //return total errors count (optional clear this value, default false)
    uint16_t getSuccCount(bool _clear = false);                                 //return total success count (optional clear this value, default false)
    void clearErrCode();                                                        //clear last errorcode
    void clearErrCount();                                                       //clear total errors count
    void clearSuccCount();                                                      //clear total success count

  private:
#ifdef USE_HARDWARESERIAL
    HardwareSerial& MPM3PMSer;
#else
    ESPeasySerial& MPM3PMSer;
#endif

#ifdef USE_HARDWARESERIAL
    int _config = MPM3PM_UART_CONFIG;
    bool _swapuart = SWAPHWSERIAL;
#endif
    long _baud = MPM3PM_UART_BAUD;
    int _dere_pin = DERE_PIN;
    uint16_t readingerrcode = MPM3PM_ERR_NO_ERROR;                                 //4 = timeout; 3 = not enough bytes; 2 = number of bytes OK but bytes b0,b1 or b2 wrong, 1 = crc error
    uint16_t readingerrcount = 0;                                               //total errors couter
    uint32_t readingsuccesscount = 0;
    uint16_t calculateCRC(uint8_t *array, uint8_t num);
};
#endif //MPM3PM_h
