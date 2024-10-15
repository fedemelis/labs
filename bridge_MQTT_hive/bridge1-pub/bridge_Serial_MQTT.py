import serial
import serial.tools.list_ports
import configparser
import paho.mqtt.client as mqtt


class Bridge:

    def __init__(self):
        # Reading the configuration
        self.config = configparser.ConfigParser()
        self.config.read('config.ini')

        # Setupping both serial comms and MQTT service
        self.setupSerial()
        self.setupMQTT()

    def setupSerial(self):
        # open serial port
        self.ser = None

        # Verifying if my configuration has UseDescription set
        if self.config.get("Serial", "UseDescription", fallback=False):
            self.port_name = self.config.get("Serial", "PortName", fallback="COM2")
        else:
            print("list of available ports: ")
            ports = serial.tools.list_ports.comports()

            for port in ports:
                print(port.device)
                print(port.description)
                if self.config.get("Serial", "PortDescription", fallback="arduino").lower() \
                        in port.description.lower():
                    self.port_name = port.device

        try:
            if self.port_name is not None:
                print("connecting to " + self.port_name)
                self.ser = serial.Serial(self.port_name, 9600, timeout=0)
        except:
            self.ser = None

        # self.ser.open()

        # internal input buffer from serial
        self.in_buffer = []

    def setupMQTT(self):
        self.clientMQTT = mqtt.Client()

        # configuring on_connect and eventually on_message function
        self.clientMQTT.on_connect = self.on_connect
        print("connecting to MQTT broker...")
        # connect method takes a server (like broker.hive.com) and a port (usually 1883)
        self.clientMQTT.connect(
            self.config.get("MQTT", "Server", fallback="localhost"),
            self.config.getint("MQTT", "Port", fallback=1883),
            60)

        # NB: both publisher and subscriber must connect to the same broker
        self.clientMQTT.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code " + str(rc))

    def loop(self):
        # infinite loop for serial managing
        while True:
            # look for a byte from serial
            if self.ser is not None:
                if self.ser.in_waiting > 0:
                    # data available from the serial port
                    last_char = self.ser.read(1)

                    if last_char == b'\xfe':  # EOL
                        print("\nValue received")
                        self.useData()
                        self.in_buffer = []
                    else:
                        # append
                        self.in_buffer.append(last_char)

    def useData(self):
        # I have received a packet from the serial port. I can use it
        if len(self.in_buffer) < 3:  # at least header, size, footer
            return False
        # split parts
        if self.in_buffer[0] != b'\xff':
            return False

        numval = int.from_bytes(self.in_buffer[1], byteorder='little')

        for i in range(numval):
            # i + 2 è l'offset per saltare l'header
            val = int.from_bytes(self.in_buffer[i + 2], byteorder='little')
            strval = "Sensor %d: %d " % (i, val)
            print(strval)
            self.clientMQTT.publish(f'RVsensor/{i}', f'{val}')


if __name__ == '__main__':
    br = Bridge()
    # fakeClient.simulate_read()
    br.loop()
