#include "ihm.h"

IHM::IHM(int enter, int back, int up, int down) {
  _wifiStatus = OFFLINE;
  _dbWifi = "";
  _time = "";
  _deviceName = "";

  _externalTemp = "";

  _refresh = true;
  _status = 0;

  _pinEnter = enter;
  _pinBack = back;
  _pinUp = up;
  _pinDown = down;

  pinMode(enter, INPUT);
  pinMode(back, INPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
}



void IHM::_printHeader() {
  // 'wifi', 16x16px
  const unsigned char PROGMEM epd_bitmap_wifi[] = {
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x3f, 0xfc, 0x78, 0x1e, 0xe0, 0x07, 0x8f, 0xf1, 0x1f, 0xf8,
    0x18, 0x18, 0x03, 0xc0, 0x07, 0xe0, 0x06, 0x60, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00
  };
  // 'no-wifi', 16x16px
  const unsigned char PROGMEM epd_bitmap_no_wifi[] = {
    0x00, 0x00, 0x00, 0x18, 0x01, 0x98, 0x0f, 0xf0, 0x3c, 0x7c, 0x73, 0xee, 0x6f, 0xf6, 0x1d, 0xb8,
    0x19, 0x98, 0x07, 0xe0, 0x06, 0x60, 0x07, 0x80, 0x0d, 0x80, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00
  };

  // 'web', 16x16px
  const unsigned char PROGMEM epd_bitmap_web[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1f, 0x80, 0x3f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };


  if (_wifiStatus == OFFLINE) {
    _LcdOLED.setTextSize(1);  // Draw 1X-scale text
    _LcdOLED.setCursor(20, 6);
    _LcdOLED.println(F("Nao conectado"));

    _LcdOLED.drawBitmap(_LcdOLED.width() - 16, 0, epd_bitmap_no_wifi, 16, 16, WHITE);
  } else if (_wifiStatus == CONNECTED) {
    _printTime();
    _LcdOLED.setTextSize(1);  // Draw 1X-scale text
    _LcdOLED.setCursor(_LcdOLED.width() - 16 * (_dbWifi.length() / 2 + 1), 6);
    _LcdOLED.println(_dbWifi);
    _LcdOLED.drawBitmap(_LcdOLED.width() - 16, 0, epd_bitmap_wifi, 16, 16, WHITE);
  } else {
    _printTime();
    _LcdOLED.setTextSize(1);  // Draw 1X-scale text
    _LcdOLED.setCursor(_LcdOLED.width() - 16 * (_dbWifi.length() / 2 + 2), 6);
    _LcdOLED.println(_dbWifi);
    _LcdOLED.drawBitmap(_LcdOLED.width() - 16, 0, epd_bitmap_web, 16, 16, WHITE);
    _LcdOLED.drawBitmap(_LcdOLED.width() - 32, 0, epd_bitmap_wifi, 16, 16, WHITE);
  }
  _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
  _LcdOLED.display();
}



void IHM::_print() {
  switch (_status) {
    case 0:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(2);  // Draw 2X-scale text
      _LcdOLED.setCursor(_LcdOLED.width() / 2 - 12 * (_externalTemp.length() / 2), 19);
      _LcdOLED.println(_externalTemp);
      _LcdOLED.setTextSize(2);  // Draw 2X-scale text
      _LcdOLED.setCursor(0, 35);
      _LcdOLED.println(_deviceName);
      _LcdOLED.setTextSize(1);
      _LcdOLED.setCursor(0, 56);
      _LcdOLED.print("Pressione algum botao");
      _LcdOLED.display();
      break;

    case 1:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(1);  // Draw 1x-scale text
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("->");
      _LcdOLED.setCursor(12, 19);
      _LcdOLED.println("Configurar Aparelho");
      _LcdOLED.setCursor(12, 31);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.setCursor(12, 43);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.display();
      break;

    case 2:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("->");
      _LcdOLED.setCursor(12, 19);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.setCursor(12, 31);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.setCursor(12, 43);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.display();
      break;

    case 3:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 31);
      _LcdOLED.println("->");
      _LcdOLED.setCursor(12, 19);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.setCursor(12, 31);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.setCursor(12, 43);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.display();
      break;

    case 4:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 43);
      _LcdOLED.println("->");
      _LcdOLED.setCursor(12, 19);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.setCursor(12, 31);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.setCursor(12, 43);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.display();
      break;

    case 10:
      _LcdOLED.clearDisplay();
      _printHeader();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("Configurar Wi-Fi");
      _LcdOLED.setTextSize(2);
      _LcdOLED.setCursor(0, 31);
      _LcdOLED.println("Confirme!");
      _LcdOLED.setTextSize(1);
      _LcdOLED.setCursor(0, 48);
      _LcdOLED.println("Deseja alterar a rede?");
      _LcdOLED.display();
      break;

    case 11:
      _LcdOLED.clearDisplay();
      _LcdOLED.setCursor(0, 0);
      _LcdOLED.println("Configurar Wi-Fi");
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(0, 19);

      _LcdOLED.println((*_wifiProv)().c_str());
      _LcdOLED.display();
      break;

    case 12:
      _LcdOLED.clearDisplay();
      _LcdOLED.setCursor(0, 0);
      _LcdOLED.println("Configurar Wi-Fi");
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(16, 19);

      _LcdOLED.println("Sucesso!!");
      _LcdOLED.display();

      vTaskDelay(pdMS_TO_TICKS(2000));

      _status = 0;
      _print();
      break;

    case 13:
      _LcdOLED.clearDisplay();
      _LcdOLED.setCursor(0, 0);
      _LcdOLED.println("Configurar Wi-Fi");
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(16, 19);

      _LcdOLED.println("Erro!!\nFalha na autenticação");
      _LcdOLED.display();
      break;

    case 20:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("Voce tera 15 segundos");
      _LcdOLED.setCursor(0, 42);
      _LcdOLED.println("Pressione Enter para comecar!");
      _LcdOLED.display();
      break;

    case 21:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Aperte o comando");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("do controle");
      _LcdOLED.display();

      if ((*_addCommand)()) {
        _status = 22;
      } else {
        _status = 25;
      }
      _print();

      break;

    case 22:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Confirme!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("o aparelho Apitou?");
      _LcdOLED.display();

      (*_testCommand)();

      break;

    case 23:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Salvando o comando");
      _LcdOLED.display();

      if ((*_saveCmdOn)()) {
        _status = 24;
      } else {
        _status = 25;
      }
      _print();
      break;

    case 24:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd On");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Pronto!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("deu tudo certo.");
      _LcdOLED.display();
      vTaskDelay(pdMS_TO_TICKS(2000));

      _status = 2;
      _print();
      break;

    case 30:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("Voce tera 15 segundos");
      _LcdOLED.setCursor(0, 42);
      _LcdOLED.println("Pressione Enter para comecar!");
      _LcdOLED.display();
      break;

    case 31:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Aperte o comando");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("do controle");
      _LcdOLED.display();

      if ((*_addCommand)()) {
        _status = 32;
      } else {
        _status = 35;
      }
      _print();

      break;

    case 32:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Confirme!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("o aparelho Apitou?");
      _LcdOLED.display();

      (*_testCommand)();

      break;

    case 33:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Salvando o comando");
      _LcdOLED.display();

      if ((*_saveCmdOff)()) {
        _status = 34;
      } else {
        _status = 35;
      }
      _print();
      break;

    case 34:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Off");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Pronto!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("deu tudo certo.");
      _LcdOLED.display();

      vTaskDelay(pdMS_TO_TICKS(2000));
      _status = 3;
      _print();
      break;

    case 40:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setCursor(0, 19);
      _LcdOLED.println("Voce tera 15 segundos");
      _LcdOLED.setCursor(0, 42);
      _LcdOLED.println("Pressione Enter para comecar!");
      _LcdOLED.display();
      break;

    case 41:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Aperte o comando");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("do controle");
      _LcdOLED.display();

      if ((*_addCommand)()) {
        _status = 42;
      } else {
        _status = 45;
      }
      _print();

      break;

    case 42:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Confirme!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("o aparelho Apitou?");
      _LcdOLED.display();

      (*_testCommand)();

      break;

    case 43:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Salvando o comando");
      _LcdOLED.display();

      if ((*_saveCmdVentilate)()) {
        _status = 44;
      } else {
        _status = 45;
      }
      _print();
      break;

    case 44:
      _LcdOLED.clearDisplay();
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(0, 6);
      _LcdOLED.println("Adicionar Cmd Vent.");
      _LcdOLED.drawLine(0, 17, _LcdOLED.width() - 1, 17, WHITE);
      _LcdOLED.setTextSize(1);  // Draw 1X-scale text
      _LcdOLED.setCursor(12, 25);
      _LcdOLED.println("Pronto!");
      _LcdOLED.setCursor(12, 37);
      _LcdOLED.println("deu tudo certo.");
      _LcdOLED.display();
      vTaskDelay(pdMS_TO_TICKS(2000));
      _status = 4;
      _print();
      break;

    default:
      _LcdOLED.clearDisplay();
      _LcdOLED.setCursor(0, 0);
      _LcdOLED.setTextSize(2);
      _LcdOLED.println("Ops!\nTemos um  Problema.");
      _LcdOLED.display();
      break;
  }
}

