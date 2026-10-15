//        SMART IRRIGATION SYSTEM

// -------- Water Level LEDs --------
const int waterLED1 = 2;
const int waterLED2 = 3;
const int waterLED3 = 4;
const int waterLED4 = 5;
const int waterLED5 = 6;

// -------- Ultrasonic Sensor --------
const int trigPin = 7;
const int echoPin = 8;

// -------- Pump Relay --------
const int relayPin = 9;

// -------- Buzzer --------
const int buzzer = 10;

// -------- Pump Status LED --------
const int pumpLED = 11;

// -------- Soil Status LEDs --------
const int redLED = 12;
const int greenLED = 13;

// -------- Soil Moisture Sensor --------
const int soilPin = A0;

// -------- Soil Moisture Threshold --------
const int soilThreshold = 500;


//                 SETUP


void setup() {

  // Water LEDs
  pinMode(waterLED1, OUTPUT);
  pinMode(waterLED2, OUTPUT);
  pinMode(waterLED3, OUTPUT);
  pinMode(waterLED4, OUTPUT);
  pinMode(waterLED5, OUTPUT);

  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Relay
  pinMode(relayPin, OUTPUT);

  // Buzzer
  pinMode(buzzer, OUTPUT);

  // Pump LED
  pinMode(pumpLED, OUTPUT);

  // Soil LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Soil Sensor
  pinMode(soilPin, INPUT);

  // Serial Monitor
  Serial.begin(9600);

  // Start with pump OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(pumpLED, LOW);
}


//                  LOOP

void loop() {
  

  // 1. READ ULTRASONIC SENSOR

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.0343 / 2;


  // 2. CALCULATE WATER LEVEL

  int waterLevel = map(distance, 20, 0, 0, 100);

  waterLevel = constrain(waterLevel, 0, 100);


  // 3. WATER LEVEL LEDs

  digitalWrite(waterLED1, waterLevel >= 20);
  digitalWrite(waterLED2, waterLevel >= 40);
  digitalWrite(waterLED3, waterLevel >= 60);
  digitalWrite(waterLED4, waterLevel >= 80);
  digitalWrite(waterLED5, waterLevel >= 90);


  // 4. READ SOIL MOISTURE

  int soilValue = analogRead(soilPin);

  bool soilDry = soilValue < soilThreshold;


  // 5. SOIL STATUS LEDs

  if (soilDry) {

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

  } else {

    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }


  // 6. LOW WATER WARNING

  bool lowWater = waterLevel <= 20;

  if (lowWater) {

    tone(buzzer, 1000);

  } else {

    noTone(buzzer);
  }


  // 7. PUMP CONTROL

  bool pumpON = soilDry && waterLevel > 20;

  if (pumpON) {

    digitalWrite(relayPin, HIGH);
    digitalWrite(pumpLED, HIGH);

  } else {

    digitalWrite(relayPin, LOW);
    digitalWrite(pumpLED, LOW);
  }


  // 8. SERIAL MONITOR

  Serial.print("Distance: ");
  Serial.print(distance);

  Serial.print(" cm | Water: ");
  Serial.print(waterLevel);

  Serial.print("% | Soil: ");
  Serial.print(soilValue);

  Serial.print(" | Soil Status: ");

  if (soilDry) {
    Serial.print("DRY");
  } else {
    Serial.print("WET");
  }

  Serial.print(" | Pump: ");

  if (pumpON) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }


  delay(500);
}
