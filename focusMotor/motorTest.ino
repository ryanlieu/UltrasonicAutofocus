#include <Servo.h> // built in Arduino servo library
#include <NewPing.h> // custom library built by Tim Eckel built for better ultrasonic reading http://playground.arduino.cc/Code/NewPing

Servo focusServo;             // servo for focusing
int focusPos = 0;             // focus position angle
const int numReadings = 10;   // number of readings to take for average
int index = 0;                // current reading index
int total = 0;                // total of all readings for average calcualation
int average = 0;              // average over numReadings
int echoPin = 11;             // sensor echo pin
int initPin = 12;             // sensor init pin
int maxDistance=200;          // maximum sensor reading
int distance = 0;             // distance of sensor
long toTurn = 0;              // angle to turn, calculated from curve fit
int sensorMin = 8;            // closest necessary sensor distance
int sensorMax = 35;           // furthest possible sensor distance

NewPing sonar(initPin, echoPin, maxDistance); 
 
// initialization
void setup() {
  // sets up servo at pin 9
  focusServo.attach(9);
  // spins the motor to the closest focal length
  focusServo.write(180);
  // serial port only works at 115200 for some reason
  Serial.begin(115200);
} 
 
// loop code
void loop() {
    // time inbetween sensor readings
    delay(10);            
    // pings sensor
    distance=sonar.ping_cm();        
    Serial.print("Ping: ");
    Serial.print(distance); 
    Serial.println("cm");

    // sums up distance to total
    total=total+distance;
    // increase index by one
    index=index+1;

    // once numReadings amount of pings has been reached, turn motor. Prevents over-sensitive motor from twitching
    if (index >= numReadings)  {  
      // average taken                             
      average=total/numReadings;
      Serial.println(average);
      index = 0;
      total = 0;
      // checks if sensor is within necessary range
      if(average>=sensorMin&&average<=sensorMax){
        // magic numbers here needed. This equation to calculate relationship between distance and angle was done through
        // taking a few data points and creating a quadratic fit. This quadratic fit was the formula for the angle to turn.
        toTurn=.0557143*average*average-4.66143*average+213.083;
        Serial.println(toTurn);
        focusServo.write(toTurn);
      }
    }
}
