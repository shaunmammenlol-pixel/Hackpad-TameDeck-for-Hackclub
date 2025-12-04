# Hackpad-TameDeck-for-Hackclub
It's basically a cheaper version of the stream deck which uses a raspberry pico.(much more cheaper and affordable) the code(thanks to gemini), and the case (designed by me) with the readme is all attached down here so please do go through it!..P.S, Pcb making is not done ive gone through the route of hand wiring all the diodes. rather than custom pcb

Heres the list of main hardware i have used for the project!:
Microcontroller: Raspberry Pi Pico (RP2040) @2020 (kinda old yes, but doe sthe job.) 
Inputs: 11 Tactile Push Buttons (9 in matrix, 2 dedicated yes/no buttons.)
Encoder: EC11 Rotary Encoder(US\sed
Display: SSD1306 OLED (128x64)
Case Design: Tilted Wedge/Stream deck surface. (White.)
Case Hardware holder: Holds all the stuff ie, the cherry MX switches, the oled screen,encoder etc.

Glue: Any Sort of mighty glue can hold all of this together.(No screws required which is cost effective and easier to assemble.)

Working:
There a matrix of 9 keys, (in the shape of a keypad but way different in the terms of how it works lol.).When a person presses a key to open an app, The oled screen goes from the homescreen to the app's name which the user is about to open.If, the application which the user wants to open is correct the (yes) button [in the left side] and the action or the app will be opened or if the application is wrong the user clicks the (no) button in the [in the right side] and it returns to the homescreen.

HOW TO CHANGE THE APPLICATIONS:                                    (Example:Discord to Outlook:)
Say, if the user doesnt like the preset applications then they can go to the raspberry code and do this:


Before:                                                              
                                                      
KC.LAUNCH_DISCORD = KC.MACRO( (KC.LGUI(KC.R), KC.DF('discord'), KC.ENTER) )                                                  
KC.APP_1 = KC.LAUNCH_DISCORD                                     

To:
KC.LAUNCH_OUTLOOK = KC.MACRO( (KC.LGUI(KC.R), KC.DF('outlook'), KC.ENTER) )


KC.APP_1 = KC.LAUNCH_OUTLOOK

(Hey if you're rading this you're 2/3rds of the way in!!.Yay!!..Here's your medal🥈 1/3rd of the way to go.!)

Features:
Affordable.(Production cost goes around $40 to £55)
Simplistic.(No flashy displays like the stream deck.)
Volume encoder.(Not trying to be one of those Chinese manufacturers but yeah volume control in the stream deck costs $199 whereas, here in tamedeck it's only $40 to $60(Resale value lol)).
Customizable.(It's HIGHLY CUSTOMIZABLE....From the tactile keycaps to what the oled screen displays,the customization of the tamedeck is on another level, [you might not even need to jailbreak it] Even the code itself can be edited.)
Holes in the bottom of the deck and in the middle For cable managment and for later customization.

Smaller hardware components:
Front plate(Holding all the hardware)
Back plate(The wedge thats contains the diodes and the raspberry pi(brain).)
Diodes.
Wires:(2 of them)
Type22 AWG or 24 AWG Solid Core WireSolid core wire is rigid....you can bend it and it stays in place, making the neat, organized runs needed for the diode matrix much easier.
Two Colors:Use one color (e.g., Red) for Rows and a second color (e.g., Blue) for Columns.(The 9 key matrix).



Final connections:
Component Pin	Connects To Pico Pin	Function / Location
OLED SDA (Data)     	GP0	          I2C Display Data
OLED SCK/SCL (Clock)	GP1	          I2C Display Clock
Case Hardware holder: Holds all the stuff ie, the cherry MX switches, the oled screen,encoder etc.
Encoder CLK (A)	      GP8	          Rotation Signal A
Encoder DT (B)	      GP9	          Rotation Signal B
Encoder SW (Switch)   GP10	        Mute/Select Button
3V3 OUT           	  3V3           Powers OLED, Encoder VCC, etc.
GND (Ground Bus)	    GND    	      Common Ground Return for all components.


                    Pins 1/2       pin 3/4
                
'No'(Exit)	          GP11         GND
'Yes'(Continue)	      GP12	       GND


*all the Nine  mx keys require soldering to the diodes*
ie, 9x diodes


Here's a demo working of the curcuit:

<img width="1403" height="809" alt="Screenshot 2025-12-04 192047" src="https://github.com/user-attachments/assets/9c933e69-174d-4542-aa3c-aace5f5598f9" />

*I built this curcuit and used tactile button keys as there were no mx keys*

Total bill of items:



 
Microcontroller:  1x Raspberry Pi Pico (RP2040)                                            Main controller for HID and screen.
MX Switches:      11x MX-Style Mechanical Switch (Red/Linear)                              9 for Matrix, 2 for Action Buttons (Yes/No).
Rotary Encoder:   1x  EC11 Rotary Encoder                                                  Rotation (GP8/GP9) and Push Switch (GP10)/Controls volume
OLED Display:     1x  SSD1306 128x32                                                       Display for App Selection Menu.
Diodes:           9x  1N4148 Through-Hole Diode                                            Helps in isolating the 3x3 matrix keys.
Keycaps:          11x DSA profile, Mx compatible                                           Required for all the 11 switches.
Wiring      As needed 24 AWG Solid Core (Matrix) / 28 AWG Stranded (Peripheral)            For all hand-wired connections.
Case Files       2x  Tilted bottom case and top plate                                      Assembly uses adhesive fit only.


 GALLERY:

 
 
<img width="1644" height="849" alt="Bodacious Allis-Vihelmo (2)" src="https://github.com/user-attachments/assets/8a2f7d58-62c7-403c-8444-8bea87d741aa" />

<img width="1644" height="849" alt="Bodacious Allis-Vihelmo (4)" src="https://github.com/user-attachments/assets/57f0abce-3cb2-46ba-b8e3-875b827529f8" />

<img width="1644" height="849" alt="Bodacious Allis-Vihelmo (3)" src="https://github.com/user-attachments/assets/700dc479-37ec-40d4-aa71-7fcddc9857ba" />

<img width="1644" height="849" alt="Bodacious Allis-Vihelmo (5)" src="https://github.com/user-attachments/assets/393e6db5-c292-4d4e-8f7e-331de5d87a2c" />

![3643332-40](https://github.com/user-attachments/assets/5e213115-664c-4882-bdb8-00163f0832d1)


[all of this was taken in TINKERCAD FOR AN EASIER UNDERSTANDING]






