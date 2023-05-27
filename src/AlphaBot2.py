import RPi.GPIO as GPIO
import time

class AlphaBot2(object):
	
	def __init__(self,ain1=12,ain2=13,ena=6,bin1=20,bin2=21,enb=26):
		self.AIN1 = ain1
		self.AIN2 = ain2
		self.BIN1 = bin1
		self.BIN2 = bin2
		self.ENA = ena
		self.ENB = enb
		self.PA  = 50
		self.PB  = 50
		self.IR = 17
		self.PWM = 50

		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(23,GPIO.IN)
		GPIO.setup(self.AIN1,GPIO.OUT)
		GPIO.setup(self.AIN2,GPIO.OUT)
		GPIO.setup(self.BIN1,GPIO.OUT)
		GPIO.setup(self.BIN2,GPIO.OUT)
		GPIO.setup(self.ENA,GPIO.OUT)
		GPIO.setup(self.ENB,GPIO.OUT)
		GPIO.setup(self.IR,GPIO.IN)
		self.PWMA = GPIO.PWM(self.ENA,500)
		self.PWMB = GPIO.PWM(self.ENB,500)
		self.PWMA.start(self.PA)
		self.PWMB.start(self.PB)
		self.stop()

	def forward(self):
		self.PWMA.ChangeDutyCycle(self.PA)
		self.PWMB.ChangeDutyCycle(self.PB)
		GPIO.output(self.AIN1,GPIO.LOW)
		GPIO.output(self.AIN2,GPIO.HIGH)
		GPIO.output(self.BIN1,GPIO.LOW)
		GPIO.output(self.BIN2,GPIO.HIGH)

	def stop(self):
		self.PWMA.ChangeDutyCycle(0)
		self.PWMB.ChangeDutyCycle(0)
		GPIO.output(self.AIN1,GPIO.LOW)
		GPIO.output(self.AIN2,GPIO.LOW)
		GPIO.output(self.BIN1,GPIO.LOW)
		GPIO.output(self.BIN2,GPIO.LOW)

	def backward(self):
		self.PWMA.ChangeDutyCycle(self.PA)
		self.PWMB.ChangeDutyCycle(self.PB)
		GPIO.output(self.AIN1,GPIO.HIGH)
		GPIO.output(self.AIN2,GPIO.LOW)
		GPIO.output(self.BIN1,GPIO.HIGH)
		GPIO.output(self.BIN2,GPIO.LOW)

		
	def left(self):
		self.PWMA.ChangeDutyCycle(30)
		self.PWMB.ChangeDutyCycle(30)
		GPIO.output(self.AIN1,GPIO.HIGH)
		GPIO.output(self.AIN2,GPIO.LOW)
		GPIO.output(self.BIN1,GPIO.LOW)
		GPIO.output(self.BIN2,GPIO.HIGH)


	def right(self):
		self.PWMA.ChangeDutyCycle(30)
		self.PWMB.ChangeDutyCycle(30)
		GPIO.output(self.AIN1,GPIO.LOW)
		GPIO.output(self.AIN2,GPIO.HIGH)
		GPIO.output(self.BIN1,GPIO.HIGH)
		GPIO.output(self.BIN2,GPIO.LOW)
		
	def setPWMA(self,value):
		self.PA = value
		self.PWMA.ChangeDutyCycle(self.PA)

	def setPWMB(self,value):
		self.PB = value
		self.PWMB.ChangeDutyCycle(self.PB)	
		
	def setMotor(self, left, right):
		if((right >= 0) and (right <= 100)):
			GPIO.output(self.AIN1,GPIO.HIGH)
			GPIO.output(self.AIN2,GPIO.LOW)
			self.PWMA.ChangeDutyCycle(right)
		elif((right < 0) and (right >= -100)):
			GPIO.output(self.AIN1,GPIO.LOW)
			GPIO.output(self.AIN2,GPIO.HIGH)
			self.PWMA.ChangeDutyCycle(0 - right)
		if((left >= 0) and (left <= 100)):
			GPIO.output(self.BIN1,GPIO.HIGH)
			GPIO.output(self.BIN2,GPIO.LOW)
			self.PWMB.ChangeDutyCycle(left)
		elif((left < 0) and (left >= -100)):
			GPIO.output(self.BIN1,GPIO.LOW)
			GPIO.output(self.BIN2,GPIO.HIGH)
			self.PWMB.ChangeDutyCycle(0 - left)

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
			# Ab.forward()
			return "forward"
		if key == 0x08:
			# Ab.left()
			return "left"
		if key == 0x1c:
			# Ab.stop()
			return "stop"
		if key == 0x5a:
			# Ab.right()
			return "right"
		if key == 0x52:
			# Ab.backward()		
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
		return None
	
	def remoteCommand(self):
		for i in range(50000):
			cmd = self.parseKey()
			if cmd != None:
				return cmd
			

if __name__=='__main__':

	Ab = AlphaBot2()
	try:
		while True:
			Ab.remoteCommand()
	except KeyboardInterrupt:
		GPIO.cleanup()
