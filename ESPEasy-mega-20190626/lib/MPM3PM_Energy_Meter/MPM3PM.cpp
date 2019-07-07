/* Library for reading MPM3PM Modbus Energy meters.
*  Reading via Hardware or Software Serial library & rs232<->rs485 converter
*  2016-2018 Reaper7 (tested on wemos d1 mini->ESP8266 with Arduino 1.9.0-beta & 2.4.1 esp8266 core)
*  crc calculation by Jaime Garc�a (https://github.com/peninquen/Modbus-Energy-Monitor-Arduino/)
*/
//------------------------------------------------------------------------------
#include "MPM3PM.h"
//------------------------------------------------------------------------------
#ifdef USE_HARDWARESERIAL
MPM3PM::MPM3PM(HardwareSerial& serial, long baud, int dere_pin, int config, bool swapuart) : MPM3PMSer(serial) {
  this->_baud = baud;
  this->_config = config;
  this->_dere_pin = dere_pin;
  this->_swapuart = swapuart;
}
#else
MPM3PM::MPM3PM(ESPeasySerial& serial, long baud, int dere_pin) : MPM3PMSer(serial) {
  this->_baud = baud;
  this->_dere_pin = dere_pin;
}
#endif

MPM3PM::~MPM3PM() {
}

void MPM3PM::begin(void) {
#ifdef USE_HARDWARESERIAL
  #ifdef ESP8266
    MPM3PMSer.begin(_baud, (SerialConfig)_config);
  #else
    MPM3PMSer.begin(_baud, _config);
  #endif
#else
  MPM3PMSer.begin(_baud);
#endif
#ifdef USE_HARDWARESERIAL
  #ifdef ESP8266
    if (_swapuart)
      MPM3PMSer.swap();
  #endif
#endif
  if (_dere_pin != NOT_A_PIN)	                                                  //set output pin mode for DE/RE pin when used (for control MAX485)
    pinMode(_dere_pin, OUTPUT);
}


int32_t MPM3PM::readVal(uint16_t reg, uint8_t node) {
  uint16_t temp;
  unsigned long resptime;
  uint8_t MPM3PMarr[FRAMESIZE_OUT] = {node, MPM3PM_B_02, 0, 0, MPM3PM_B_05, MPM3PM_B_06, 0, 0, 0}; // for MPM3P
  //float res = nan;
  int32_t res = 3141;
  uint16_t readErr = MPM3PM_ERR_NO_ERROR;

  MPM3PMarr[2] = highByte(reg);
  MPM3PMarr[3] = lowByte(reg);

  temp = calculateCRC(MPM3PMarr, FRAMESIZE_OUT - 3);                                   //calculate out crc only from first 6 bytes

  MPM3PMarr[6] = lowByte(temp); // for MPM3P
  MPM3PMarr[7] = highByte(temp);

#ifndef USE_HARDWARESERIAL
  MPM3PMSer.listen();                                                              //enable softserial rx interrupt
#endif

  while (MPM3PMSer.available() > 0)  {                                             //read serial if any old data is available
    MPM3PMSer.read();
  }

  if (_dere_pin != NOT_A_PIN)                                                   //transmit to MPM3PM  -> DE Enable, /RE Disable (for control MAX485)
    digitalWrite(_dere_pin, HIGH);

  delay(2);                                                                     // necessary?

  MPM3PMSer.write(MPM3PMarr, FRAMESIZE_OUT - 1);                                          //send 8 bytes

  MPM3PMSer.flush();                                                               //clear out tx buffer

  if (_dere_pin != NOT_A_PIN)                                                   //receive from MPM3PM -> DE Disable, /RE Enable (for control MAX485)
    digitalWrite(_dere_pin, LOW);

  resptime = millis() + MAX_MILLIS_TO_WAIT;

  while (MPM3PMSer.available() < FRAMESIZE_IN) {
    if (resptime < millis()) {
      readErr = MPM3PM_ERR_TIMEOUT;                                                //err debug (4)
      break;
    }
    yield();
  }

  if (readErr == MPM3PM_ERR_NO_ERROR) {                                            //if no timeout...

    if(MPM3PMSer.available() >= FRAMESIZE_IN) {

      for(int n=0; n<FRAMESIZE_IN; n++) {
        MPM3PMarr[n] = MPM3PMSer.read();
      }

      if (MPM3PMarr[0] == node && MPM3PMarr[1] == MPM3PM_B_02 && MPM3PMarr[2] == MPM3PM_REPLY_BYTE_COUNT) {

        if ((calculateCRC(MPM3PMarr, FRAMESIZE_IN - 2)) == ((MPM3PMarr[8] << 8) | MPM3PMarr[7])) {  //calculate crc from first 7 bytes and compare with received crc (bytes 7 & 8)
          ((uint8_t*)&res)[3]= MPM3PMarr[3];
          ((uint8_t*)&res)[2]= MPM3PMarr[4];
          ((uint8_t*)&res)[1]= MPM3PMarr[5];
          ((uint8_t*)&res)[0]= MPM3PMarr[6];
        } else {
          readErr = MPM3PM_ERR_CRC_ERROR;                                          //err debug (1)
        }

      } else {
        readErr = MPM3PM_ERR_WRONG_BYTES;                                          //err debug (2)
      }

    } else {
      readErr = MPM3PM_ERR_NOT_ENOUGHT_BYTES;                                      //err debug (3)
    }

  }

  if (readErr != MPM3PM_ERR_NO_ERROR) {                                            //if error then copy temp error value to global val and increment global error counter
    readingerrcode = readErr;
    readingerrcount++;
  } else {
    ++readingsuccesscount;
  }

  while (MPM3PMSer.available() > 0)  {                                             //read redundant serial bytes, if any
    MPM3PMSer.read();
  }

#ifndef USE_HARDWARESERIAL
  MPM3PMSer.end();                                                                 //disable softserial rx interrupt
#endif

  return (res);
}

uint16_t MPM3PM::getErrCode(bool _clear) {
  uint16_t _tmp = readingerrcode;
  if (_clear == true)
    clearErrCode();
  return (_tmp);
}

uint16_t MPM3PM::getErrCount(bool _clear) {
  uint16_t _tmp = readingerrcount;
  if (_clear == true)
    clearErrCount();
  return (_tmp);
}

uint16_t MPM3PM::getSuccCount(bool _clear) {
  uint16_t _tmp = readingsuccesscount;
  if (_clear == true)
    clearSuccCount();
  return (_tmp);
}

void MPM3PM::clearErrCode() {
  readingerrcode = MPM3PM_ERR_NO_ERROR;
}

void MPM3PM::clearErrCount() {
  readingerrcount = 0;
}

void MPM3PM::clearSuccCount() {
  readingsuccesscount = 0;
}

uint16_t MPM3PM::calculateCRC(uint8_t *array, uint8_t num) {
  uint16_t _crc, _flag;
  _crc = 0xFFFF;
  for (uint8_t i = 0; i < num; i++) {
    _crc = _crc ^ array[i];
    for (uint8_t j = 8; j; j--) {
      _flag = _crc & 0x0001;
      _crc >>= 1;
      if (_flag)
        _crc ^= 0xA001;
    }
  }
  return _crc;
}
