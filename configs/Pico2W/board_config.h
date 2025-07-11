#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

// Board: Pico2W (RP2350)
// Flash: 4MB
// USB: onboard
// LED: onboard (GPIO 25)

// Onboard LED
#define BOARD_LED_PIN 25

// USB pins (typically handled by SDK, but defined for clarity)
#define BOARD_USB_DP_PIN 24
#define BOARD_USB_DM_PIN 25

// Example GPIO assignments for buttons
#define BUTTON_PIN_UP     2
#define BUTTON_PIN_DOWN   3
#define BUTTON_PIN_LEFT   4
#define BUTTON_PIN_RIGHT  5
#define BUTTON_PIN_B      6
#define BUTTON_PIN_A      7
#define BUTTON_PIN_X      8
#define BUTTON_PIN_Y      9
#define BUTTON_PIN_L      10
#define BUTTON_PIN_R      11
#define BUTTON_PIN_SELECT 12
#define BUTTON_PIN_START  13

// Analog pins (replace if using actual analog sticks)
#define ANALOG_LX_PIN 26
#define ANALOG_LY_PIN 27
#define ANALOG_RX_PIN 28
#define ANALOG_RY_PIN 29

#endif
