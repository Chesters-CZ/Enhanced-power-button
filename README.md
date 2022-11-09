## Enhanced-power-button
Have you ever noticed how boring starting your computer is? You just press a button and that's it, but don't worry. I'm here to help!

# Introducing the triple Arduino setup

![image](https://user-images.githubusercontent.com/62833819/200816144-c55af122-c362-48e6-8ba1-a79fa11617ea.png)
*the third arduino has yet to be added*

The main Arduino is the screen module. It turns on the pc via a long cable connected to the motherboard's PWR+ and PWR- pins. This is triggered by the user pressing a button. The two secondary Arduinos are connected to this one, so that the screen can display popups depending on the current status of the setup. It also communicates with a python script on the computer, which is designed to be run on startup.

The first secondary Arduino is the audio module. This one plays a random audio file from the connected micro SD card when the PC is turned on.

The third arduino contains an encrypted password, which is decrypted using the key stored on a keycard. The decrypted password is then sent to the computer as keyboard input.
