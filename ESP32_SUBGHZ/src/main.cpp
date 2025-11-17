#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include "U8g2lib.h"
#include <Wire.h>
// custom includes
#include <menu.h>
#include <icon.h>
#include <radio.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // [full framebuffer, size = 1024 bytes]

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 384)
const unsigned char *bitmap_icons[8] = {
    bitmap_icon_3dcube,
    bitmap_icon_battery,
    bitmap_icon_dashboard,
    bitmap_icon_fireworks,
    bitmap_icon_gps_speed,
    bitmap_icon_knob_over_oled,
    bitmap_icon_parksensor,
    bitmap_icon_turbo};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 8320)
const unsigned char *bitmap_screenshots[8] = {
    bitmap_screenshot_3dcube,
    bitmap_screenshot_battery,
    bitmap_screenshot_gauges,
    bitmap_screenshot_fireworks,
    bitmap_screenshot_gps_speed,
    bitmap_screenshot_knob_over_oled,
    bitmap_screenshot_parksensor,
    bitmap_screenshot_turbo};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 8320)
const unsigned char *bitmap_qr_codes[8] = {
    bitmap_qr_3dcube,
    bitmap_qr_battery,
    bitmap_qr_gauges,
    bitmap_qr_fireworks,
    bitmap_qr_gps_speed,
    bitmap_qr_knob_over_oled,
    bitmap_qr_parksensor,
    bitmap_qr_turbo};

// Menu variables
const int NUM_ITEMS = 8;        // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name
char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    "3D Cube", "Battery", "Dashboard", "Fireworks",
    "GPS Speed", "Big Knob", "Park Sensor", "Turbo Gauge"
};
int button_up_clicked = 0;     // only perform action when button is clicked, and wait until another press
int button_select_clicked = 0; // same as above
int button_down_clicked = 0;   // same as above
int item_selected = 0; // which item in the menu is selected
int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one
int item_sel_next;     // next item - used in the menu screen to draw next item after the selected one
int current_screen = 0; // 0 = menu, 1 = screenshot, 2 = qr
// note - when changing the order of items above, make sure the other arrays referencing bitmaps
// also have the same order, for example array "bitmap_icons" for icons, and other arrays for screenshots and QR codes

void setup()
{
  Serial.begin(115200);
  // define pins for buttons
  u8g2.clearBuffer(); // clear buffer for storing display content in RAM

  Serial.println("\n[setup] Booting ESP32...");
  Serial.println("[setup] Initializing CC1101...");
  initCC1101(433.92);

  if (ELECHOUSE_cc1101.getCC1101())
  {
    Serial.println("[setup] ✅ Connection OK");
  }
  else
  {
    Serial.println("[setup] ❌ No CC1101 detected");
  }

  Serial.println("[setup] Setup complete.\n");
  // Call the menu setup function
  menuSetup();
}

void loop()
{
  // Menu loop to handle button presses and display updates
    // (Implementation of menu navigation and display logic goes here)
  if (current_screen == 0)
  { // MENU SCREEN

    // up and down buttons only work for the menu screen
    if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0))
    {                                    // up button clicked - jump to previous menu item
      item_selected = item_selected - 1; // select previous item
      button_up_clicked = 1;             // set button to clicked to only perform the action once
      if (item_selected < 0)
      { // if first item was selected, jump to last item
        item_selected = NUM_ITEMS - 1;
      }
    }
    else if ((digitalRead(BUTTON_DOWN_PIN) == LOW) && (button_down_clicked == 0))
    {                                    // down button clicked - jump to next menu item
      item_selected = item_selected + 1; // select next item
      button_down_clicked = 1;           // set button to clicked to only perform the action once
      if (item_selected >= NUM_ITEMS)
      { // last item was selected, jump to first menu item
        item_selected = 0;
      }
    }

    if ((digitalRead(BUTTON_UP_PIN) == HIGH) && (button_up_clicked == 1))
    { // unclick
      button_up_clicked = 0;
    }
    if ((digitalRead(BUTTON_DOWN_PIN) == HIGH) && (button_down_clicked == 1))
    { // unclick
      button_down_clicked = 0;
    }
  }

  if ((digitalRead(BUTTON_SELECT_PIN) == LOW) && (button_select_clicked == 0))
  {                            // select button clicked, jump between screens
    button_select_clicked = 1; // set button to clicked to only perform the action once
    if (current_screen == 0)
    {
      current_screen = 1;
    } // menu items screen --> screenshots screen
    else if (current_screen == 1)
    {
      current_screen = 2;
    } // screenshots screen --> qr codes screen
    else
    {
      current_screen = 0;
    } // qr codes screen --> menu items screen
  }
  if ((digitalRead(BUTTON_SELECT_PIN) == HIGH) && (button_select_clicked == 1))
  { // unclick
    button_select_clicked = 0;
  }

  // set correct values for the previous and next items
  item_sel_previous = item_selected - 1;
  if (item_sel_previous < 0)
  {
    item_sel_previous = NUM_ITEMS - 1;
  } // previous item would be below first = make it the last
  item_sel_next = item_selected + 1;
  if (item_sel_next >= NUM_ITEMS)
  {
    item_sel_next = 0;
  } // next item would be after last = make it the first

  u8g2.clearBuffer(); // clear buffer for storing display content in RAM

  if (current_screen == 0)
  { // MENU SCREEN

    // selected item background
    u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);

    // draw previous item as icon + label
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25, 15, menu_items[item_sel_previous]);
    u8g2.drawXBMP(4, 2, 16, 16, bitmap_icons[item_sel_previous]);

    // draw selected item as icon + label in bold font
    u8g2.setFont(u8g_font_7x14B);
    u8g2.drawStr(25, 15 + 20 + 2, menu_items[item_selected]);
    u8g2.drawXBMP(4, 24, 16, 16, bitmap_icons[item_selected]);

    // draw next item as icon + label
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25, 15 + 20 + 20 + 2 + 2, menu_items[item_sel_next]);
    u8g2.drawXBMP(4, 46, 16, 16, bitmap_icons[item_sel_next]);

    // draw scrollbar background
    u8g2.drawXBMP(128 - 8, 0, 8, 64, bitmap_scrollbar_background);

    // draw scrollbar handle
    u8g2.drawBox(125, 64 / NUM_ITEMS * item_selected, 3, 64 / NUM_ITEMS);

    u8g2.setFont(u8g2_font_5x8_tf); // small readable font
    u8g2.drawStr(108, 63, "C.J.");  // position bottom-right
  }
  else if (current_screen == 1)
  {                                                                  // SCREENSHOTS SCREEN
    u8g2.drawXBMP(0, 0, 128, 64, bitmap_screenshots[item_selected]); // draw screenshot
  }
  else if (current_screen == 2)
  {                                                               // QR SCREEN
    u8g2.drawXBMP(0, 0, 128, 64, bitmap_qr_codes[item_selected]); // draw qr code screenshot
  }

  u8g2.sendBuffer(); // send buffer from RAM to display controller
}