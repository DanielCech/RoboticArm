#define SERVICE_UUID        "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define IMU_UUID            "326a9001-85cb-9195-d9dd-464cfbbae75a"
#define JUGGLE_UUID         "326a9006-85cb-9195-d9dd-464cfbbae75a"
#define DEVICE_NAME         "RoboticArm"
#define ENABLE_IMU_NOTIFY   0

typedef struct {
  float x, y, z;
} Vector3f;

typedef struct {
  uint32_t timestamp;
  Vector3f accel;
  Vector3f gyro;
  Vector3f mag;
} IMUData;

IMUData imuData = {
  .timestamp = 12345,
  .accel = {.x = 0, .y = 0, .z = 0},
  .gyro = {.x = 0.5, .y = 0, .z = 1},
  .mag = {.x = 0, .y = 0, .z = 1}
};

Scheduler scheduler;

void juggleCb();
void imuCb();

Task taskJuggle(3000, TASK_FOREVER, &juggleCb, &scheduler, false);
Task taskImu(50, TASK_FOREVER, &imuCb, &scheduler, false);

BLECharacteristic *pCharImu;
BLECharacteristic *pCharJuggle;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Connected");
      taskJuggle.restartDelayed(0);
      taskImu.restartDelayed(0);
    };
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Disconnected");
      taskJuggle.disable();
      taskImu.disable();
    }
};

void enableBluetooth() {
  Serial.begin(115200);
  Serial.println("Starting...");
  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService;
  // ----- Battery service
  pService = pServer->createService(BLEUUID((uint16_t)ESP_GATT_UUID_BATTERY_SERVICE_SVC));
  BLECharacteristic *pCharBat = pService->createCharacteristic(BLEUUID((uint16_t)ESP_GATT_UUID_BATTERY_LEVEL), BLECharacteristic::PROPERTY_READ);
  uint8_t level = 100; // hardcoded for now
  pCharBat->setValue(&level, 1);
  pService->start();
  // ----- Main service
  pService = pServer->createService(SERVICE_UUID);
  pCharImu = pService->createCharacteristic(IMU_UUID, BLECharacteristic::PROPERTY_READ
#if ENABLE_IMU_NOTIFY
             | BLECharacteristic::PROPERTY_NOTIFY
#endif
                                           );
  pCharImu->addDescriptor(new BLE2902());
  pCharJuggle = pService->createCharacteristic(JUGGLE_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pCharJuggle->addDescriptor(new BLE2902());
  pService->start();
  // ----- Advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData adv;
  adv.setName(DEVICE_NAME);
  adv.setCompleteServices(BLEUUID(SERVICE_UUID));
  pAdvertising->setAdvertisementData(adv);
  BLEAdvertisementData adv2;
  adv2.setName(DEVICE_NAME);
  //  adv.setCompleteServices(BLEUUID(SERVICE_UUID));  // uncomment this if iOS has problems discovering the service
  pAdvertising->setScanResponseData(adv2);
  pAdvertising->start();
  Serial.println("Ready");
}

void juggleCb() {
  uint16_t value = taskJuggle.getRunCounter();
  pCharJuggle->setValue((uint8_t*)&value, sizeof(value));
  pCharJuggle->notify();
  Serial.print("Juggles: ");
  Serial.println(value);
}

void imuCb() {
  // no real IMU connected, just randomly change some data
  imuData.timestamp = millis();
  float angle = taskImu.getRunCounter() / 40.f * M_PI;
  imuData.accel.x = sinf(angle);
  imuData.accel.y = cosf(angle);
  imuData.mag.z = (taskImu.getRunCounter() % 40) / 20.f - 1.f;
  pCharImu->setValue((uint8_t*)&imuData, sizeof(IMUData));
#if ENABLE_IMU_NOTIFY
  pCharImu->notify();
#endif
}

void processBluetoothLoop() {
  scheduler.execute();
}

