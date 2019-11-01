from os import system
from serial import Serial

system("sudo rfcomm release 0")
system("sudo rfcomm bind 0 00:18:E4:34:D4:DE 1")
system("sudo chmod 666 /dev/rfcomm0")

b = Serial('/dev/rfcomm0')

print("Bluetooth Connected!")

print(b.read_all())