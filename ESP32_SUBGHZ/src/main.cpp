#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include "U8g2lib.h"
#include <Wire.h>
// custom includes
#include <menu.h>
#include <icon.h>
#include <radio.h>
#include "signals.h"
#include "button.h"

// create display object
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// create button objects
Button buttonUp(BUTTON_UP_PIN);
Button buttonDown(BUTTON_DOWN_PIN);
Button buttonSelect(BUTTON_SELECT_PIN);
Button buttonBack(BUTTON_BACK_PIN);

// Keep your existing bitmap arrays
const unsigned char *bitmap_icons[8] = {
    bitmap_icon_3dcube,
    bitmap_icon_battery,
    bitmap_icon_dashboard,
    bitmap_icon_fireworks,
    bitmap_icon_gps_speed,
    bitmap_icon_knob_over_oled,
    bitmap_icon_parksensor,
    bitmap_icon_turbo
};



// Category menu variables
int category_selected = 0;
int category_sel_previous;
int category_sel_next;

// Signal submenu variables
int signal_selected = 0;
int signal_sel_previous;
int signal_sel_next;

// Screen states: 0=category menu, 1=signal submenu, 2=signal details, 3=transmit
int current_screen = 0;

void setup()
{
    Serial.begin(115200);
    
    Serial.println("\n[setup] Booting ESP32...");
    Serial.println("[setup] Initializing CC1101...");
    initCC1101(433.92);
    
    if (ELECHOUSE_cc1101.getCC1101()) {
        Serial.println("[setup] ✅ Connection OK");
    } else {
        Serial.println("[setup] ❌ No CC1101 detected");
    }
    
    Serial.println("[setup] Setup complete.\n");
    menuSetup();
    u8g2.clearBuffer();
}

