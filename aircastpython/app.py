import serial
import matplotlib.pyplot as plt
import numpy as np

ser = serial.Serial('/dev/cu.usbmodem2101', 9600)  

# initialize NumPy arrays 
time_points = np.array([])
gas_levels = np.array([])
temperatures = np.array([])
humidities = np.array([])

plt.ion()  


try:
    while True:
        # read data from Arduino
        data = ser.readline().decode().strip()
        print(data)
        
        # split data into individual sets of values
        data_sets = data.split('\n')
        
        for data_set in data_sets:
            if data_set:
                parts = data_set.split(',')
                # parse received data
                gas_level = int(parts[0].split(':')[1])
                quality = (parts[1].split(':')[1])
                temperature = float(parts[2].split(':')[1])
                humidity = float(parts[3].split(':')[1])
                
                # append data to NumPy arrays
                time_points = np.append(time_points, len(time_points) + 1)
                gas_levels = np.append(gas_levels, gas_level)
                temperatures = np.append(temperatures, temperature)
                humidities = np.append(humidities, humidity)
        
        # clear previous plot and update with new data
        plt.clf()
        plt.suptitle('AirCast Data Tracker', fontweight='bold')

        plt.subplot(2, 1, 1)
        if gas_level > 225:
            plt.plot(time_points, gas_levels, label='Gas Level', color='red')
        elif gas_level > 181:
            plt.plot(time_points, gas_levels, label='Gas Level', color='orange')
        else:
            plt.plot(time_points, gas_levels, label='Gas Level', color='green')

        plt.xlabel('Time')
        plt.ylabel('Gas Level')
        plt.legend()
        
        plt.subplot(2, 1, 2)
        if temperature > 35:
            plt.plot(time_points, temperatures, label='Temperature', color='red')
        elif temperature > 26:
            plt.plot(time_points, temperatures, label='Temperature', color='orange')
        else:
            plt.plot(time_points, temperatures, label='Temperature', color='green')

        plt.xlabel('Time')
        plt.ylabel('Temperature')
        plt.legend()
        
        plt.tight_layout()
        plt.pause(1) 


except KeyboardInterrupt:
    ser.close()
    plt.ioff()
    plt.show()
