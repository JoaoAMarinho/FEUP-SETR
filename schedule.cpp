#include <wiringPi.h>
#include <iostream>

// GPIO pin numbers for motor control
const int MOTOR_PIN_1 = 23;  // GPIO 17 (wiringPi pin number)
const int MOTOR_PIN_2 = 19;  // GPIO 18 (wiringPi pin number)

// Function to initialize GPIO pins
void initializePins() {
    wiringPiSetupGpio();  // Initialize wiringPi
    
    pinMode(MOTOR_PIN_1, OUTPUT);  // Set MOTOR_PIN_1 as output
    pinMode(MOTOR_PIN_2, OUTPUT);  // Set MOTOR_PIN_2 as output
}

// Function to make the car move forward
void moveForward() {
    digitalWrite(MOTOR_PIN_1, HIGH);  // Set MOTOR_PIN_1 high (logic level 1)
    digitalWrite(MOTOR_PIN_2, HIGH);   // Set MOTOR_PIN_2 low (logic level 0)
}

int main() {
    initializePins();  // Initialize GPIO pins

    moveForward();  // Move the car forward

    // Delay for a certain period of time (in milliseconds)
    delay(3000);

    // Stop the car by setting both pins low
    digitalWrite(MOTOR_PIN_1, LOW);
    digitalWrite(MOTOR_PIN_2, LOW);

    return 0;
}