void loop()
{
    // Button handling
    if (current_screen == 0) {
        if (buttonUp.isPressed()) {
            category_selected = (category_selected - 1 + NUM_CATEGORIES) % NUM_CATEGORIES;
        }
        if (buttonDown.isPressed()) {
            category_selected = (category_selected + 1) % NUM_CATEGORIES;
        }
    }
    else if (current_screen == 1) {
        int num_signals = CATEGORIES[category_selected].count;
        if (buttonUp.isPressed()) {
            signal_selected = (signal_selected - 1 + num_signals) % num_signals;
        }
        if (buttonDown.isPressed()) {
            signal_selected = (signal_selected + 1) % num_signals;
        }
    }
    
    if (buttonSelect.isPressed()) {
        if (current_screen == 0) {
            current_screen = 1;
            signal_selected = 0;
        }
        else if (current_screen == 1) {
            current_screen = 2;
        }
        else if (current_screen == 2) {
            current_screen = 3;
            // Transmit using radio object
            SubGHzSignal* signal = &CATEGORIES[category_selected].signals[signal_selected];
            radio.transmit(signal->samples, signal->length, signal->frequency);
        }
    }
    
    if (buttonBack.isPressed()) {
        if (current_screen > 0) {
            current_screen--;
        }
    }
    
    // ========== DRAW SCREENS ==========
    u8g2.clearBuffer();
    
    if (current_screen == 0) {
        // ========== CATEGORY MENU ==========
        u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);
        
        // Previous category
        u8g2.setFont(u8g_font_7x14);
        u8g2.drawStr(25, 15, CATEGORIES[category_sel_previous].name);
        u8g2.drawXBMP(4, 2, 16, 16, bitmap_icons[category_sel_previous % 8]);
        
        // Selected category (bold)
        u8g2.setFont(u8g_font_7x14B);
        u8g2.drawStr(25, 37, CATEGORIES[category_selected].name);
        u8g2.drawXBMP(4, 24, 16, 16, bitmap_icons[category_selected % 8]);
        
        // Next category
        u8g2.setFont(u8g_font_7x14);
        u8g2.drawStr(25, 59, CATEGORIES[category_sel_next].name);
        u8g2.drawXBMP(4, 46, 16, 16, bitmap_icons[category_sel_next % 8]);
        
        // Scrollbar
        u8g2.drawXBMP(128 - 8, 0, 8, 64, bitmap_scrollbar_background);
        u8g2.drawBox(125, 64 / NUM_CATEGORIES * category_selected, 3, 64 / NUM_CATEGORIES);
        
        u8g2.setFont(u8g2_font_5x8_tf);
        u8g2.drawStr(108, 63, "C.J.");
    }
    else if (current_screen == 1) {
        // ========== SIGNAL SUBMENU ==========
        SubGHzSignal* signals = CATEGORIES[category_selected].signals;
        
        // Draw category title at top
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(5, 10, CATEGORIES[category_selected].name);
        u8g2.drawLine(0, 12, 128, 12);
        
        // Adjust positions for submenu
        u8g2.drawFrame(2, 18, 124, 18);
        
        // Previous signal
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(8, 26, signals[signal_sel_previous].name);
        
        // Selected signal (bold)
        u8g2.setFont(u8g2_font_7x13B_tf);
        u8g2.drawStr(8, 46, signals[signal_selected].name);
        
        // Show frequency
        u8g2.setFont(u8g2_font_5x8_tf);
        char freq_str[20];
        snprintf(freq_str, 20, "%.2f MHz", signals[signal_selected].frequency);
        u8g2.drawStr(8, 56, freq_str);
        
        // Next signal
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(8, 66, signals[signal_sel_next].name);
        
        // Scrollbar
        u8g2.drawFrame(122, 14, 4, 48);
        int scrollbar_pos = map(signal_selected, 0, num_signals - 1, 0, 40);
        u8g2.drawBox(123, 15 + scrollbar_pos, 2, 6);
    }
    else if (current_screen == 2) {
        // ========== SIGNAL DETAILS ==========
        SubGHzSignal* signal = &CATEGORIES[category_selected].signals[signal_selected];
        
        u8g2.setFont(u8g2_font_7x13B_tf);
        u8g2.drawStr(5, 12, signal->name);
        u8g2.drawLine(0, 14, 128, 14);
        
        u8g2.setFont(u8g2_font_6x10_tf);
        
        // Category
        char cat_str[25];
        snprintf(cat_str, 25, "Cat: %s", CATEGORIES[category_selected].name);
        u8g2.drawStr(5, 28, cat_str);
        
        // Frequency
        char freq_str[25];
        snprintf(freq_str, 25, "Freq: %.2f MHz", signal->frequency);
        u8g2.drawStr(5, 40, freq_str);
        
        // Samples
        char len_str[25];
        snprintf(len_str, 25, "Samples: %d", signal->length);
        u8g2.drawStr(5, 52, len_str);
        
        // Instruction
        u8g2.setFont(u8g2_font_5x8_tf);
        u8g2.drawStr(5, 63, "SELECT to transmit");
    }
    else if (current_screen == 3) {
        // ========== TRANSMIT SCREEN ==========
        SubGHzSignal* signal = &CATEGORIES[category_selected].signals[signal_selected];
        
        u8g2.setFont(u8g2_font_9x15B_tf);
        u8g2.drawStr(15, 25, "SENDING...");
        
        u8g2.setFont(u8g2_font_7x13_tf);
        u8g2.drawStr(10, 45, signal->name);
        
        char freq_str[20];
        snprintf(freq_str, 20, "%.2f MHz", signal->frequency);
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(30, 58, freq_str);
        
        u8g2.sendBuffer();  // Show immediately
        
        // ← CALL YOUR SEND FUNCTION
        sendSamples(signal->samples, signal->length, signal->frequency);
        
        // Show completion
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_9x15B_tf);
        u8g2.drawStr(35, 30, "SENT!");
        u8g2.sendBuffer();
        delay(800);
        
        current_screen = 1;  // Back to signal submenu
    }
    
    u8g2.sendBuffer();
    delay(10);
}