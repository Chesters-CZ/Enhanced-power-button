import datetime
import os
from win10toast import ToastNotifier
import serial
import time
import getpass
import serial.tools.list_ports
import serial.tools.list_ports
from serial import SerialException


def getTime():
    return datetime.datetime.now().strftime("%H:%M:%S")


def log(string):
    print(string)
    file = open(os.environ['USERPROFILE'] + '/enhanced-power-button-log.txt', 'a')
    file.write(string.__str__() + "\n")
    file.close()


def awaitK():
    log(getTime() + " Awaiting 'k'...")
    screenInoMsg = arduino[screenAt].readline()
    log(ports[screenAt].name + "@" + getTime() + ": " + screenInoMsg.decode('utf-8'))
    while (screenInoMsg != b"k"):
        screenInoMsg = arduino[screenAt].readline()
        log(ports[screenAt].name + "@" + getTime() + ": " + screenInoMsg.decode('utf-8'))
        time.sleep(0.25)
    log(getTime() + " Receieved 'k'")


ports = serial.tools.list_ports.comports()
file2 = open(os.environ['USERPROFILE'] + '\\enhanced-power-button-config.txt', 'r')

log("Script spuštěn v " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
log("Dostupné COM porty jsou:")

for port, desc, hwid in sorted(ports):
    log("{}: {} [{}]".format(port, desc, hwid))

arduino = []
connected = []
screenAt = -1
i = 0
for prt, desc, hwid in sorted(ports):
    log(getTime() + " Zkouším se připojit na " + prt + " (" + desc + ")")
    try:
        temp = serial.Serial(port=prt, baudrate=9600, timeout=1)

        if (arduino.__sizeof__() == 0):
            arduino = temp
        else:
            arduino.append(temp)

        connected.append(True)
    except Exception as e:
        try:
            if (arduino.__sizeof__() != 0):
                arduino.append(False)
            log(getTime() + " Nastala chyba při připojování k " + "{}: {} [{}]".format(prt, desc, hwid)
                + " : " + e.__str__())
            log("")
            connected.append(False)
        except Exception as eFatal:
            log(getTime() + " Nastala chyba při pokusu zotavit se z chyby: " + eFatal.__str__())
            log("")
            exit(-2)

i = 0
found = False
looped = 0

while (looped < 3 and not found):
    log(getTime() + " Kontroluji seznam arduin. Pokus " + looped.__str__())
    i = 0
    for a in arduino:
        log(getTime() + " Kontroluji arduino " + i.__str__())
        if (connected[i]):
            inp = a.readline()
            log(ports[i].name + "@" + getTime() + ": " + inp.decode('utf-8'))
            if (inp == b"Screen module"):
                log(getTime() + ": " + "Nalezeno arduino s obrazovkou")
                found = True
                screenAt = i
                break
            i = i + 1
        else:
            log(getTime() + " Toto arduino není připojeno.")
    if (not found):
        looped = looped + 1
        time.sleep(0.5)

if (not found):
    log(getTime() + " Žádné z detekovaných arduin není obrazovkové, vypínám se.")
    exit(0)

# v config souboru bude buď 0 pro amoogus screensaver, 1 pro hodiny, nebo 2 pro datum
match (file2.read()):
    case "0":
        log(getTime() + " Arduino bude ukazovat amoguse")
        arduino[screenAt].write(b"ua")  # use amogus
    case "1":
        log(getTime() + " Arduino bude ukazovat čas")
        arduino[screenAt].write(b"ut")  # use time
        awaitK()
        rn = datetime.datetime.now()
        log(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        arduino[screenAt].write(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        # posílá čas jako počet sekund. arduino funguje v milis, ale tutim šetřim bandwidth
    case "2":
        log(getTime() + " Arduino bude ukazovat datum")
        arduino[screenAt].write(b"ud")  # use date
        awaitK()
        rn = datetime.datetime.now()
        arduino[screenAt].write(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        log(getTime() + " Čas odeslán: " + ((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__())
        awaitK()
        arduino[screenAt].write(str.encode(rn.strftime("%Y-%m-%d")))
        log(getTime() + " Datum odesláno: " + rn.strftime("%Y-%m-%d"))

log(getTime() + " Přecházím na pasivní mód")

try:
    while True:
        i = 0
        for a in arduino:
            if (connected[i]):
                inp = a.readline()
                if (inp.__len__() > 0):
                    log(ports[i].name + "@" + getTime() + ": " + inp.decode('utf-8').replace("\n", ""))
                i = i + 1

except KeyboardInterrupt as e:
    log(getTime() + " Přijato Ctrl+C.")
    log("")
    exit(0)
except Exception as e:
    log(getTime() + " Nastala chyba: " + e.__str__())
    log("")
