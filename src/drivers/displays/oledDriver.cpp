#include "displayDriver.h"

#ifdef ESP32_OLED
#include "version.h"
#include "monitor.h"
#include <U8g2lib.h>

#include <Wire.h>

#define WIDTH 128
#define HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void oled_Display_Init(void)
{
    Serial.println("OLED ... display init");

    if (!u8g2.begin())
    {
        Serial.println(F("OLED allocation failed"));
        for (;;)
            ;
    }

    u8g2.clearBuffer();                  // clear the internal memory
    u8g2.setFont(u8g2_font_helvR14_tr);  // choose a suitable font
    u8g2.drawStr(0, 43, "NerdMiner V2"); // write something to the internal memory
    u8g2.sendBuffer();                   // transfer internal memory to the display
    delay(1000);
    u8g2.clearDisplay();
}

void oled_Display_AlternateScreenState(void)
{
    // tbd
}

void oled_Display_AlternateRotation(void)
{
    // tbd
}

void oled_Display_MinerScreen(unsigned long mElapsed)
{
    mining_data data = getMiningData(mElapsed);
    u8g2.clearBuffer();

    Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                  data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(0, 0);

    u8g2.drawStr(0, 12, data.currentHashRate.c_str());
    u8g2.drawStr(30, 12, "kH/s");

    u8g2.drawStr(64, 12, data.totalMHashes.c_str());
    u8g2.drawStr(80, 12, "MHashes");

    u8g2.drawStr(0, 24, data.valids.c_str());
    u8g2.drawStr(10, 24, "Blocks");

    u8g2.drawStr(64, 24, data.completedShares.c_str());
    u8g2.drawStr(80, 24, "Shares");

    u8g2.drawStr(0, 36, data.templates.c_str());
    u8g2.drawStr(30, 36, "Block Templates");

    u8g2.drawStr(0, 48, data.bestDiff.c_str());
    u8g2.drawStr(30, 48, "Best Difficulty");

    u8g2.drawStr(0, 60, data.temp.c_str());
    u8g2.drawStr(15, 60, "C");

    char timeMining[15];
    unsigned long secElapsed = millis() / 1000;
    int days = secElapsed / 86400;
    int hours = (secElapsed - (days * 86400)) / 3600;               // Number of seconds in an hour
    int mins = (secElapsed - (days * 86400) - (hours * 3600)) / 60; // Remove the number of hours and calculate the minutes.
    int secs = secElapsed - (days * 86400) - (hours * 3600) - (mins * 60);
    sprintf(timeMining, "%01d d %02d:%02d:%02d h", days, hours, mins, secs);

    u8g2.drawStr(60, 60, String(timeMining).c_str());
    u8g2.sendBuffer();
}

// uint16_t osx=64, osy=64, omx=64, omy=64, ohx=64, ohy=64;  // Saved H, M, S x & y coords
void oled_Display_ClockScreen(unsigned long mElapsed)
{
    clock_data_t data = getClockData_t(mElapsed);

    char clocktimeNow[8];
    uint8_t secs = data.currentSeconds;
    uint8_t mins = data.currentMinutes;
    uint8_t hours = data.currentHours;

    sprintf(clocktimeNow, "%02d:%02d:%02d", hours, mins, secs);

    // u8g2.clearDisplay();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR14_tr);
    u8g2.setCursor(0, 0);
    uint16_t w = u8g2.getStrWidth(String(clocktimeNow).c_str());

    u8g2.drawStr(u8g2_uint_t((WIDTH - w) / 2), 45, String(clocktimeNow).c_str());

    u8g2.sendBuffer();
}

void oled_Display_GlobalHashScreen(unsigned long mElapsed)
{
    Serial.println("OLED ... no global hash screen");
}

void oled_Display_LoadingScreen(void)
{

    Serial.println("OLED ... no loading screen");
    // tft.fillScreen(TFT_BLACK);
    // tft.pushImage(0, 0, initWidth, initHeight, initScreen);
    // tft.setTextColor(TFT_GOLD);
    // tft.drawString(CURRENT_VERSION, 2, 100, FONT2);
}

void oled_Display_SetupScreen(void)
{

    Serial.println("OLED ... no setup screen");
}

void oled_Display_AnimateCurrentScreen(unsigned long frame)
{
}

void oled_Display_DoLedStuff(unsigned long frame)
{
}

CyclicScreenFunction oled_DisplayCyclicScreens[] = {oled_Display_ClockScreen, oled_Display_MinerScreen};

DisplayDriver esp32_oled_driver = {
    oled_Display_Init,
    oled_Display_AlternateScreenState,
    oled_Display_AlternateRotation,
    oled_Display_LoadingScreen,
    oled_Display_SetupScreen,
    oled_DisplayCyclicScreens,
    oled_Display_AnimateCurrentScreen,
    oled_Display_DoLedStuff,
    SCREENS_ARRAY_SIZE(oled_DisplayCyclicScreens),
    0,
    WIDTH,
    HEIGHT};
#endif