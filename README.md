# 基于51单片机的简单蓝牙模拟家具控制
## 项目介绍
51单片机上连接了3个器件来分别模拟3个家具，通过蓝牙与手机连接，可以在手机上控制3个家具的状态。此外还准备了一块TFT屏幕用于实时显示着各个家具的状态（开与关）

![8b3fd958d1939e082ac09bc708e25c7](https://github.com/zxcvbnmkj/simulation-furniture-control-based-on-51-microcontroller/assets/133875139/6ebe6d03-9c9a-400e-b625-bc783f7ad222)


除了可以通过手机控制之外，还可以通过3个独立按键来分别控制3个家具。3个家具分别是电灯，门和窗帘。其中电灯的开关通过51单片机开发板上的LED小灯的亮灭来模拟，门的开关使用舵机来模拟，窗帘的拉上与拉开通过步进电机的转动来模拟，步进电机顺时针旋转几圈后停止代表着窗帘被拉上了，逆时针旋转代表窗帘拉开。
## 控制说明
### 蓝牙控制
打开“汇承BLE串口助手”小程序和手机蓝牙，然后连接”HC-08”，再勾选HEX发送（HEX接收不要勾选）。发送“01”的话，LED小灯会点亮，如果再发送一次“01”小灯就熄灭。发送“02”的话，舵机会旋转90度。发送“03”的话，步进电机旋转。如果发送了其他的数据，小程序的接收区会收到一个“Error!”。

![image](https://github.com/zxcvbnmkj/simulation-furniture-control-based-on-51-microcontroller/assets/133875139/f23cee12-e1a2-46d8-b744-543c1ef428b2)

![image](https://github.com/zxcvbnmkj/simulation-furniture-control-based-on-51-microcontroller/assets/133875139/94bda205-518a-48b5-aa51-d220341a3bdc)

### 按键控制
开发板上的3个独立按键k8,k7,k6控制3个家具，第一个按键k8控制LED小灯的亮灭；第二个按键k7控制舵机；第三个按键k6控制步进电机，按第一下顺时针转，模拟窗帘开，按第二下逆时针旋转，模拟窗帘关。

![image](https://github.com/zxcvbnmkj/simulation-furniture-control-based-on-51-microcontroller/assets/133875139/e74e8d22-8897-4dc7-9b24-1c610cd8feb0)
