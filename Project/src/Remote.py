import RPi.GPIO as GPIO
import time

class Remote(object):
	
	def __init__(self):

		self.IR = 17
		self.PWM = 50

		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(self.IR,GPIO.IN)

	def getkey(self):
		if GPIO.input(self.IR) == 0:
			count = 0
			while GPIO.input(self.IR) == 0 and count < 200:  #9ms
				count += 1
				time.sleep(0.00006)
			if(count < 10):
				return
			count = 0
			while GPIO.input(self.IR) == 1 and count < 80:  #4.5ms
				count += 1
				time.sleep(0.00006)

			idx = 0
			cnt = 0
			data = [0,0,0,0]
			for i in range(0,32):
				count = 0
				while GPIO.input(self.IR) == 0 and count < 15:    #0.56ms
					count += 1
					time.sleep(0.00006)
					
				count = 0
				while GPIO.input(self.IR) == 1 and count < 40:   #0: 0.56mx
					count += 1                               #1: 1.69ms
					time.sleep(0.00006)
					
				if count > 7:
					data[idx] |= 1<<cnt
				if cnt == 7:
					cnt = 0
					idx += 1
				else:
					cnt += 1
			if data[0]+data[1] == 0xFF and data[2]+data[3] == 0xFF:  #check
				return data[2]
			else:
				return "repeat"

	def parseKey(self):
		key = self.getkey()
		if key == 0x18:
			return "forward"
		if key == 0x08:
			return "left"
		if key == 0x1c:
			return "stop"
		if key == 0x5a:
			return "right"
		if key == 0x52:
			return "backward"
		if key == 0x15:
			key = 0
			if(self.PWM + 10 < 101):
				self.PWM = self.PWM + 10
				Ab.setPWMA(self.PWM)
				Ab.setPWMB(self.PWM)
				print(self.PWM, " cmd")
		if key == 0x07:
			key = 0
			if(self.PWM - 10 > -1):
				self.PWM = self.PWM - 10
				self.setPWMA(self.PWM)
				self.setPWMB(self.PWM)
				print(PWM, " cmd")
		return "None"
	
	def remoteCommand(self):
		for i in range(50000):
			cmd = self.parseKey()
			if cmd != None:
				return cmd
			

