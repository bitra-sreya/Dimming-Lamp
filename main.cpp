//Arduino code

#define echoPin 2
#define trigPin 3

long duration; 
int distance; 

int analogI1 = A0;
int analogRef = A1;
int analogPot = A5;

const int LED_PIN = 11;

int DUTY = 10;

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode( LED_PIN, OUTPUT );
  Serial.begin(9600);

}

void ultrasonic() {
  digitalWrite(trigPin, LOW); // Clears the trigPin condition
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin HIGH for 10 us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Returns travel time in us
  
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

float lightLevel;

void ldr() {
  int analogL1 = analogRead(analogI1) - analogRead(analogRef);
  float voltage1 = analogL1 * (5.0/1023.0);
  lightLevel = (voltage1 - 2.4)/-.0813;
  //Serial.println(voltage1);
  Serial.print("lightLevel: ");
  Serial.println(lightLevel);
}

float voltageP;
double shiftP;

void potentiometer() {
  analogPot = analogRead(analogPot);
  voltageP = analogPot * (5.0/1023.0);
  //Serial.print("Shift voltage:");
  //Serial.println(voltageP);
  shiftP = (double)voltageP;
}

double shift;
double shiftPrev;

void loop() {

  ultrasonic();
  
  ldr();

  potentiometer();

  if (shiftP < 3 && shiftP > 2){
    shift = shiftPrev;
  } else if (shiftP > 3 || shiftP < 2) {
    shift = shiftP;
    shiftPrev = shiftP;
  }
  
  /**
  Serial.print(", ");
  Serial.print("Shift:");
  Serial.println(shift);
  */

  if (lightLevel < (10 + shift)) {
    if (DUTY == 255) {
      DUTY = 255;
    } else {
      DUTY = DUTY + 1;
    }
  } else if (lightLevel > (10 + shift)) {
    if (DUTY == 0) {
      DUTY = 0;
    } else {
      DUTY = DUTY - 1;
    }
  } else {
    DUTY = DUTY;
  }

  if (distance < 6) {
    DUTY = DUTY + 3;
  }
  
  
  analogWrite( LED_PIN, DUTY );

  delay(500);
}