void IHM::setExternalTemp(String value) {
  if (!value.equals(_externalTemp)) {
    _externalTemp = value;
    _refresh = true;
  }
}

void IHM::setDBWifi(String value) {
  if (!value.equals(_dbWifi)) {
    _dbWifi = value;
    _refresh = true;
  }
}

void IHM::setDeviceName(String value) {
  if (!value.equals(_deviceName)) {
    _deviceName = value;
    _refresh = true;
  }
}

void IHM::setTime(String value) {
  if (!value.equals(_time)) {
    _time = value;
    _refresh = true;
  }
}

void IHM::setWifiStatus(ihmWifiStatus wifiStatus) {
  if (wifiStatus != _wifiStatus) {
    _wifiStatus = wifiStatus;
    _refresh = true;
  }
}

void IHM::controller() {

  ihmInteraction interaction = NONE;
  if (digitalRead(_pinEnter)) {
    interaction = ENTER;
  }
  if (digitalRead(_pinBack)) {
    interaction = BACK;
  }
  if (digitalRead(_pinUp)) {
    interaction = UP;
  }
  if (digitalRead(_pinDown)) {
    interaction = DOWN;
  }

  if (_status == 0 && interaction != NONE) {
    _status = 1;
    _refresh = true;
  } else if (_status / 10 == 0 && interaction != NONE) {
    if (interaction == BACK) {
      _status = 0;
    } else if (interaction == ENTER) {
      _status = _status * 10;
    } else if (interaction == UP) {
      if (_status == 1) {
        _status = 4;
      } else {
        _status--;
      }
    } else if (interaction == DOWN) {
      if (_status == 4) {
        _status = 1;
      } else {
        _status++;
      }
    }
    _refresh = true;

  } else if (interaction != NONE) {
    if (interaction == BACK && _status != 22) {
      _status /= 10;
      _refresh = true;
    }

    if ((_status == 10 || _status == 20 || _status == 30 || _status == 40) && interaction == ENTER) {
      _status++;
      _refresh = true;
    }

    if (_status == 22 || _status == 32 || _status == 42) {
      if (interaction == ENTER) {
        _status++;
        _refresh = true;
      } else if (interaction == BACK) {
        _status += 3;
        _refresh = true;
      }
    }
  }

  if (_refresh) {
    _refresh = false;
    _print();
  }
}

