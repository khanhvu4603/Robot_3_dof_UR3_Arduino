// CODE NÀY FULL. CÓ CẢ HÚT NHẢ
const int stepX = 2;
const int dirX = 5;
const int stepY = 3;
const int dirY = 6;
const int stepZ = 4;  // Chân điều khiển cho động cơ Z
const int dirZ = 7;   // Chân điều khiển hướng cho động cơ Z
const int ena = 8;
const int sensorPinX = 13;
const int sensorPinY = 12;
const int sensorPinZ = 11; // Cảm biến cho động cơ Z
const int stepPerRevolution = 24000; // Số xung cho một vòng
const int namcham1 = 10;
const int namcham2 = 9;

const int BASE_HOME_COMMAND = 10000;
const int LINK1_HOME_COMMAND = 11000;
const int LINK2_HOME_COMMAND = 12000; // Lệnh home cho động cơ Z

bool motorRunningX = false; // Trạng thái động cơ X
bool motorRunningY = false; // Trạng thái động cơ Y
bool motorRunningZ = false; // Trạng thái động cơ Z
int nX = 0;
int nY = 0;
int nZ = 0; // Giá trị cho động cơ Z
int nhan1;
int nhan2;
int nhan3;
int ht1 = 0;
int ht2 = 0;
int ht3 = 0;

bool isHomeCompleted = false; // Biến kiểm tra trạng thái home đã hoàn thành

// Khai báo hàm handleMotorInput trước
void handleMotorInput(int n, bool isX, bool isZ = false);
void executeHomeCommand();
void setDirectionAndHome(int &n, int dirPin, int homeCommand, bool isX, bool isZ = false);
void processMotorAngles(int nhan1, int nhan2, int nhan3);
void hut();
void tha();

String inputData;

// Các hàm khai báo cho di chuyển
void moveToA();
void moveToB();
void moveToC();
void moveToEnd();

void setup() {
  Serial.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);
  pinMode(stepY, OUTPUT);
  pinMode(dirY, OUTPUT);
  pinMode(stepZ, OUTPUT); // Cài đặt cho động cơ Z
  pinMode(dirZ, OUTPUT);  // Cài đặt cho động cơ Z
  pinMode(namcham1, OUTPUT); // Đặt chân namcham1 (D10) là OUTPUT
  pinMode(namcham2, OUTPUT); // Đặt chân namcham2 (D9) là OUTPUT
  pinMode(sensorPinX, INPUT);
  pinMode(sensorPinY, INPUT);
  pinMode(sensorPinZ, INPUT); // Cài đặt cho cảm biến Z
  digitalWrite(ena, LOW); // Bật động cơ
}
void loop() {
  if (Serial.available() > 0) 
{
    inputData = Serial.readString();
    inputData.trim();

    if (inputData == "S" || inputData == "10000 11000 12000") 
    {
      Serial.println("Home Command Received");
      executeHomeCommand();
      Serial.println("Home Completed, waiting for next command...");
      return;
    }

    if (inputData == "H") 
    {
      hut();
      return;
    }

    if (inputData == "T") 
    {
      tha();
      return;
    }

    int firstSpace = inputData.indexOf(' ');
    int secondSpace = inputData.indexOf(' ', firstSpace + 1);
    int eleventhSpace = inputData.indexOf(' ', inputData.lastIndexOf(' ', firstSpace));

    if (inputData.startsWith("A")) 
    {
      int firstSpace = inputData.indexOf(' ');
      int secondSpace = inputData.indexOf(' ', firstSpace + 1);
      int angle1 = inputData.substring(1, firstSpace).toInt();
      int angle2 = inputData.substring(firstSpace + 1, secondSpace).toInt();
      int angle3 = inputData.substring(secondSpace + 1).toInt();

      Serial.println("Processing simple move command to angles: " + String(angle1) + " " + String(angle2) + " " + String(angle3));
      processMotorAngles(angle1, angle2, angle3);
      return;
    }

    if (inputData != "S" && inputData != "10000 11000 12000" && eleventhSpace != -1) 
    {
      Serial.println("Processing Move Commands...");

      moveToA();
      //delay(1000);
      //hut();
      moveToEnd();
      delay(1000);
      tha();
      moveToB();
      //delay(1000);
      //hut();
      moveToEnd();
      delay(1000);
      tha();
      moveToC();
      //delay(1000);
      //hut();
      moveToEnd();
      delay(1000);
      tha();
      processMotorAngles(0,0,0);
    } 
    else {
      Serial.println("Please enter 'S' or '10000 11000 12000' to home first.");
    } 
  }
}



