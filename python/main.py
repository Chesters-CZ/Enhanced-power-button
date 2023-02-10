import datetime
import os
import time
import serial.tools.list_ports


def getTime():
    return datetime.datetime.now().strftime("%H:%M:%S")


def log(string):
    print(string)
    file = open(os.environ['USERPROFILE'] + '/enhanced-power-button-log.txt', 'a')
    file.write(string.__str__() + "\n")
    file.close()


def AwaitK():
    log(getTime() + " Awaiting 'k'...")
    screeninomsg = arduino[screenAt].readline()
    log(ports[screenAt].name + "@" + getTime() + ": " + screeninomsg.decode('utf-8'))
    while (screeninomsg != b"k"):
        screeninomsg = arduino[screenAt].readline()
        if (screeninomsg.__len__() > 0):
            log(ports[screenAt].name + "@" + getTime() + ": " + screeninomsg.decode('utf-8'))
    log(getTime() + " Receieved 'k'")


ports = serial.tools.list_ports.comports()
file2 = open(os.environ['USERPROFILE'] + '\\enhanced-power-button-config.txt', 'r')

log("")
log("################################")
log("Script spuštěn v " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
log("Dostupné COM porty jsou:")

for port, desc, hwid in sorted(ports):
    log("{}: {} [{}]".format(port, desc, hwid))

arduino = []
screenAt = -1
for prt, desc, hwid in sorted(ports):
    log(getTime() + " Zkouším se připojit na " + prt + " (" + desc + ")")
    try:
        temp = serial.Serial(port=prt, baudrate=9600, timeout=1)
        temp.reset_input_buffer()
        temp.reset_output_buffer()
        if (arduino.__sizeof__() == 0):
            arduino = temp
        else:
            arduino.append(temp)

    except Exception as e:
        try:
            if (arduino.__sizeof__() != 0):
                arduino.append(serial.Serial())
            log(getTime() + " Nastala chyba při připojování k " + "{}: {} [{}]".format(prt, desc, hwid)
                + " : " + e.__str__())
            log("")
        except Exception as eFatal:
            log(getTime() + " Nastala chyba při pokusu zotavit se z chyby: " + eFatal.__str__())
            log("")
            exit(-2)

found = False
looped = 0

while (looped < 10 and not found):
    log(getTime() + " Kontroluji seznam arduin. Pokus " + looped.__str__())
    i = 0
    for a in arduino:
        log(getTime() + " Kontroluji arduino na " + a.port.__str__())
        if (a.isOpen()):
            try:
                inp = a.readline()

                log(a.port.__str__() + "@" + getTime() + ": " + inp.__str__())
                if (inp.__contains__(b"Screen")):
                    log(getTime() + ": " + "Nalezeno arduino s obrazovkou")
                    found = True
                    screenAt = i
                    break
            except Exception as e:
                log(getTime() + ": Nastala chyba při hledání arduina s obrazovkou (" + e.__str__() + ")")
                print(inp)
            i = i + 1
        else:
            log(getTime() + " COM port " + a.port.__str__() + " není otevřený.")
    if (not found):
        looped = looped + 1

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
        AwaitK()
        rn = datetime.datetime.now()
        log(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        arduino[screenAt].write(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        # posílá čas jako počet sekund. arduino funguje v milis, ale tutim šetřim bandwidth
    case "2":
        log(getTime() + " Arduino bude ukazovat datum")
        arduino[screenAt].write(b"ud")  # use date
        AwaitK()
        rn = datetime.datetime.now()
        arduino[screenAt].write(str.encode(((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__()))
        log(getTime() + " Čas odeslán: " + ((rn.hour * 60 + rn.minute) * 60 + rn.second).__str__())
        AwaitK()
        arduino[screenAt].write(str.encode(rn.strftime("%Y-%m-%d")))
        log(getTime() + " Datum odesláno: " + rn.strftime("%Y-%m-%d"))

log(getTime() + " Přecházím na pasivní mód")

while (True):
    try:
        while True:
            for a in arduino:
                if (a.isOpen()):
                    try:
                        inp = a.readlines()
                    except Exception as e:
                        log(getTime() + " Nastala chyba při čtení zprávy od " + a.port.__str__() + ". Arduino pravděpodobně není připojeno. Nebudu se dále pokoušet ho kontaktovat (" + e.__str__() + ")")
                        continue

                    for line in inp:
                        decoded = ""
                        if (line.__len__() > 0):
                            try:
                                decoded = line.decode('utf-8').replace("\n", "")
                            except Exception as e:
                                log(getTime() + " Nepovedlo se dekódovat zprávu od Arduina " + a.port.__str__() + " (" + e.args.__str__() + ")")
                                decoded = line.__str__().replace("\n", "")
                            log(a.port.__str__() + "@" + getTime() + ": " + decoded)
    except KeyboardInterrupt as e:
        log(getTime() + " Přijato Ctrl+C.")
        log("")
        exit(0)
    except Exception as e:
        try:
            log(getTime() + " Nastala obecná chyba při komunikaci s " + a.port.__str__() + " (" + e.__str__() + ")")
            log("")
        except Exception as e2:
            log(getTime() + " Nastala chyba při pokusu reportovat obecnou chybu při komunikaci")
            log("Obecná chyba: (" + e.__str__() + ")")
            log("Druhá chyba: (" + e2.__str__() + ")")
            log("")