bool IHM::begin() {
  const unsigned char PROGMEM logo_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xcf, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xcf, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xe7, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc7, 0xe7, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x31, 0x3b, 0xf5, 0xf2, 0x2f, 0x8e, 0x00, 0x01, 0x3b, 0x7f, 0xf5, 0xfe, 0x2f, 0x9f, 0x00,
    0x01, 0x3b, 0x60, 0xcc, 0x66, 0x23, 0x31, 0x80, 0x01, 0x2d, 0x38, 0xcc, 0x66, 0x23, 0x31, 0x80,
    0x01, 0x2f, 0x1c, 0xcc, 0x66, 0x23, 0x31, 0x80, 0x01, 0x27, 0x0c, 0xcc, 0x66, 0x63, 0x31, 0x80,
    0x01, 0x27, 0x7c, 0xcc, 0x63, 0xe3, 0x1f, 0x00, 0x01, 0x21, 0x30, 0x04, 0x41, 0x82, 0x04, 0x00,
    0x00, 0x0e, 0x41, 0x03, 0x98, 0x10, 0x80, 0x00, 0x00, 0x1f, 0x7b, 0xe7, 0xde, 0x38, 0xc0, 0x00,
    0x00, 0x18, 0x43, 0x36, 0x13, 0x38, 0xc0, 0x00, 0x00, 0x1e, 0x7b, 0x37, 0x9f, 0x2c, 0xc0, 0x00,
    0x00, 0x1e, 0x7b, 0x37, 0x9e, 0x6c, 0xc0, 0x00, 0x00, 0x18, 0x43, 0x36, 0x16, 0x7c, 0xc0, 0x00,
    0x00, 0x18, 0x7b, 0xe7, 0x93, 0x46, 0xf0, 0x00, 0x00, 0x18, 0x7b, 0xc7, 0x91, 0xc6, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02, 0x80, 0x20, 0x00, 0x00,
    0x00, 0x01, 0x0d, 0xe2, 0xba, 0x67, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x23, 0x9a, 0x69, 0x00, 0x00,
    0x00, 0x00, 0x35, 0x22, 0x2a, 0x69, 0x00, 0x00, 0x00, 0x01, 0xdc, 0xc2, 0x3b, 0xe7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  Wire.begin();
  _LcdOLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_LcdOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  // Clear the buffer
  _LcdOLED.clearDisplay();
  _LcdOLED.drawBitmap(_LcdOLED.width() / 4, 0, logo_bmp, 64, 64, WHITE);
  _LcdOLED.display();
  _LcdOLED.setTextColor(WHITE);
  return true;
}

