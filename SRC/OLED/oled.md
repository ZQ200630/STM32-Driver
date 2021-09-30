# OLED模块
## 移植方式

- 在CMakeLists.txt里加入头文件路径
  ![img.png](img.png)
- 如果用CubeMX生成，需要配置I2C1
- 删除Src中与Inc中关于i2c.h与i2c.c
- main函数中删除MX_I2C1_Init函数调用
- \#include"oled.h"
- 调用OLED_Init();