#include <ModbusMaster.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 14); // RX, TX
#define TxEnablePin 13 //pin สำหรับกำหนดว่าจะเป็นการรับหรือการส่งข้อมูล.

// instantiate ModbusMaster object
ModbusMaster node;

#define Reg_VOLT_L_N                          3035  //Voltage L-N Avg
#define Reg_VOLT_A_N                          3027  //Voltage A-N
#define Reg_VOLT_B_N                          3029  //Voltage B-N
#define Reg_VOLT_C_N                          3031  //Voltage C-N
#define Reg_VOLT_A_B                          3019  //Voltage A-B
#define Reg_VOLT_B_C                          3021  //Voltage B-C
#define Reg_VOLT_C_A                          3023  //Voltage C-A
#define Reg_VOLT_L_L                          3025  //Voltage L-L Avg
#define Reg_CURRENT_A                         2999  //Current A
#define Reg_CURRENT_B                         3001  //Current B
#define Reg_CURRENT_C                         3003  //Current C
#define Reg_CURRENT_Avg                       3009  //Current Avg
#define Reg_POWER_TOTAL                       3059  //Active Power Total
#define Reg_POWER_A                           3053  //Active Power A
#define Reg_POWER_B                           3055  //Active Power B
#define Reg_POWER_C                           3057  //Active Power C
#define Reg_ACTIVE_ENERGY                     3211  //Active Energy Delivered + Received
#define Reg_FREQUENCY                         3109  //Frequency
#define Reg_POWER_FACTOR_A                    3077  //Power Factor A
#define Reg_POWER_FACTOR_B                    3079  //Power Factor B
#define Reg_POWER_FACTOR_C                    3081  //Power Factor C
#define Reg_POWER_FACTOR_Total                3083  //Power Factor Total

void preTransmission() {
  //digitalWrite(TxEnablePin_RE, HIGH);
  //digitalWrite(TxEnablePin_DE, HIGH);
  digitalWrite(TxEnablePin, HIGH);
}

void postTransmission() {
  //digitalWrite(TxEnablePin_RE, LOW);
  //digitalWrite(TxEnablePin_DE, LOW);
  digitalWrite(TxEnablePin, LOW);
}

float HexTofloat(uint32_t x) {
  return (*(float*)&x);
}

float Read_Meter(char addr, uint16_t REG) {
  preTransmission();
  float i = 0;
  uint8_t j, result;
  uint16_t data[2];
  uint32_t value = 0;
  node.begin(addr, mySerial) ;
  result = node.readHoldingRegisters(REG, 2); //> Modbus function 0x03 Read Holding Registers
  delay(100);
  if (result == node.ku8MBSuccess) {
    for (j = 0; j < 2; j++) {
      data[j] = node.getResponseBuffer(j);
    }
    value = data[0];
    value = value << 16;
    value = value + data[1];
    i = HexTofloat(value);
    return i;
  } else {
    Serial.print("Connec modbus fail. REG >>> "); Serial.println(REG);
    delay(100);
    return 0;
  }
  postTransmission();
}
