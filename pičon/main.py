import datetime
import os
from win10toast import ToastNotifier
import serial
import time
import getpass
import serial.tools.list_ports
import serial.tools.list_ports


def getTime():
    return datetime.datetime.now().strftime("%H:%M:%S")


def log(string):
    file = open(os.environ['USERPROFILE'] + '/enhanced-power-button-log.txt', 'a')
    file.write(string + "\n")
    file.close()


def awaitK():
    time.sleep(0.5)
    inp = arduino[screenAt].readLine()
    log(ports[screenAt].port + "@" + getTime() + ": " + inp.decode('utf-8'))
    while (inp != b"k"):
        time.sleep(0.25)
        inp = arduino[screenAt].readLine()
        log(ports[screenAt].port + "@" + getTime() + ": " + inp.decode('utf-8'))


time.sleep(10)
toast = ToastNotifier()
ports = serial.tools.list_ports.comports()

log("Script spuštěn v " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
log("Dostupné COM porty jsou:")

for port, desc, hwid in sorted(ports):
    log("{}: {} [{}]".format(port, desc, hwid))

arduino = []
connected = []
screenAt = -1
i = 0
for port, desc, hwid in sorted(ports):
    try:
        arduino.append = serial.Serial(port=port, baudrate=9600, timeout=1)
        connected.append(True)
    except:
        log(getTime() + "Nastala chyba při připojování k " + "{}: {} [{}]".format(port, desc, hwid))
        connected.append(False)

i = 0
found = False
looped = 0
while (looped < 3 and not found):
    for a in arduino:
        inp = a.readLine()
        log(ports[i].port + "@" + getTime() + ": " + inp.decode('utf-8'))
        if (inp == b"Screen module"):
            log(getTime() + ": " + "Nalezeno arduino s obrazovkou")
            found = True
            screenAt = i
        i = i + 1
    looped = looped + 1

file2 = open(os.environ['USERPROFILE'] + '/enhanced-power-button-config.txt', 'r')
# v config souboru bude buď 0 pro amoogus screensaver, 1 pro hodiny, nebo 2 pro datum
match (file2):
    case 0:
        arduino[screenAt].write(b"ua")  # use amogus
    case 1:
        arduino[screenAt].write(b"ut")  # use time
        awaitK()
        rn = datetime.datetime.now()
        arduino[screenAt].write("" + ((rn.hour * 60 + rn.minute) * 60 + rn.second)) # posílá čas jako počet sekund
                                                                                    # arduino funguje v milis, ale tutim
                                                                                    # šetřim bandwidth
    case 2:
        arduino[screenAt].write(b"ud")  # use date
        awaitK()
        rn = datetime.datetime.now()
        arduino[screenAt].write("" + ((rn.hour * 60 + rn.minute) * 60 + rn.second))
        awaitK()
        arduino[screenAt].write("" + rn.strftime("%Y-%m-%d"))

while True:
    i = 0
    for a in arduino:
        inp = a.readline()
        print(inp)
        log(ports[i].port + "@" + getTime() + ": " + inp.decode('utf-8'))
        i = i + 1
