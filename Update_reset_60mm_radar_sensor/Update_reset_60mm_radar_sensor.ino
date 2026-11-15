#include <DFRobot_HumanDetection.h>

DFRobot_HumanDetection hu(&Serial1);

unsigned long ghostDataTimer = 0;
bool isTimerStarted = false;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  Serial.println("Start initialization...");
  while (hu.begin() != 0) {
    Serial.println("init error!!!");
    delay(1000);
  }

  while (hu.configWorkMode(hu.eSleepMode) != 0) {
    Serial.println("Mode switch error!!!");
    delay(1000);
  }

  hu.configLEDLight(hu.eHPLed, 1);
  hu.sensorRet(); // সেন্সর মোডিউল বুট রিসেট
  delay(3000);
}

void loop() {
  uint8_t presence = hu.smHumanData(hu.eHumanPresence);
  uint8_t movement = hu.smHumanData(hu.eHumanMovement);
  uint8_t range    = hu.smHumanData(hu.eHumanMovingRange);
  
  uint8_t heartRate = hu.getHeartRate();
  uint8_t breathe   = hu.getBreatheValue();

  Serial.printf("Presence: %d | Motion: %d | Range: %d\n", presence, movement, range);
  Serial.printf("Heart Rate: %d | Breathe: %d\n", heartRate, breathe);

  // -------------------------------------------------------------
  // সমাধান লজিক: 
  // যদি Presence = 0 হয় (কেউ নেই) অথবা Movement = 0 এবং Range খুব কম হয়
  // -------------------------------------------------------------
  if (presence == 0 || (movement == 0 && range <= 1)) {
    if (!isTimerStarted) {
      ghostDataTimer = millis();
      isTimerStarted = true;
    } 
    // মানুষ চলে যাওয়ার পর ৫ সেকেন্ড ফ্যান্টম ডাটা স্থায়ী হলেই রিসেট মারবে
    else if (millis() - ghostDataTimer > 5000) { 
      Serial.println("\n[WARNING] Ghost Vital Data Frozen! Resetting Sensor Hardware...");
      
      hu.sensorRet();  // সেন্সরের ইন্টারনাল মেমোরি/বাফার ফ্রেশ করা
      delay(2500);     // র্যাডার পুনরায় সিগন্যাল সেটেল করার টাইম
      
      isTimerStarted = false;
    }
  } else {
    // সত্যি মানুষ থাকলে এবং নড়াচড়া করলে টাইমার রিসেট
    isTimerStarted = false; 
  }

  Serial.println("----------------------------------");
  delay(1000);
}