// Hàm di chuyển đến vị trí A: lấy 3 giá trị đầu từ inputData
void moveToA() {
  int firstSpace = inputData.indexOf(' ');
  int secondSpace = inputData.indexOf(' ', firstSpace + 1);
  int thirdSpace = inputData.indexOf(' ', secondSpace + 1);

  nhan1 = inputData.substring(0, firstSpace).toInt();
  nhan2 = inputData.substring(firstSpace + 1, secondSpace).toInt();
  nhan3 = inputData.substring(secondSpace + 1, thirdSpace).toInt();

  processMotorAngles(nhan1, nhan2, nhan3);
  delay(1000);
  hut();
  processMotorAngles(nhan1, 0, nhan3);
  Serial.println("Moving to A: X=" + String(nhan1) + ", Y=" + String(nhan2) + ", Z=" + String(nhan3));
}

// Hàm di chuyển đến vị trí B: lấy 3 giá trị tiếp theo từ inputData
void moveToB() {
  int thirdSpace = inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ') + 1) + 1);
  int fourthSpace = inputData.indexOf(' ', thirdSpace + 1);
  int fifthSpace = inputData.indexOf(' ', fourthSpace + 1);
  int sixthSpace = inputData.indexOf(' ', fifthSpace + 1);

  nhan1 = inputData.substring(thirdSpace + 1, fourthSpace).toInt();
  nhan2 = inputData.substring(fourthSpace + 1, fifthSpace).toInt();
  nhan3 = inputData.substring(fifthSpace + 1, sixthSpace).toInt();

  processMotorAngles(nhan1, nhan2, nhan3);
  delay(1000);
  hut();
  processMotorAngles(nhan1, 0, nhan3);
  Serial.println("Moving to B: X=" + String(nhan1) + ", Y=" + String(nhan2) + ", Z=" + String(nhan3));
}

// Hàm di chuyển đến vị trí C: lấy 3 giá trị tiếp theo từ inputData
void moveToC() {
  int fifthSpace = inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ') + 1) + 1) + 1) +1) +1 ;
  int sixthSpace = inputData.indexOf(' ', fifthSpace + 1);
  int seventhSpace = inputData.indexOf(' ', sixthSpace + 1);
  int eighthSpace = inputData.indexOf(' ', seventhSpace + 1);
  int ninthSpace = inputData.indexOf(' ', eighthSpace + 1);

  nhan1 = inputData.substring(sixthSpace + 1, seventhSpace).toInt();
  nhan2 = inputData.substring(seventhSpace + 1, eighthSpace).toInt();
  nhan3 = inputData.substring(eighthSpace + 1, ninthSpace).toInt();

  processMotorAngles(nhan1, nhan2, nhan3);
  delay(1000);
  hut();
  processMotorAngles(nhan1, 0, nhan3);
  Serial.println("Moving to C: X=" + String(nhan1) + ", Y=" + String(nhan2) + ", Z=" + String(nhan3));
}

// Hàm di chuyển đến vị trí End: lấy 3 giá trị cuối từ inputData
void moveToEnd() {
  int seventhSpace = inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ', inputData.indexOf(' ') + 1) + 1) + 1) + 1) + 1) + 1) + 1;
  int eighthSpace = inputData.indexOf(' ', seventhSpace + 1);
  int ninthSpace = inputData.indexOf(' ', eighthSpace + 1);
  int tenthSpace = inputData.indexOf(' ', ninthSpace + 1);
  int eleventhSpace = inputData.indexOf(' ', tenthSpace + 1);

  nhan1 = inputData.substring(ninthSpace + 1, tenthSpace).toInt();
  nhan2 = inputData.substring(tenthSpace + 1, eleventhSpace).toInt();
  nhan3 = inputData.substring(eleventhSpace + 1).toInt();

  processMotorAngles(nhan1, nhan2, nhan3);
  Serial.println("Moving to End: X=" + String(nhan1) + ", Y=" + String(nhan2) + ", Z=" + String(nhan3));
}

