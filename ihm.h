#ifndef ihm_H_
#define ihm_H_


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

enum ihmWifiStatus { OFFLINE,
                     CONNECTED,
                     ONLINE };

enum ihmInteraction { ENTER,
                      BACK,
                      UP,
                      DOWN,
                      DONE,
                      UNDONE,
                      NONE };
class IHM {
private:
  ihmWifiStatus _wifiStatus;
  String _dbWifi;
  String _externalTemp;
  String _time;
  String _deviceName;
  int _status;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     //Reset pin # (or -1 if sharing Arduino reset pin)

  void _printHeader();
  void _printTime();
  void _print();

  int _pinEnter;
  int _pinBack;
  int _pinUp;
  int _pinDown;

  bool(*_addCommand) ();
  void(*_testCommand) ();
  bool (*_saveCmdOn)();
  bool (*_saveCmdOff)();
  bool (*_saveCmdVentilate)();

  bool _refresh;

  Adafruit_SSD1306 _LcdOLED;
public:
  IHM(int enter, int back, int up, int down);
  bool begin();
  bool begin(int sda, int scl);

  void setExternalTemp(String value);
  void setDBWifi(String value);
  void setDeviceName(String value);
  void setTime(String value);
  void setWifiStatus(ihmWifiStatus wifiStatus);
  void setAddCommand(bool (*fuction)());
  void setTestCommand(void (*fuction)());
  void setSaveCmdOn(bool (*fuction)());
  void setSaveCmdOff(bool (*fuction)());
  void setSaveCmdVentilate(bool (*fuction)());

  void controller();
};
#endif