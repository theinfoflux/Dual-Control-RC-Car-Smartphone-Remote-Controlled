// Arduino Bluetooth Car with Speed Control (using SoftwareSerial)
// 2 DC Motors + L298N Driver + HC-05 Bluetooth
#include <SoftwareSerial.h>
#include <IRremote.hpp>
int receiver = 3;  
// Define SoftwareSerial pins for HC-05
SoftwareSerial BT(10, 11); // TX=10, RX=11

char command[10];   // buffer for incoming command
int index = 0;      // buffer index
int speedValue = 200; // default speed (0-255)

// Motor Driver Pins
int ENA = 9;  // Enable pin for Motor A
int IN1 = 8;  // Motor A input 1
int IN2 = 7;  // Motor A input 2

int ENB = 6;  // Enable pin for Motor B
int IN3 = 5;  // Motor B input 1
int IN4 = 4;  // Motor B input 2

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);   // create instance of 'irrecv'
decode_results results;    // create instance of 'decode_results'


void setup() {
  // Motor pins as output
    Serial.begin(9600);
  Serial.println("IR Remote Car Control with Speed Adjustment (+/-)");

  irrecv.enableIRIn(); // Start the receiver

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start Serial for debugging
  Serial.begin(9600);
  // Start Bluetooth serial
  BT.begin(9600);

// stopMotors();
  Serial.println("Bluetooth Car Ready. Waiting for commands...");
}

void loop() {
  // Read incoming Bluetooth commands
if (BT.available() > 0) {
  char c = BT.read();
  Serial.println(c);
switch(c){
  case 'F':
    Serial.println("Forward");
    digitalWrite(IN1, HIGH);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  break;
   case 'B':
   Serial.println("Backward");

  digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  break;
    case 'R':
  Serial.println("Right");
        digitalWrite(IN1, HIGH);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  break;

      case 'L':
      Serial.println("Left");
  digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  break;

     case 'S':
        Serial.println("Stop");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  break;
  case'0':
  speedValue=0;
  break;
  case'1':
  speedValue=25;
  break;
  case'2':
  speedValue=50;
  break;

  case'3':
  speedValue=75;
  break;
  case'4':
  speedValue=100;
  break;
  case'5':
  speedValue=125;
  break;
    case'6':
  speedValue=150;
  break;
  case'7':
  speedValue=175;
  break;

   case'8':
  speedValue=200;
  break;
    case'9':
  speedValue=255;
  break;
  
  }
}
 if (irrecv.decode(&results)) {
    Serial.print("IR Code: ");
    Serial.println(results.value, HEX);

    bool moveCommand = true; // To check if direction button was pressed

    switch (results.value) {
      case 0x807F02FD:  // Forward
        Serial.println("Forward");
         digitalWrite(IN1, HIGH);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
        setMotorSpeed(speedValue);
        break;
      case 0x807F22DD:  // Backward
        Serial.println("Backward");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        setMotorSpeed(speedValue);
        break;
      case 0x807F12ED:  // Right
        Serial.println("Right");
         digitalWrite(IN1, HIGH);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
        setMotorSpeed(speedValue);
        break;
      case 0x807F827D:  // Left
        Serial.println("Left");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        setMotorSpeed(speedValue);
        break;

      // ---- Speed Adjustment ----
      case 0x807FA05F:  // "+" button
        speedValue += 25;
        if (speedValue > 255) speedValue = 255;
        Serial.print("Speed Increased: ");
        Serial.println(speedValue);
        moveCommand = false; // speed change, not movement
        break;

      case 0x807F609F:  // "-" button
        speedValue -= 25;
        if (speedValue < 0) speedValue = 0;
        Serial.print("Speed Decreased: ");
        Serial.println(speedValue);
        moveCommand = false; // speed change, not movement
        break;

      default:
        moveCommand = false;
        break;
    }

    // If it was a move command → stop motors after button release
    if (moveCommand) {
      delay(200); // give a short move pulse
      stopMotors();
    }

    irrecv.resume(); // Receive the next value
  }

}
// Function to stop motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Function to update speed
void setMotorSpeed(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}
