import RPi.GPIO as GPIO
import time

class Sensor(object):
	def __init__(self):
		self.DR = 16
		self.DL = 19

		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(self.DR,GPIO.IN,GPIO.PUD_UP)
		GPIO.setup(self.DL,GPIO.IN,GPIO.PUD_UP)
	
	def sensorCommand(self):
		for i in range(500):
			DR_status = GPIO.input(self.DR)
			DL_status = GPIO.input(self.DL)

			if ((DL_status == 0) or (DR_status == 0)):
				return "stop"
		return None
