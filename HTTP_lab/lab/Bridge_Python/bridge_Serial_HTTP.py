### author: Roberto Vezzani

import serial
import serial.tools.list_ports
import requests
import time
import configparser


class Bridge():

	def __init__(self):
		self.config = configparser.ConfigParser()
		self.config.read('config.ini')
		self.setupSerial()

	def setupSerial(self):
		# open serial port
		self.ser = None

		if not self.config.getboolean("Serial","UseDescription", fallback=False):
			self.portname = self.config.get("Serial","PortName", fallback="COM1")
		else:
			print("list of available ports: ")
			ports = serial.tools.list_ports.comports()

			for port in ports:
				print (port.device)
				print (port.description)
				if self.config.get("Serial","PortDescription", fallback="arduino").lower() \
						in port.description.lower():
					self.portname = port.device

		try:
			if self.portname is not None:
				print ("connecting to " + self.portname)
				self.ser = serial.Serial(self.portname, 9600, timeout=0)
		except:
			self.ser = None

		# self.ser.open() ### serial.Serial apre la connessione in automatico

		# internal input buffer from serial
		self.inbuffer = []

	def postData(self, i, val):
		if i > 0:
			return
		url = self.config.get("HTTPAIO","Url")
		feed = self.config.get("HTTPAIO","Feed1")
		headers = {'X-AIO-Key': self.config.get("HTTPAIO","X-AIO-Key") }

		url += feed +"/data"

		myobj = {'value': val}
		print ("> Sending POST to " + url)

		x = requests.post(url, data=myobj, headers=headers)
		print(x.json())


	def getData(self):
		url = self.config.get("HTTPAIO","Url")
		feed = self.config.get("HTTPAIO","Feed2")
		headers = {'X-AIO-Key': self.config.get("HTTPAIO","X-AIO-Key") }

		url += feed + "/data/last"

		print ("> Sending GET to " + url)

		x = requests.get(url, headers=headers)
		res = x.json()
		val = res.get('value', None)

		print(x.json())

		return val

	def loop(self):
		lasttime = time.time()
		
	    # infinite loop for serial managing
		while (True):
			current_time = time.time()
	        
	        #look for a byte from serial
			if not self.ser is None:
				while self.ser.in_waiting > 0:
	                # data available from the serial port
					lastchar = self.ser.read(1)
					if lastchar == b'\xfe': #EOL
	                    # Only process data if 2 seconds have passed
						if current_time - lasttime >= 2:
							print("\nValue received")
							self.useData()
	                        
	                        # Get data and send appropriate command
							val = self.getData()
							if val == '1':
								self.ser.write(b'A')
							elif val == '0':
								self.ser.write(b'S')
	                        
	                        # Update lasttime
							lasttime = current_time
	                    
						self.inbuffer = []
					else:
	                    # append
						self.inbuffer.append(lastchar)
	        
	        # If no serial data, still check if 2 seconds have passed
			elif current_time - lasttime >= 2:
				val = self.getData()
				if val == '1':
					self.ser.write(b'A')
				elif val == '0':
					self.ser.write(b'S')	
					lasttime = current_time


	def useData(self):
		# I have received a packet from the serial port. I can use it
		if len(self.inbuffer)<3:   # at least header, size, footer
			return False
		# split parts
		if self.inbuffer[0] != b'\xff':
			return False

		numval = int.from_bytes(self.inbuffer[1], byteorder='little')

		for i in range (numval):
			val = int.from_bytes(self.inbuffer[i+2], byteorder='little')
			strval = "Sensor %d: %d " % (i, val)
			print(strval)
			self.postData(i, val)

if __name__ == '__main__':
	br = Bridge()
	br.loop()

