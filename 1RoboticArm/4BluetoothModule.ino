#define SERVICE_UUID        "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define CONTROL_UUID        "326a9001-85cb-9195-d9dd-464cfbbae75a"
#define PROGRAM_UUID        "326a9006-85cb-9195-d9dd-464cfbbae75a"
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
      
      receivedMessage = pCharacteristic->getValue();
     
      if (receivedMessage.length() > 0) {
        
        std::string stringX = receivedMessage.substr(0, 4);
        std::string stringY = receivedMessage.substr(4, 4);
        std::string stringZ = receivedMessage.substr(8, 4);
        std::string stringAngle = receivedMessage.substr(12, 4);
        std::string stringPump = receivedMessage.substr(16, 1);
        std::string stringImmediately = receivedMessage.substr(17, 1);
        std::string stringControlServos = receivedMessage.substr(18, 1);

        //Serial.printf("sX:%s sY:%s sZ:%s sAngle:%s sPump:%s", stringX.c_str(), stringY.c_str(), stringZ.c_str(), stringAngle.c_str(), stringPump.c_str());

        numberX = (float)strtol(stringX.c_str(), NULL, 16) / 10;
        numberY = (float)strtol(stringY.c_str(), NULL, 16) / 10;
        numberZ = (float)strtol(stringZ.c_str(), NULL, 16) / 10;
        numberAngle = (float)strtol(stringAngle.c_str(), NULL, 16) / 10;
        int numberPump = (int)strtol(stringPump.c_str(), NULL, 16);
        int numberImmediately = (int)strtol(stringImmediately.c_str(), NULL, 16);
        int numberControlServos = (int)strtol(stringControlServos.c_str(), NULL, 16);

        if (numberImmediately > 0) {
          movementType = remoteProgram;
        }
        else {
          movementType = remoteManual;
        }

        numbersToCurrentInput();

        switch (movementType) {
          case remoteManual:
            movementType = remoteManual;
            startManualMovement(numberX, numberY, numberZ, numberAngle);
            currentlyPumpEnabled = (numberPump > 0);
            break;
            
          case remoteProgram:
            break;


          default:
            break;  
        }


//        if (movementType == remoteProgram) {
//          if (lastBluetoothUpdate < 0) {
//              lastBluetoothUpdate = millis();
////              updateLastServoAngles();
//              return;
//            }
//          
//            long now = millis();
//            long stepDuration = now - lastBluetoothUpdate;
//          
//            if (stepDuration > 2000) {
//              lastBluetoothUpdate = millis();
////              updateLastServoAngles();
//              return;
//            }
//
//            // TODO: fix lastServoAngles etc....
////            convertCoordinatesToAngles(selectedInputX, selectedInputY, selectedInputZ, selectedInputAngle);
////            convertedToNextServoAngles();
//
//            lastBluetoothUpdate = millis();
//        }
//        else {
//          numbersToCurrentInput();
////          updateNextServoAngles(true);
//        }
        

//        if ((currentState == ST_MANUAL_MODE) || (currentState == ST_CREATE_PROGRAM)) {
//          refreshDisplay = true;
//        }
        
//        Serial.println();
      }
    }
};

//void updateLastServoAngles() {
//  convertCoordinatesToAngles(selectedInputX, selectedInputY, selectedInputZ, selectedInputAngle);
//  
//  lastServo1Angle = convertedServo1Angle;
//  lastServo2Angle = convertedServo2Angle;
//  lastServo3Angle = convertedServo3Angle;
//  lastServo4Angle = convertedServo4Angle;
//}

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


