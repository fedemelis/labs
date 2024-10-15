import random

import serial
import time


def simulate_read():
    # Configura la porta seriale
    ser = serial.Serial('COM4', 9600, timeout=1)
    print("connecting as COM4")

    # Attendi che la connessione si stabilisca
    time.sleep(2)

    rnd = random.Random()

    while True:
        # Costruisci un pacchetto (ad esempio, 0xFF 0x02 0x10 0x20 0xFE)
        header = b'\xff'
        num_values = b'\x02'
        value1 = rnd.randint(1, 20).to_bytes(1)
        value2 = rnd.randint(1, 20).to_bytes(1)
        footer = b'\xfe'

        # Invia il pacchetto
        packet = header + num_values + value1 + value2 + footer
        ser.write(packet)

        print("Pacchetto inviato:", packet)
        time.sleep(3)

    # Chiudi la porta
    ser.close()

if __name__ == '__main__':
	simulate_read()
