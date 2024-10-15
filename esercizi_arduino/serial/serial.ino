#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

// pins for the LEDs:
const int redPin = 3;
const int greenPin = 6;
const int bluePin = 5;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");

  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() { // run over and over
  while (mySerial.available() > 0 ) {
      // look for the next valid integer in the incoming serial stream:
      int red = mySerial.parseInt();
      // do it again:
      int green = mySerial.parseInt();
      // do it again:
      int blue = mySerial.parseInt();
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);

      // fade the red, green, and blue legs of the LED:
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
  }
}