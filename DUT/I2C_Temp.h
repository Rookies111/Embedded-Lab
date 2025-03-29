#ifndef I2C_Temp_H
#define I2C_Temp_H
#include <Arduino.h>

class I2C_Temp{
  private:
    int SDA;
    int SCL;
  
  public:
    I2C_Temp(int SDA, int SCL);
    void init();
    float readTemp();
};
#endif