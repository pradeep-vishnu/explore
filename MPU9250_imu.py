import FaBo9Axis_MPU9250

mpu9250 = FaBo9Axis_MPU9250.MPU9250()

while True:
           try:
                 accel = mpu9250.readAccel()
                 ax = (str(accel['x']) + "000000000000")[0:4]
                 print(ax)
                 ay = (str(accel['y']) + "000000000000")[0:4]
                 print(ay)
                 az = (str(accel['z']) + "000000000000")[0:4]
                 print(az)
                
                 gyro = mpu9250.readGyro()
                 gx = (str(gyro['x']) + "000000000000")[0:4]
                 print(gx)
                 gy = (str(gyro['y']) + "000000000000")[0:4]
                 print(gy)
                 gz = (str(gyro['z']) + "000000000000")[0:4]
                 print(gz)
                 time.sleep(0.3)

           except:
                  print("Waiting for the MPU9250 data")
