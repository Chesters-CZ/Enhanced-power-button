## Enhanced-power-button

Have you ever noticed how boring starting your computer is? You just press a button and that's it, but don't worry. I'm here to help!

# Introducing the triple Arduino setup

![image](https://user-images.githubusercontent.com/62833819/200816144-c55af122-c362-48e6-8ba1-a79fa11617ea.png)

*this is an old picture, the final build is still a work-in-progress*

### The main Arduino

The main Arduino is the screen module. It turns on the PC via a long cable connected to the motherboard's `PWR+` and `PWR-` pins. This is triggered by the user pressing a button. The two secondary Arduinos are connected to this one, so that the screen can display popups depending on the current status of the setup. It also communicates with the python script on the computer.

### The audio module

The first secondary Arduino is the audio module. This one plays a random audio file from the connected micro SD card when the PC is turned on. If there's an error during initialization, it transmits the error code via morse code through the speaker. 

### The keycard modules

The third Arduino reads an encrypted string from a MIFARE keycard, which is decrypted using a key hardcoded on the Arduino. The decrypted string is then sent to the computer as keyboard input localized for the default Czech QWERTZ keyboard layout. If the user has chosen so when writing to the keycard, the arduino can lock the connected PC using the `Win+L` button combo. 
There is also an uploader version, which encrypts a set string and writes it onto keycards. The user chooses whether to lock the PC on removal before writing to each of the keycards.

### The python script

This is a script designed to be run on OS startup. On Windows, this can be accomplished by creating a shortcut to the .py file in `C:\Users\YourName\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup`. It contacts all of the Arduinos through their respective COM ports, restarting them in the process. The script then loads a config file from the user's folder in `C:\Users\` and tells the main Arduino what to display when idle. If the user has chosen time or date to be displayed, the script also sends the current time and possibly the date. The Arduino is not able to keep time information through a reboot, only keeping track of how long it is running and even that deviates by around -120s per 24h. By refreshing the time information every time the computer is rebooted, I can ignore dealing with leap years and daylight savings time by just making the user restart their computer or relaunch the script.

#### I could only upload a compressed version of the final pdf. The original version is available on my website [here](https://chesters.cz/files/Maturitni-prace.pdf).
