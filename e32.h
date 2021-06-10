#ifndef E32_H
#define E32_H

#define PARLEN 6

#define ODD 1
#define EVEN 2

#define U1200 0
#define U2400 1
#define U4800 2
#define U9600 3
#define U19200 4
#define U38400 5
#define U57600 6
#define U115200 7

#define A300 0
#define A1200 1
#define A2400 2
#define A4800 3
#define A9600 4
#define A19200 5

#define TM_TRANSPARENT 0
#define TM_FIXED 1

#define IO_OC 0
#define IO_DEF 1

#define WOR250 0
#define WOR500 1
#define WOR750 2
#define WOR1000 3
#define WOR1250 4
#define WOR1500 5
#define WOR1750 6
#define WOR2000 7

#define FEC_OFF 0
#define FEC_ON 1

#define POWER4 0 // 20dBm for T100, 27dBm for T500, 30dBm for T1W
#define POWER3 1 // 17dBm for T100, 24dBm for T500, 27dBm for T1W
#define POWER2 2 // 14dBm for T100, 21dBm for T500, 24dBm for T1W
#define POWER1 3 // 10dBm for T100, 18dBm for T500, 21dBm for T1W

#include <QMainWindow>
#include <QObject>

class e32
{
public:
    e32();
    uint8_t parameters[6];
    void setParity(uint8_t* paramArray, uint8_t parity);
    void setUartSpeed(uint8_t* paramArray, uint8_t speed);
    void setAirSpeed(uint8_t* paramArray, uint8_t speed);
    void setRadioChannel(uint8_t* paramArray, uint8_t channel);
    void setTramsmissionMode(uint8_t* paramArray, uint8_t mode);
    void setIODriveMode(uint8_t* paramArray, uint8_t mode);
    void setWakeUpTime(uint8_t* paramArray, uint8_t wor);
    void setFEC(uint8_t* paramArray, uint8_t fec);
    void setPower(uint8_t* paramArray, uint8_t pwr);

    uint8_t getParity(uint8_t* paramArray);
    uint8_t getUartSpeed(uint8_t* paramArray);
    uint8_t getAirSpeed(uint8_t* paramArray);
    uint8_t getRadioChannel(uint8_t* paramArray);
    uint8_t getTramsmissionMode(uint8_t* paramArray);
    uint8_t getIODriveMode(uint8_t* paramArray);
    uint8_t getWakeUpTime(uint8_t* paramArray);
    uint8_t getFEC(uint8_t* paramArray);
    uint8_t getPower(uint8_t* paramArray);

private:

};

#endif // E32_H
