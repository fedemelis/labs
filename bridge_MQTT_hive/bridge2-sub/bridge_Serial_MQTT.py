import configparser
import paho.mqtt.client as mqtt


class Bridge:

	def __init__(self):
		self.config = configparser.ConfigParser()
		self.config.read('config.ini')

		# Setupping MQTT service
		self.setupMQTT()


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
		self.clientMQTT.subscribe("RVsensor/+")


	# The callback for when a PUBLISH message is received from the server.
	def on_message(self, client, userdata, msg):
		print(msg.topic + " " + str(msg.payload))

	def loop(self):
		# infinite loop for serial managing
		#
		while (True):
			pass

if __name__ == '__main__':
	br = Bridge()
	br.loop()


'''
we do not find there a serial port configuration, because
our goal here is to build a client that recive messages
through MQTT's broker
'''