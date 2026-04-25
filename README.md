# arduino-r4-PONG
A single-player Pong game built for the Arduino UNO R4 WiFi, using the onboard 8×12 LED matrix as the game display and an external SSD1306 OLED to show your live score. 

---

## Project Preview
https://github.com/user-attachments/assets/0e0c08d0-06ea-4960-bbe1-327881711188
### (The video was recorded before the final buzzer and OLED were added)

---

## Wiring
<img width="124" height="150" alt="arduino_pong_wiring_diagram" src="https://github.com/user-attachments/assets/087b445f-392c-44dd-af3d-46d55968f956" />

---

The ball bounces around the 8×12 LED matrix at a speed controlled by dt (default 50ms loop delay) and moves one pixel every two loop iterations
The paddle is 3 LEDs wide and starts centered at the bottom row
Scoring: each successful paddle hit increments the score shown on the OLED
Game over: a descending tone plays, the score resets, and the ball relaunches from a random position
Sound effects:

Wall / ceiling bounce → short high beep (B6)
Paddle hit → short higher beep (C7)
Miss → descending three-tone fail sound
