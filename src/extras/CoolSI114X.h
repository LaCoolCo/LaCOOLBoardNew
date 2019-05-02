#ifndef _CoolSI114X_H_
#define _CoolSI114X_H_
#include "Arduino.h"
/*------------------------------------------------------// 
Registers,Parameters and commands

*/
//
//commands
//
#define CoolSI114X_QUERY 0X80
#define CoolSI114X_SET 0XA0
#define CoolSI114X_NOP 0X0
#define CoolSI114X_RESET    0X01
#define CoolSI114X_BUSADDR    0X02
#define CoolSI114X_PS_FORCE    0X05
#define CoolSI114X_GET_CAL    0X12
#define CoolSI114X_ALS_FORCE    0X06
#define CoolSI114X_PSALS_FORCE    0X07
#define CoolSI114X_PS_PAUSE    0X09
#define CoolSI114X_ALS_PAUSE    0X0A
#define CoolSI114X_PSALS_PAUSE    0XB
#define CoolSI114X_PS_AUTO    0X0D
#define CoolSI114X_ALS_AUTO   0X0E
#define CoolSI114X_PSALS_AUTO 0X0F
//
//IIC REGISTERS
//
#define CoolSI114X_PART_ID  0X00
#define CoolSI114X_REV_ID  0X01
#define CoolSI114X_SEQ_ID  0X02
#define CoolSI114X_INT_CFG  0X03
#define CoolSI114X_IRQ_ENABLE  0X04
#define CoolSI114X_IRQ_MODE1 0x05
#define CoolSI114X_IRQ_MODE2 0x06
#define CoolSI114X_HW_KEY  0X07
#define CoolSI114X_MEAS_RATE0 0X08
#define CoolSI114X_MEAS_RATE1  0X09
#define CoolSI114X_PS_RATE  0X0A
#define CoolSI114X_PS_LED21  0X0F
#define CoolSI114X_PS_LED3  0X10
#define CoolSI114X_UCOEFF0  0X13
#define CoolSI114X_UCOEFF1  0X14
#define CoolSI114X_UCOEFF2  0X15
#define CoolSI114X_UCOEFF3  0X16
#define CoolSI114X_WR  0X17
#define CoolSI114X_COMMAND  0X18
#define CoolSI114X_RESPONSE  0X20
#define CoolSI114X_IRQ_STATUS  0X21
#define CoolSI114X_ALS_VIS_DATA0 0X22
#define CoolSI114X_ALS_VIS_DATA1 0X23
#define CoolSI114X_ALS_IR_DATA0 0X24
#define CoolSI114X_ALS_IR_DATA1 0X25
#define CoolSI114X_PS1_DATA0 0X26
#define CoolSI114X_PS1_DATA1 0X27
#define CoolSI114X_PS2_DATA0 0X28
#define CoolSI114X_PS2_DATA1 0X29
#define CoolSI114X_PS3_DATA0 0X2A
#define CoolSI114X_PS3_DATA1 0X2B
#define CoolSI114X_AUX_DATA0_UVINDEX0 0X2C
#define CoolSI114X_AUX_DATA1_UVINDEX1 0X2D
#define CoolSI114X_RD 0X2E
#define CoolSI114X_CHIP_STAT 0X30
//
//Parameters
//
#define CoolSI114X_I2C_ADDR 0X00

#define CoolSI114X_CHLIST   0X01
#define CoolSI114X_CHLIST_ENUV 0x80
#define CoolSI114X_CHLIST_ENAUX 0x40
#define CoolSI114X_CHLIST_ENALSIR 0x20
#define CoolSI114X_CHLIST_ENALSVIS 0x10
#define CoolSI114X_CHLIST_ENPS1 0x01
#define CoolSI114X_CHLIST_ENPS2 0x02
#define CoolSI114X_CHLIST_ENPS3 0x04

#define CoolSI114X_PSLED12_SELECT   0X02
#define CoolSI114X_PSLED3_SELECT   0X03

#define CoolSI114X_PS_ENCODE   0X05
#define CoolSI114X_ALS_ENCODE  0X06

#define CoolSI114X_PS1_ADCMUX   0X07
#define CoolSI114X_PS2_ADCMUX   0X08
#define CoolSI114X_PS3_ADCMUX   0X09

#define CoolSI114X_PS_ADC_COUNTER   0X0A
#define CoolSI114X_PS_ADC_GAIN 0X0B
#define CoolSI114X_PS_ADC_MISC 0X0C

#define CoolSI114X_ALS_IR_ADC_MUX   0X0E
#define CoolSI114X_AUX_ADC_MUX   0X0F

#define CoolSI114X_ALS_VIS_ADC_COUNTER   0X10
#define CoolSI114X_ALS_VIS_ADC_GAIN 0X11
#define CoolSI114X_ALS_VIS_ADC_MISC 0X12

#define CoolSI114X_LED_REC 0X1C