bool IHM::begin(int sda, int scl) {

  const unsigned char PROGMEM logo_bmp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xcf, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xcf, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xe7, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc7, 0xe7, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x31, 0x3b, 0xf5, 0xf2, 0x2f, 0x8e, 0x00, 0x01, 0x3b, 0x7f, 0xf5, 0xfe, 0x2f, 0x9f, 0x00,
    0x01, 0x3b, 0x60, 0xcc, 0x66, 0x23, 0x31, 0x80, 0x01, 0x2d, 0x38, 0xcc, 0x66, 0x23, 0x31, 0x80,
    0x01, 0x2f, 0x1c, 0xcc, 0x66, 0x23, 0x31, 0x80, 0x01, 0x27, 0x0c, 0xcc, 0x66, 0x63, 0x31, 0x80,
    0x01, 0x27, 0x7c, 0xcc, 0x63, 0xe3, 0x1f, 0x00, 0x01, 0x21, 0x30, 0x04, 0x41, 0x82, 0x04, 0x00,
    0x00, 0x0e, 0x41, 0x03, 0x98, 0x10, 0x80, 0x00, 0x00, 0x1f, 0x7b, 0xe7, 0xde, 0x38, 0xc0, 0x00,
    0x00, 0x18, 0x43, 0x36, 0x13, 0x38, 0xc0, 0x00, 0x00, 0x1e, 0x7b, 0x37, 0x9f, 0x2c, 0xc0, 0x00,
    0x00, 0x1e, 0x7b, 0x37, 0x9e, 0x6c, 0xc0, 0x00, 0x00, 0x18, 0x43, 0x36, 0x16, 0x7c, 0xc0, 0x00,
    0x00, 0x18, 0x7b, 0xe7, 0x93, 0x46, 0xf0, 0x00, 0x00, 0x18, 0x7b, 0xc7, 0x91, 0xc6, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02, 0x80, 0x20, 0x00, 0x00,
    0x00, 0x01, 0x0d, 0xe2, 0xba, 0x67, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x23, 0x9a, 0x69, 0x00, 0x00,
    0x00, 0x00, 0x35, 0x22, 0x2a, 0x69, 0x00, 0x00, 0x00, 0x01, 0xdc, 0xc2, 0x3b, 0xe7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  Wire.begin(sda, scl);
  _LcdOLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_LcdOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  // Clear the buffer
  _LcdOLED.clearDisplay();
  _LcdOLED.drawBitmap(_LcdOLED.width() / 4, 0, logo_bmp, 64, 64, WHITE);
  _LcdOLED.display();
  _LcdOLED.setTextColor(WHITE);
  return true;
}

void IHM::_printTime() {
  _LcdOLED.setCursor(0, 6);
  _LcdOLED.println(_time);
}

void IHM::setAddCommand(bool (*fuction)()) {
  _addCommand = fuction;
}

void IHM::setTestCommand(void (*fuction)()) {
  _testCommand = fuction;
}
void IHM::setSaveCmdOn(bool (*fuction)()) {
  _saveCmdOn = fuction;
}

void IHM::setSaveCmdOff(bool (*fuction)()) {
  _saveCmdOff = fuction;
}
void IHM::setSaveCmdVentilate(bool (*fuction)()) {
  _saveCmdVentilate = fuction;
}

void IHM::setWifiProv(String (*fuction)()) {
  _wifiProv = fuction;
}