#include "Wi_Fi.h"
#include "read485.h"
#include "oled.h"
#include <Ticker.h>  //Ticker Library

Ticker Tick10s;
boolean read_meter = true;
int id[3], countID = 0, No_ID = 0;

//----------- Variable Meter ---------------------//
float volts_1[3], volts_2[3], volts_3[3], volt_avg[3];
float amp_1[3], amp_2[3], amp_3[3], amp_avg[3];
float power_1[3], power_2[3], power_3[3], power_total[3];
float energy[3], fq[3];
void setup() {
  Serial.begin(9600);
  pinMode(TxEnablePin, OUTPUT);
  digitalWrite(TxEnablePin, LOW); // Init in receive mode

  configwifi();
  setoled();
  mySerial.begin(9600); // Modbus communication runs at 9600 baud

  if (sensor_1 != "-") {
    id[0] = sensor_1.toInt();
    No_ID++;
  }
  if (sensor_2 != "-") {
    id[1] = sensor_2.toInt();
    No_ID++;
  }
  if (sensor_3 != "-") {
    id[2] = sensor_3.toInt();
    No_ID++;
  }

  Serial.println(F("RS485 Modbus RTU Test Schneider EasyLogic PM2130"));

  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Tick10s.attach(10, tick10s); //Interrupt every 30sec
}

void loop() {
  Wifi_re();
  if (read_meter == true) {
    GET_METER(id[countID]);
    display_data(id[countID], volt_avg[countID], amp_avg[countID], power_total[countID], energy[countID]);
    countID++;
    if (countID > No_ID) countID = 0;
    read_meter = false;
  }
  delay(50);
}

void GET_METER(int ID) {
  delay(10);
  volts_1[ID] = Read_Meter(ID, Reg_VOLT_A_B);
  volts_2[ID] = Read_Meter(ID, Reg_VOLT_B_C);
  volts_3[ID] = Read_Meter(ID, Reg_VOLT_C_A);
  volt_avg[ID] = Read_Meter(ID, Reg_VOLT_L_L);

  amp_1[ID] = Read_Meter(ID, Reg_CURRENT_A);
  amp_2[ID] = Read_Meter(ID, Reg_CURRENT_B);
  amp_3[ID] = Read_Meter(ID, Reg_CURRENT_C);
  amp_avg[ID] = Read_Meter(ID, Reg_CURRENT_Avg);

  power_1[ID] = Read_Meter(ID, Reg_POWER_A);
  power_2[ID] = Read_Meter(ID, Reg_POWER_B);
  power_3[ID] = Read_Meter(ID, Reg_POWER_C);
  power_total[ID] = Read_Meter(ID, Reg_POWER_TOTAL);

  fq[ID] = Read_Meter(ID, Reg_FREQUENCY);
  energy[ID] = Read_Meter(ID, Reg_ACTIVE_ENERGY);
}

void tick10s() {
  read_meter = true;
}
