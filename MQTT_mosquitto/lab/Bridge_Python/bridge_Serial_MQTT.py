### author: Roberto Vezzani

import serial
import serial.tools.list_ports

import configparser

import paho.mqtt.client as mqtt


class Bridge():

    def __init__(self):
        self.config = configparser.ConfigParser()
        self.config.read('config.ini')

        #sensor
        self.pubtopic = self.config.get("MQTT", "PubTopic", fallback="sensor")

        self.setupSerial()
        self.setupMQTT()

    def setupSerial(self):
        # open serial port
        self.ser = None

        if not self.config.getboolean("Serial", "UseDescription", fallback=False):
            self.portname = self.config.get("Serial", "PortName", fallback="COM1")
        else:
            print("list of available ports: ")
            ports = serial.tools.list_ports.comports()

            for port in ports:
                print(port.device)
                print(port.description)
                if self.config.get("Serial", "PortDescription", fallback="arduino").lower() \
                        in port.description.lower():
                    self.portname = port.device

        try:
            if self.portname is not None:
                print("connecting to " + self.portname)
                self.ser = serial.Serial(self.portname, 9600, timeout=0)
        except:
            self.ser = None
            print("Cannot connect to " + self.portname)

        # self.ser.open()

        # internal input buffer from serial
        self.inbuffer = []

    def setupMQTT(self):
        self.clientMQTT = mqtt.Client()
        self.clientMQTT.on_connect = self.on_connect
        self.clientMQTT.on_message = self.on_message
        print("Connecting to MQTT broker...")
        self.clientMQTT.connect(
            self.config.get("MQTT", "Server", fallback="localhost"),
            self.config.getint("MQTT", "Port", fallback=1883),
            60)

        self.clientMQTT.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code " + str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        t = self.config.get("MQTT", "SubTopic", fallback="mylight")

        self.clientMQTT.subscribe(t)
        print("Subscribed to " + t)

    # on_message is triggered when somthing is published on a subscribed topic
    # this is the part that realize the serial communication
    def on_message(self, client, userdata, msg):
        print(msg.topic + " " + str(msg.payload))
        if msg.topic == self.config.get("MQTT", "SubTopic", fallback="mylight"):
            if self.ser is not None:    
                self.ser.write(msg.payload)
    def loop(self):
        # infinite loop for serial managing
        #
        while True:
            # this code handle the possible serial message from arduino to the bridge
            if self.ser is not None:
                if self.ser.in_waiting > 0:
                    # data available from the serial port
                    lastchar = self.ser.read(1)

                    if lastchar == b'\xfe':  # EOL
                        print("\nValue received")
                        self.useData()
                        self.inbuffer = []
                    else:
                        # append
                        self.inbuffer.append(lastchar)

    # useData is a definition of the publisher role of the bridge
    def useData(self):
        # I have received a packet from the serial port. I can use it
        if len(self.inbuffer) < 3:  # at least header, size, footer
            return False
        # split parts
        if self.inbuffer[0] != b'\xff':
            return False

        numval = int.from_bytes(self.inbuffer[1], byteorder='little')

        for i in range(numval):
            val = int.from_bytes(self.inbuffer[i + 2], byteorder='little')
            strval = "Sensor %d: %d " % (i, val)
            print(strval)
            # publish requires 2 parameters, topic and value
            print("postato")
            self.clientMQTT.publish(self.config.get("MQTT", "PubTopic", fallback="mylight") + '/{:d}'.format(i),
                                    '{:d}'.format(val))


if __name__ == '__main__':
    br = Bridge()
    br.loop()
