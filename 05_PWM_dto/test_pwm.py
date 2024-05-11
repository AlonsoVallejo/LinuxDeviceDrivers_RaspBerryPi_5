from gpiozero import PWMLED
from time import sleep

led = PWMLED(12)

while True:
    try:
        for b in range(101):
            led.value = b / 100.0
            sleep(0.01)
    except KeyboardInterrupt:
        print("The Program is terminated manually!")
        raise SystemExit