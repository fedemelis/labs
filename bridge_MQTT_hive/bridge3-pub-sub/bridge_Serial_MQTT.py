import serial
import serial.tools.list_ports
import configparser
import paho.mqtt.client as mqtt


class Bridge():

	def __init__(self):
		self.config = configparser.ConfigParser()
		self.config.read('config.ini')
		self.setupSerial()
		self.setupMQTT()

	def setupSerial(self):
		# open serial port
		self.ser = None

		if self.config.get("Serial", "UseDescription", fallback=False):
			self.portname = self.config.get("Serial", "PortName", fallback="COM1")
		else:
			print("list of available ports: ")
			ports = serial.tools.list_ports.comports()

			for port in ports:
				print(port.device)
				print(port.description)
				if self.config.get("Serial","PortDescription", fallback="arduino").lower() \
						in port.description.lower():
					self.portname = port.device

		try:
			if self.portname is not None:
				print ("connecting to " + self.portname)
				self.ser = serial.Serial(self.portname, 9600, timeout=0)
		except:
			self.ser = None

		# self.ser.open()

		# internal input buffer from serial
		self.inbuffer = []

	def setupMQTT(self):
		self.clientMQTT = mqtt.Client()
		self.clientMQTT.on_connect = self.on_connect
		self.clientMQTT.on_message = self.on_message
		print("connecting to MQTT broker...")
		self.clientMQTT.connect(
			self.config.get("MQTT", "Server", fallback="localhost"),
			self.config.getint("MQTT", "Port", fallback=1883),
			60)

		self.clientMQTT.loop_start()



	def on_connect(self, client, userdata, flags, rc):
		print("Connected with result code " + str(rc))

		# Subscribing in on_connect() means that if we lose the connection and
		# reconnect then subscriptions will be renewed.
		self.clientMQTT.subscribe("RVsensor/0")


	# The callback for when a PUBLISH message is received from the server.
	def on_message(self, client, userdata, msg):
		print(msg.topic + " " + str(msg.payload))
		if int(msg.payload)>100:
			self.ser.write (b'A')
		else:
			self.ser.write(b'S')



	def loop(self):
		# infinite loop for serial managing
		#
		while (True):
			pass




if __name__ == '__main__':
	br=Bridge()
	br.loop()

