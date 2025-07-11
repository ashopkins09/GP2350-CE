# GP2040 Configuration for Raspberry Pi Pico2W

![Pin Mapping](PinMapping.png)

Basic pin setup for a **Raspberry Pi Pico2W (RP2350, 4MB, WiFi, onboard LED)**.  
Pair with a GPIO breakout or screw terminal board for an easy DIY arcade stick or leverless controller using **GP2040-CE**.

This configuration enables low-latency USB gamepad input for:
- **PC (XInput, DInput)**
- **Nintendo Switch**
- **PS3 / PS4 / PS5**
- **Arcade cabinet controllers**

---

## Main Pin Mapping Configuration

| Pico2W GPIO Pin | Action                           | GP2040 | XInput | Switch | PS3/4/5 | DInput | Arcade |
|-----------------|----------------------------------|--------|--------|--------|---------|--------|--------|
| GPIO 02         | `GpioAction::BUTTON_PRESS_UP`   | UP     | UP     | UP     | UP      | UP     | UP     |
| GPIO 03         | `GpioAction::BUTTON_PRESS_DOWN` | DOWN   | DOWN   | DOWN   | DOWN    | DOWN   | DOWN   |
| GPIO 04         | `GpioAction::BUTTON_PRESS_RIGHT`| RIGHT  | RIGHT  | RIGHT  | RIGHT   | RIGHT  | RIGHT  |
| GPIO 05         | `GpioAction::BUTTON_PRESS_LEFT` | LEFT   | LEFT   | LEFT   | LEFT    | LEFT   | LEFT   |
| GPIO 06         | `GpioAction::BUTTON_PRESS_B1`   | B1     | A      | B      | Cross   | 2      | K1     |
| GPIO 07         | `GpioAction::BUTTON_PRESS_B2`   | B2     | B      | A      | Circle  | 3      | K2     |
| GPIO 08         | `GpioAction::BUTTON_PRESS_R2`   | R2     | RT     | ZR     | R2      | 8      | K3     |
| GPIO 09         | `GpioAction::BUTTON_PRESS_L2`   | L2     | LT     | ZL     | L2      | 7      | K4     |
| GPIO 10         | `GpioAction::BUTTON_PRESS_B3`   | B3     | X      | Y      | Square  | 1      | P1     |
| GPIO 11         | `GpioAction::BUTTON_PRESS_B4`   | B4     | Y      | X      | Triangle| 4      | P2     |
| GPIO 12         | `GpioAction::BUTTON_PRESS_R1`   | R1     | RB     | R      | R1      | 6      | P3     |
| GPIO 13         | `GpioAction::BUTTON_PRESS_L1`   | L1     | LB     | L      | L1      | 5      | P4     |
| GPIO 16         | `GpioAction::BUTTON_PRESS_S1`   | S1     | Back   | Minus  | Select  | 9      | Coin   |
| GPIO 17         | `GpioAction::BUTTON_PRESS_S2`   | S2     | Start  | Plus   | Start   | 10     | Start  |
| GPIO 18         | `GpioAction::BUTTON_PRESS_L3`   | L3     | LS     | LS     | L3      | 11     | LS     |
| GPIO 19         | `GpioAction::BUTTON_PRESS_R3`   | R3     | RS     | RS     | R3      | 12     | RS     |
| GPIO 20         | `GpioAction::BUTTON_PRESS_A1`   | A1     | Guide  | Home   | PS      | 13     | -      |
| GPIO 21         | `GpioAction::BUTTON_PRESS_A2`   | A2     | -      | Capture| -       | 14     | -      |

---
