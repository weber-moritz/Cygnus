This is a project for a little  game, its basically the final.
It is just a box that recieves 4 balls that play sound and animations when the balls are placed.

This is a rubbish code but works for what is supposed to.

# Wiriing:
## Components:
- Arduino Nano
- DFPlayer mini
- <3W Speaker
- strip of neopixel/ws2812b (dividable by 4)
- 4x btns (make shure the balls are heavy enough. i used limit switches, tactile btns are not enough)
- 4x 10k resistor
- 1x 1k resistor
- wires
- breadboard/pcb


### Peripherals
| Component           | Pin    | Connection / Destination      | Notes                                  |
| ------------------- | ------ | ----------------------------- | -------------------------------------- |
| **NeoPixel Strip**  | 5V/VCC | `+5V Power Rail`              | (maybe powered by the external supply) |
|                     | GND    | `GND Power Rail`              |                                        |
|                     | DIN/DI | Arduino Nano `D2`             |                                        |
| **DFPlayer Mini**   | VCC    | `+5V Power Rail`              | (maybe powered by the external supply) |
|                     | GND    | `GND Power Rail`              |                                        |
|                     | RX     | Arduino Nano `D5 (TX)`        | **Via 1kΩ Resistor (In Series)**       |
|                     | TX     | Arduino Nano `D4 (RX)`        |                                        |
|                     | SPK1   | Speaker (+) Terminal          |                                        |
|                     | SPK2   | Speaker (-) Terminal          |                                        |

### Buttons (10kΩ Pull-Down Resistor Setup)
This wiring pattern is repeated for all four buttons, connecting each to its own Digital Pin (D6-D9).

| Component                  | Pin   | Connection / Destination          | Notes                                     |
| -------------------------- | ----- | --------------------------------- | ----------------------------------------- |
| **Button (Example for D8)**| Leg 1 | Arduino Nano `5V Pin`             | Provides power to the button.             |
|                            | Leg 2 | Arduino Nano `Digital Pin 6`      | Sends the `HIGH` signal when pressed.     |
| **10kΩ Resistor**          | End 1 | The **same** `Leg 2` as above     | This creates the pull-down circuit.       |
|                            | End 2 | `GND Power Rail`                  | Ensures the pin reads `LOW` when not pressed. |


# Sound files
1. Create 4 mp3 files about 10s long that gets played when a ball gets placed
2. craete a 5th mp3 file that playes once all balls are in place
3. format an <=32gb sd card to fat32
4. create a dir on the sd called "MP3"
5. put the sound files in the dir and rename them to 0001.mp3-00005.mp3 (last one is the final one)
6. stuff the sd card in the df player mini

I did not include the sound files, as they are are protected by the license.
I pulled a ton of sound files from BBC and mixed them: https://sound-effects.bbcrewind.co.uk/

If i ever come around to create new sound files, they will be included here as well.
The files are just 10s long soudn files of mechanical and electrical noises mashed together to make some sort of sense, idk...


# Hardware
So i created this for a woodn box we got somwhere.
We just sprayed it with back paint.
I created a 3D printable insert that takes all 4 balls and the electronics.
Im sure you need to fit the file for your balls and box, but its parametric with a vars table.
You will definitly need hot glue. Its very important. I cant without anymore. I just cant... Im sorry mom...
