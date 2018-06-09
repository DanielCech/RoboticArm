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

//void juggleCb();
//void imuCb();

BLECharacteristic *pCharControl;
BLECharacteristic *pCharProgram;

std::string prijataZprava;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Connected");
//      taskJuggle.restartDelayed(0);
//      taskImu.restartDelayed(0);
    };
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Disconnected");
//      taskJuggle.disable();
//      taskImu.disable();
    }
};


// třída pro příjem zprávy
class ControlCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      // načti přijatou zprávu do proměnné
      prijataZprava = pCharacteristic->getValue();
      // pokud není zpráva prázdná, vypiš její obsah
      // po znacích po sériové lince

      Serial.print("prijato");
      
      if (prijataZprava.length() > 0) {
        
        std::string stringX = prijataZprava.substr(0, 2);
        std::string stringY = prijataZprava.substr(2, 2);
        std::string stringZ = prijataZprava.substr(4, 2);
        std::string stringAngle = prijataZprava.substr(6, 2);
        std::string stringPump = prijataZprava.substr(8, 2);

        //Serial.printf("sX:%s sY:%s sZ:%s sAngle:%s sPump:%s", stringX.c_str(), stringY.c_str(), stringZ.c_str(), stringAngle.c_str(), stringPump.c_str());

        int numberX = (int)strtol(stringX.c_str(), NULL, 16);
        int numberY = (int)strtol(stringY.c_str(), NULL, 16);
        int numberZ = (int)strtol(stringZ.c_str(), NULL, 16);
        int numberAngle = (int)strtol(stringAngle.c_str(), NULL, 16);
        int numberPump = (int)strtol(stringPump.c_str(), NULL, 16);


        if ((numberX >= minX) && (numberX <= maxX)) {
          currentX = numberX;
        }

        if ((numberY >= minY) && (numberY <= maxY)) {
          currentY = numberY;
        }

        if ((numberZ >= minZ) && (numberZ <= maxZ)) {
          currentZ = numberZ;
        }

        if ((numberAngle >= minAngle) && (numberAngle <= maxAngle)) {
          currentAngle = numberAngle;
        }

        currentlyPumpEnabled = (numberPump > 0);

        refreshDisplay = true;
        
//        int numberX = (int)strtol(stringX.c_str());
//        int numberX = (int)strtol(stringX.c_str());
//        int numberX = (int)strtol(stringX.c_str());

        Serial.printf("X:%d Y:%d Z:%d Angle:%d Pump:%d", numberX, numberY, numberZ, numberAngle, numberPump); 

        Serial.print("Prijata zprava: ");
        for (int i = 0; i < prijataZprava.length(); i++) {
          Serial.print(prijataZprava[i]);
        }
        Serial.println();
        // kontrola přijaté zprávy
        // pokud obsahuje znak A, rozsviť LED diodu
//        if (prijataZprava.find("A") != -1) {
//          Serial.println("Zapnutí LED!");
//          digitalWrite(LED, HIGH);
//        }
//        // pokud obsahuje znak B, zhasni LED diodu
//        else if (prijataZprava.find("B") != -1) {
//          Serial.println("Vypnutí LED!");
//          digitalWrite(LED, LOW);
//        }
      }
    }
};

void enableBluetooth() {
  Serial.println("Starting...");
  
  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService;
  
  // Battery service
  pService = pServer->createService(BLEUUID((uint16_t)ESP_GATT_UUID_BATTERY_SERVICE_SVC));
  BLECharacteristic *pCharBat = pService->createCharacteristic(BLEUUID((uint16_t)ESP_GATT_UUID_BATTERY_LEVEL), BLECharacteristic::PROPERTY_READ);
  uint8_t level = 100; // hardcoded for now
  pCharBat->setValue(&level, 1);
  pService->start();
  
  // Control service
  pService = pServer->createService(SERVICE_UUID);
  pCharControl = pService->createCharacteristic(IMU_UUID, BLECharacteristic::PROPERTY_WRITE);
  pCharControl->setCallbacks(new ControlCallbacks());
  pCharControl->addDescriptor(new BLE2902());
  
  pCharProgram = pService->createCharacteristic(JUGGLE_UUID, BLECharacteristic::PROPERTY_WRITE);
  pCharProgram->addDescriptor(new BLE2902());
  
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

//void controlCb() {
//  uint16_t value = taskJuggle.getRunCounter();
//  pCharControl->setValue((uint8_t*)&value, sizeof(value));
//  Serial.print("Juggles: ");
//  Serial.println(value);
//}
//
//void programCb() {
//  // no real IMU connected, just randomly change some data
//  imuData.timestamp = millis();
//  float angle = taskImu.getRunCounter() / 40.f * M_PI;
//  imuData.accel.x = sinf(angle);
//  imuData.accel.y = cosf(angle);
//  imuData.mag.z = (taskImu.getRunCounter() % 40) / 20.f - 1.f;
//  pCharProgram->setValue((uint8_t*)&imuData, sizeof(IMUData));
//}

void processBluetoothLoop() {
//  scheduler.execute();
}

