#include <U8g2lib.h>
#include <Wire.h>
#include <WS2812FX.h>
#include <String.h>

// LED信号IO
#define LED_PIN 3
// 定义真实的LED灯珠数量
#define NUM_LEDS 18
// 按钮IO
#define BUTTON_PIN 4
// 定义亮度
#define BRIGHTNESS 220


// 灯带
WS2812FX ws2812fx = WS2812FX(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
//# IIC version
//ESP32C3 OLED开发板的屏幕接线：scl-->gpio6  sda-->gpio5 如果是Arduino则改为scl-->A5  sda-->A4
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 6, 5, U8X8_PIN_NONE);
// 当前显示模式
int model = -1;
// button 按钮状态
bool buttonStatue = false;
// 是否需要状态更新
bool needUpdate = false;
// 按钮按下时长
int buttonDownTime = 0;

void setup(void) {
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  ws2812fx.init();
  ws2812fx.setBrightness(0);
  ws2812fx.setSpeed(0);
  ws2812fx.setColor(0x000000);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  u8g2.setContrast(250);
  u8g2.begin();
}

void loop(void) {

  // 按键消抖
  if (digitalRead(BUTTON_PIN)) {
    // 睡眠 5ms，如果依然为高电平，说明抖动已消失。
    delay(5);

    if (digitalRead(BUTTON_PIN)) {
      buttonStatue = true;
      buttonDownTime = buttonDownTime + 1;
    }
  } else {
    buttonDownTime = 0;

    if (buttonStatue == true) {
      model = model + 1;
      needUpdate = true;
      if (model >= 4) {
        model = 0;
      }
    }

    buttonStatue = false;
  }

  ws2812fx.service();
  u8g2.firstPage();

  if (needUpdate == true) {
    needUpdate == false;

    do {
      // u8g2.setFont(u8g2_font_ncenB10_tr);
      // u8g2.drawStr(30, 24, strModel.c_str());
      ShowModel(model);
      ShowLed(model);
    } while (u8g2.nextPage());
  }
}

void ShowModel(int m) {
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(30, 24, "X Dagger");
  char buffer[40];
  sprintf(buffer, "Model %d", m);

  u8g2.drawStr(30, 48, buffer);
}

void ShowLed(int m) {

  switch (m) {
    case 0:
      ws2812fx.init();
      ws2812fx.setBrightness(0);
      ws2812fx.setSpeed(0);
      ws2812fx.setColor(0x000000);
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.start();
      break;
    case 1:
      ws2812fx.init();
      ws2812fx.setBrightness(BRIGHTNESS);
      ws2812fx.setSpeed(1000);           // 速度
      ws2812fx.setColor(0xFFFF00);       // 红色
      ws2812fx.setMode(FX_MODE_STATIC);  // 静态模式
      ws2812fx.start();
      break;
    case 2:
      ws2812fx.init();
      ws2812fx.setBrightness(BRIGHTNESS);
      ws2812fx.setSpeed(1000);           // 速度
      ws2812fx.setColor(0x0000FF);       // 蓝色
      ws2812fx.setMode(FX_MODE_STATIC);  // 静态模式
      ws2812fx.start();
      break;
    case 3:
      ws2812fx.init();
      ws2812fx.setBrightness(BRIGHTNESS);
      ws2812fx.setSpeed(1000);           // 速度
      ws2812fx.setColor(0xFF0000);       // 黄色
      ws2812fx.setMode(FX_MODE_STATIC);  // 静态模式
      ws2812fx.start();
      break;
    default:
      ws2812fx.init();
      ws2812fx.setBrightness(0);
      ws2812fx.setSpeed(0);
      ws2812fx.setColor(0x000000);
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.start();
      break;
  }
}
