

// Initialize variables
int red = 0;
int green = 0;
int blue = 0;
int direction = 1;  // 1 = incrementing, -1 = decrementing

void setup() {
  Serial.begin(9600); // Start the serial communication at 9600 baud rate
}

void loop() {
  // Send current values as a string in the format "r,g,b\n"
  Serial.print(red);
  Serial.print(",");
  Serial.print(green);
  Serial.print(",");
  Serial.println(blue);

  // Update red value
  red += direction;
  if (red > 255 || red < 0) {
    direction = -direction;  // Reverse direction when it goes beyond bounds
    red += direction;        // Adjust red value after changing direction

    // Move to the next green value
    green += direction;
    if (green > 255 || green < 0) {
      direction = -direction;  // Reverse direction when green goes out of bounds
      green += direction;      // Adjust green after direction change

      // Move to the next blue value
      blue += direction;
      if (blue > 255 || blue < 0) {
        direction = -direction;  // Reverse direction when blue goes out of bounds
        blue += direction;      // Adjust blue after direction change
      }
    }
  }
  delay(100); // Delay for readability
}