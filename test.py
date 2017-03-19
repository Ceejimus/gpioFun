import RPi.GPIO as GPIO

def main():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(18, GPIO.OUT)
    GPIO.output(18, 1)
    GPIO.cleanup()

main()
