# lCD模块
## 移植步骤

- 在CMakeLists.txt里加入头文件路径
  ![img_1.png](img_1.png)
- 如果用CubeMX生成，需要配置FSMC
![img.png](img.png)
- 删除Src中与Inc中关于fsmc.h与fsmc.c
- main函数中删除MX_FSMC_Init函数调用
- \#include"lcd.h"
- 调用LCD_Init();