#define CoolSI114X_ALS_IR_ADC_COUNTER   0X1D
#define CoolSI114X_ALS_IR_ADC_GAIN 0X1E
#define CoolSI114X_ALS_IR_ADC_MISC 0X1F
//
//USER SETTINGS DEFINE
//
//ADCMUX
#define CoolSI114X_ADCMUX_SMALL_IR  0x00
#define CoolSI114X_ADCMUX_VISIABLE 0x02
#define CoolSI114X_ADCMUX_LARGE_IR  0x03
#define CoolSI114X_ADCMUX_NO  0x06
#define CoolSI114X_ADCMUX_GND  0x25
#define CoolSI114X_ADCMUX_TEMPERATURE  0x65
#define CoolSI114X_ADCMUX_VDD  0x75
//LED SEL
#define CoolSI114X_PSLED12_SELECT_PS1_NONE 0x00
#define CoolSI114X_PSLED12_SELECT_PS1_LED1 0x01
#define CoolSI114X_PSLED12_SELECT_PS1_LED2 0x02
#define CoolSI114X_PSLED12_SELECT_PS1_LED3 0x04
#define CoolSI114X_PSLED12_SELECT_PS2_NONE 0x00
#define CoolSI114X_PSLED12_SELECT_PS2_LED1 0x10
#define CoolSI114X_PSLED12_SELECT_PS2_LED2 0x20
#define CoolSI114X_PSLED12_SELECT_PS2_LED3 0x40
#define CoolSI114X_PSLED3_SELECT_PS2_NONE 0x00
#define CoolSI114X_PSLED3_SELECT_PS2_LED1 0x10
#define CoolSI114X_PSLED3_SELECT_PS2_LED2 0x20
#define CoolSI114X_PSLED3_SELECT_PS2_LED3 0x40
//ADC GAIN DIV
#define CoolSI114X_ADC_GAIN_DIV1 0X00
#define CoolSI114X_ADC_GAIN_DIV2 0X01
#define CoolSI114X_ADC_GAIN_DIV4 0X02
#define CoolSI114X_ADC_GAIN_DIV8 0X03
#define CoolSI114X_ADC_GAIN_DIV16 0X04
#define CoolSI114X_ADC_GAIN_DIV32 0X05
//LED CURRENT
#define CoolSI114X_LED_CURRENT_5MA 0X01
#define CoolSI114X_LED_CURRENT_11MA 0X02
#define CoolSI114X_LED_CURRENT_22MA 0X03
#define CoolSI114X_LED_CURRENT_45MA 0X04
//Recovery period the  ADC takes before making a PS measurement
#define CoolSI114X_ADC_COUNTER_1ADCCLK 0X00
#define CoolSI114X_ADC_COUNTER_7ADCCLK 0X01
#define CoolSI114X_ADC_COUNTER_15ADCCLK 0X02
#define CoolSI114X_ADC_COUNTER_31ADCCLK 0X03
#define CoolSI114X_ADC_COUNTER_63ADCCLK 0X04
#define CoolSI114X_ADC_COUNTER_127ADCCLK 0X05
#define CoolSI114X_ADC_COUNTER_255ADCCLK 0X06
#define CoolSI114X_ADC_COUNTER_511ADCCLK 0X07
//ADC MISC
#define CoolSI114X_ADC_MISC_LOWRANGE 0X00
#define CoolSI114X_ADC_MISC_HIGHRANGE 0X20
#define CoolSI114X_ADC_MISC_ADC_NORMALPROXIMITY 0X00
#define CoolSI114X_ADC_MISC_ADC_RAWADC 0X04
//INT OE
#define CoolSI114X_INT_CFG_INTOE 0X01
//IRQ ENABLE
#define CoolSI114X_IRQEN_ALS 0x01
#define CoolSI114X_IRQEN_PS1 0x04
#define CoolSI114X_IRQEN_PS2 0x08
#define CoolSI114X_IRQEN_PS3 0x10

#define CoolSI114X_ADDR 0x60

//OVERFLOW VALUES
#define CoolSI114X_VIS_OVERFLOW 0x8C
#define CoolSI114X_IR_OVERFLOW 0x8D
#define CoolSI114X_UV_OVERFLOW 0x8E


class CoolSI114X {
 public:
  bool Begin(void);
  void Reset(void);
  void DeInit(void);
  uint8_t  ReadParamData(uint8_t Reg);
  uint8_t  WriteParamData(uint8_t Reg,uint8_t Value);
  uint16_t ReadVisible(void);
  uint16_t ReadIR(void);
  uint16_t ReadProximity(uint8_t PSn);
  uint16_t ReadUV(void);
  uint8_t  ReadResponseReg(void);

 private:
  void  WriteByte(uint8_t Reg, uint8_t Value);
  uint8_t  ReadByte(uint8_t Reg);
  uint16_t ReadHalfWord(uint8_t Reg);
};



#endif




