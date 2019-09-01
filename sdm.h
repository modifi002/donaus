#define Total_of_Reg 14

#define Reg_LN                3035  //Voltage L-N Avg
#define Reg_FQ                3109  //Frequency
#define Reg_AN                3027  //Voltage A-N
#define Reg_BN                3029  //Voltage B-N
#define Reg_CN                3031  //Voltage C-N
#define Reg_AB                3019  //Voltage A-B
#define Reg_BC                3021  //Voltage B-C
#define Reg_CA                3023  //Voltage C-A
#define Reg_LL                3025  //Voltage L-L Avg
#define Reg_AA                2999  //Current A
#define Reg_BA                3001  //Current B
#define Reg_CA                3003  //Current C
#define Reg_A_Avg             3009  //Current Avg
#define Reg_POWER_TOTAL       3059  //Active Power Total
#define Reg_PA                3053  //Active Power A
#define Reg_PB                3055  //Active Power B
#define Reg_PC                3057  //Active Power C
#define Reg_CE                3211  //Active Energy Delivered + Received

uint16_t Regs[14]={
  Reg_LN,
  Reg_FQ,
  Reg_AN,
  Reg_BN,
  Reg_CN,
  Reg_AA,
  Reg_BA,
  Reg_CA,
  Reg_AA,
  Reg_P,
  Reg_PA,
  Reg_PB,
  Reg_PC,
  Reg_CE
};

float DATA_METER[Total_of_Reg];
