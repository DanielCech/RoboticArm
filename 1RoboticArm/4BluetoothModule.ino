#define SERVICE_UUID        "326a9000-85cb-9195-d9dd-464cfbbae75a"
#define CONTROL_UUID        "326a9001-85cb-9195-d9dd-464cfbbae75a"
#define PROGRAM_UUID        "326a9006-85cb-9195-d9dd-464cfbbae75a"
#define DEVICE_NAME         "RoboticArm"


typedef struct {
  float x, y, z;
} Vector3f;


BLECharacteristic *pCharControl;

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
        
        std::string stringCommand = receivedMessage.substr(0, 1);
        std::string stringX = receivedMessage.substr(1, 4);
        std::string stringY = receivedMessage.substr(5, 4);
        std::string stringZ = receivedMessage.substr(9, 4);
        std::string stringAngle = receivedMessage.substr(13, 4);
        std::string stringPump = receivedMessage.substr(18, 1);
        
//        std::string stringControlServos = receivedMessage.substr(18, 1);

        //Serial.printf("sX:%s sY:%s sZ:%s sAngle:%s sPump:%s", stringX.c_str(), stringY.c_str(), stringZ.c_str(), stringAngle.c_str(), stringPump.c_str());

        int numberCommand = (int)strtol(stringCommand.c_str(), NULL, 16);
        numberX = (float)strtol(stringX.c_str(), NULL, 16) / 10;
        numberY = (float)strtol(stringY.c_str(), NULL, 16) / 10;
        numberZ = (float)strtol(stringZ.c_str(), NULL, 16) / 10;
        numberAngle = (float)strtol(stringAngle.c_str(), NULL, 16) / 10;
        int numberPump = (int)strtol(stringPump.c_str(), NULL, 16);
        
//        int numberControlServos = (int)strtol(stringControlServos.c_str(), NULL, 16);

        checkInputFloatCoordinateLimits(numberX, numberY, numberZ, numberAngle);

//        Serial.printf("nX:%.2f nY:%.2f nZ:%.2f nAngle:%.2f", numberX, numberY, numberZ, numberAngle);

//        switch (movementType) {
//          case MV_NONE:
//            Serial.println("movement: none");
//            break;
//          case MV_LOCAL_MANUAL:
//            Serial.println("movement: localManual");
//            break;
//          case MV_LOCAL_PROGRAM:
//            Serial.println("movement: localProgram");
//            break;
//          case MV_REMOTE_MANUAL:
//            Serial.println("movement: remoteManual");
//            break;
//          case MV_REMOTE_PROGRAM:
//            Serial.println("movement: remoteProgram");
//            break;
//        }

        switch (numberCommand) {
          case COMMAND_NONE:
            break;

          case COMMAND_MANUAL:
            lastMovementSource = MV_REMOTE_MANUAL;
            selectedInputXUpdate = selectedInputYUpdate = selectedInputZUpdate = selectedInputAngleUpdate = millis();
            currentlyPumpEnabled = (numberPump > 0);
            break;

          case COMMAND_MOVE:
            movementType = MV_REMOTE_PROGRAM;
            lastMovementSource = MV_REMOTE_PROGRAM;
            startManualMovement(numberX, numberY, numberZ, numberAngle, MV_TEST); 
            break;


          case COMMAND_CIRCULAR:
            break;
        }


//        
//        if (numberCommand == COMMAND_MANUAL) {
//          
//        }
//        else {
//          lastMovementSource = MV_REMOTE_PROGRAM;
//          movementType = MV_REMOTE_PROGRAM;
//          
//          if (numberCommand == COMMAND_MOVE) {
//            startManualMovement(numberX, numberY, numberZ, numberAngle, MV_TEST); 
//          }
//          
//
//          
//        }
//
//        
//        if (mmovementTypeovementType == none) {
//          if (numberImmediately > 0) {
//            movementType = remoteProgram;
//          }
//          else {
//            movementType = remoteManual;
//          }
//        }
//        else {
//          return;
//        }

//        Serial.println("Bluetooth value");

        


//        switch (movementType) {
//          case MV_REMOTE_MANUAL:
//            startManualMovement(numberX, numberY, numberZ, numberAngle, MV_REMOTE_MANUAL);
//            currentlyPumpEnabled = (numberPump > 0);
//            break;
//            
//          case MV_REMOTE_PROGRAM:
//            
//            break;
//
//
//          default:
//            break;  
//        }


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

        numbersToSelectedInput();

        if ((currentState == ST_MANUAL_MODE) || (currentState == ST_CREATE_PROGRAM)) {
          refreshDisplay = true;
        }
        
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
  pCharControl = pService->createCharacteristic(CONTROL_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pCharControl->setCallbacks(new ControlCallbacks());
  pCharControl->addDescriptor(new BLE2902());
  
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

void signalizeEndOfMovement() {
  pCharControl->setValue("Finished");
}



