The project involves creating a mobile robot control system that collects data on various environmental parameters and performs actions based on received commands. It is designed using various technologies, including Bluetooth communication, sensor measurements such as accelerometer, distance sensor, and light sensor, as well as motor control for robot movement.

The system is based on the MKL05Z4 microcontroller, which communicates with external devices via I2C and UART interfaces. The device is equipped with sensors to measure lighting, temperature, and accelerations in three axes. Additionally, the robot can respond to incoming Bluetooth commands, such as moving forward, backward, left, or right, at a specified distance it must cover.

Bluetooth communication is used to transmit data between the robot and the controlling device. The robot receives data packets containing information about rotation angle, throttle, brake, direction, and tracking mode. Based on this information, the robot adjusts its speed and direction of movement and collects data from various sensors such as acceleration, light intensity, and distance. It sends this data back to the controlling device, enabling real-time monitoring of the robot's state.

The project consists of several key functions, including recognizing commands entered by the user. These commands consist of letters (f, b, r, l) and digits that specify the direction of movement and the distance the robot must travel. The system can recognize these commands and then perform the corresponding movements. For example, the command "f50" means moving forward 50 cm, "r30" means turning right by 30 cm, and "b100" means moving backward by 100 cm. The commands are checked for correctness, and if an error occurs, the system notifies the user about the incorrect format.

Sensors such as the accelerometer enable the measurement of acceleration in three axes, allowing the evaluation of the robot's movement in space. By using appropriate conversion factors, the data from the sensors is transformed into appropriate units such as gravity or light intensity in lux. Additionally, data from the sensors is sent to the controlling device, which can analyze and display it in real-time.

The robot uses motors that are controlled based on the received commands. The motors are controlled by output pins of the microcontroller, which regulate the wheel rotation speed. This allows the robot to perform precise movements in different directions, both in a straight line and during turns. Functions such as setting the direction and speed of the motors are executed by sending appropriate commands to the microcontroller.

The system also enables testing of individual sensors, such as the distance sensor, which measures the distance to obstacles, and the light sensor, which allows the evaluation of the light level in the robot's surroundings. Sensor testing is performed using appropriate functions that collect data and send it to the controlling system.

Additionally, the project uses a packet structure for transmitting data between the robot and the controlling device. These packets contain information about accelerations in three axes, light intensity, and the distance the robot measured during its movement. This structure allows for efficient data transmission and analysis by the controlling system.

The entire system is designed to allow the robot to autonomously move in space, collect data, and respond to user commands. By using the microcontroller, Bluetooth interface, and advanced sensors, the robot can perform a variety of tasks, such as measuring distance, acceleration, light intensity, and reacting to user commands in real-time.
![Zrzut ekranu (141)](https://github.com/BezStresu/INSrover/assets/102317065/24700725-4f8e-46e9-b417-800f2ce4f75d)
