import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(11, GPIO.OUT)

p = GPIO.PWM(11, 400)
p.start(50)

time.sleep(5)

GPIO.cleanup()