void executeHomeCommand() {
    Serial.println("Executing Home Command");

    // Cuối cùng là home động cơ X
    setDirectionAndHome(nX, dirX, BASE_HOME_COMMAND, true);
    while (motorRunningX) delay(10);
    
    // Home động cơ Z trước
    setDirectionAndHome(nZ, dirZ, LINK2_HOME_COMMAND, false, true);
    while (motorRunningZ) delay(10);
    
    // Home động cơ Y tiếp theo
    setDirectionAndHome(nY, dirY, LINK1_HOME_COMMAND, false);
    while (motorRunningY) delay(10);
}


void setDirectionAndHome(int &n, int dirPin, int homeCommand, bool isX, bool isZ = false) {
  digitalWrite(dirPin, n > 0 ? HIGH : LOW);
  handleMotorInput(homeCommand, isX, isZ);
}

void processMotorAngles(int nhan1, int nhan2, int nhan3) {
  nZ = nhan3 - ht3;
  nY = nhan2 - ht2;
  nX = nhan1 - ht1;
  ht3 = nhan3;
  ht2 = nhan2;
  ht1 = nhan1;

  Serial.print("LINK2 ANGLE: ");
  Serial.println(nZ);
  handleMotorInput(nZ, false, true);

  while (motorRunningZ) delay(10);

  Serial.print("LINK1 ANGLE: ");
  Serial.println(nY);
  handleMotorInput(nY, false);

  while (motorRunningY) delay(10);

  Serial.print("BASE ANGLE: ");
  Serial.println(nX);
  handleMotorInput(nX, true);

  while (motorRunningX) delay(10);
}


void handleMotorInput(int n, bool isX, bool isZ = false) {
  if (isX) {
    if (n == BASE_HOME_COMMAND) {
      motorRunningX = true;
      if (motorRunningX && digitalRead(sensorPinX) == LOW) {
        AutoHome(stepX, dirX, sensorPinX);
        ht1 = 0;
      }
    } else if (n < BASE_HOME_COMMAND) {
      RunStep(n, stepX, dirX);
    }
  } else if (!isZ) {
    if (n == LINK1_HOME_COMMAND) {
      motorRunningY = true;
      if (motorRunningY && digitalRead(sensorPinY) == LOW) {
        AutoHome(stepY, dirY, sensorPinY);
        ht2 = 0;
      }
    } else if (n < LINK1_HOME_COMMAND) {
      RunStep(n, stepY, dirY);
    }
  } else {
    if (n == LINK2_HOME_COMMAND) {
      motorRunningZ = true;
      if (motorRunningZ && digitalRead(sensorPinZ) == LOW) {
        AutoHome(stepZ, dirZ, sensorPinZ);
        ht3 = 0;
      }
    } else if (n < LINK2_HOME_COMMAND) {
      RunStep(n, stepZ, dirZ);
    }
  }
}

void AutoHome(int stepPin, int dirPin, int sensorPin) {
  for (int i = 0; i < stepPerRevolution; i++) {
    stepMotor(stepPin);
    if (digitalRead(sensorPin) == HIGH) {
      if (stepPin == stepX) motorRunningX = false;
      else if (stepPin == stepY) motorRunningY = false;
      else if (stepPin == stepZ) motorRunningZ = false;
      break;
    }
  }
}

void RunStep(int n, int stepPin, int dirPin) {
  int steps = (abs(n) <= 360) ? (stepPerRevolution * float(abs(n)) / 360) : 0;
  digitalWrite(dirPin, n > 0 ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    stepMotor(stepPin);
  }
}

void stepMotor(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
}

void hut() 
{
  digitalWrite(namcham1, HIGH); // Đặt chân D10 lên mức HIGH
  digitalWrite(namcham2, LOW); // Đặt chân D9 ở mức LOW
}

void tha() 
{
  digitalWrite(namcham1, LOW);  // Đặt chân D10 xuống mức LOW
  digitalWrite(namcham2, LOW);  // Đặt chân D9 xuống mức LOW
}
