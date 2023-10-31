#include <SPI.h>
#include <mcp2515.h>
#define slave 0x0D4

// struct can_frame recvMsg;
struct can_frame canMsg;
const int SPI_CS_PIN = 9;
MCP2515 mcp2515(SPI_CS_PIN); // Set CS pin

// Declare variables

const int trig = 5;  
const int echo = 6; 
float duration, distance;
int dist1, dist2;

void setup() {
    canMsg.can_id = slave;
    canMsg.can_dlc = 8;
    canMsg.data[0]= 0x00;
    canMsg.data[1]= 0x00;
    canMsg.data[2]= 0x00;
    canMsg.data[3]= 0x00;
    canMsg.data[4]= 0x00;
    canMsg.data[5]= 0x00;
    canMsg.data[6]= 0x00;
    canMsg.data[7]= 0x00;
    while(!Serial){};

    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);  //header file change
    mcp2515.setNormalMode();
    Serial.println("CAN BUS Shield init ok!");
    
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    Serial.begin(115200);
}

void loop() {
    // Initialize module
    digitalWrite(trig, LOW);  
    delayMicroseconds(2);
    
    // Run trig 
    digitalWrite(trig, HIGH);  
    delayMicroseconds(10);  
    digitalWrite(trig, LOW);  
  
    //Run echo 
    duration = pulseIn(echo, HIGH);
    distance = (duration * .034)/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(100);
    
    dist1 = distance;
    dist2 = round((dist1 - distance) * 100);
    canMsg.data[0] = dist1 / 256;
    canMsg.data[1] = dist1 % 256;
    canMsg.data[2] = dist2;
    
    mcp2515.sendMessage(&canMsg);
    Serial.println("Success");
}
