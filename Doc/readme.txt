本项目旨在构建一款 低成本、高性能的ROS智能小车，搭载 FreeRTOS 实时操作系统，实现：

✅ SLAM（即时定位与建图） —— 采用 激光雷达（LIDAR）/视觉（Camera）+ IMU 进行环境建模

✅ 自主导航 —— 基于 ROS Navigation Stack 实现路径规划（A*/DWA）

✅ 多任务实时控制 —— 使用 FreeRTOS 管理 电机、传感器、通信 任务

✅ 无线遥控 & 远程监控 —— 通过 Wi-Fi/蓝牙/4G 进行控制

✅ AI扩展能力 —— 可集成 OpenCV/YOLO 进行目标检测