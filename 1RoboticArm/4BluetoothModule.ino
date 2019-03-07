#define SERVICE_UUID        "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define CONTROL_UUID            "326a9001-85cb-9195-d9dd-464cfbbae75a"
#define PROGRAM_UUID         "326a9006-85cb-9195-d9dd-464cfbbae75a"
#define DEVICE_NAME         "RoboticArm"


typedef struct {
  float x, y, z;
} Vector3f;


BLECharacteristic *pCharControl;
BLECharacteristic *pCharProgram;

std::string receivedMessage;

class MyServerCallbacks: public BLEServerCallbacks {
    
    void onConnect(BLEServer* pServer) {
      Serial.println("Connected");
    };
    
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Disconnected");
    }
};


// Message receiving
class ControlCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      // načti přijatou zprávu do proměnné
      receivedMessage = pCharacteristic->getValue();
      // pokud není zpráva prázdná, vypiš její obsah
      // po znacích po sériové lince

     
      if (receivedMessage.length() > 0) {
        
        std::string stringX = receivedMessage.substr(0, 2);
        std::string stringY = receivedMessage.substr(2, 2);
        std::string stringZ = receivedMessage.substr(4, 2);
        std::string stringAngle = receivedMessage.substr(6, 2);
        std::string stringPump = receivedMessage.substr(8, 2);

        Serial.printf("sX:%s sY:%s sZ:%s sAngle:%s sPump:%s", stringX.c_str(), stringY.c_str(), stringZ.c_str(), stringAngle.c_str(), stringPump.c_str());

        int numberX = (int)strtol(stringX.c_str(), NULL, 16);
        int numberY = (int)strtol(stringY.c_str(), NULL, 16);
        int numberZ = (int)strtol(stringZ.c_str(), NULL, 16);
        int numberAngle = (int)strtol(stringAngle.c_str(), NULL, 16);
        int numberPump = (int)strtol(stringPump.c_str(), NULL, 16);


        if ((numberX >= minInputX) && (numberX <= maxInputX)) {
          currentInputX = numberX;
        }

        if ((numberY >= minInputY) && (numberY <= maxInputY)) {
          currentInputY = numberY;
        }

        if ((numberZ >= minInputZ) && (numberZ <= maxInputZ)) {
          currentInputZ = numberZ;
        }

        if ((numberAngle >= minInputAngle) && (numberAngle <= maxInputAngle)) {
          currentInputAngle = numberAngle;
        }

        currentlyPumpEnabled = (numberPump > 0);

        updateNextServoAngles(true);

        refreshDisplay = true;
        
//        Serial.print("Received message: ");
//        for (int i = 0; i < receivedMessage.length(); i++) {
//          Serial.print(receivedMessage[i]);
//        }
        Serial.println();
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
  
  // Control characteristic
  pService = pServer->createService(SERVICE_UUID);
  pCharControl = pService->createCharacteristic(CONTROL_UUID, BLECharacteristic::PROPERTY_WRITE);
  pCharControl->setCallbacks(new ControlCallbacks());
  pCharControl->addDescriptor(new BLE2902());

  // Program characteristic
  pCharProgram = pService->createCharacteristic(PROGRAM_UUID, BLECharacteristic::PROPERTY_WRITE);
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


