# main.py

# Team 1
# Humberto Barrera
# Catalina Muñoz
# Mauricio Zavala
# Erick Campos

# Libraries
import numpy as np
import csv
import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading
import queue

import config  # Import configuration file parameters

# Create a queue to hold incoming data
data_queue = queue.Queue()

# Generate x values based on the defined X_RANGE in the config
x_values = list(range(0, config.xRange))

# Create an array to store parameter values, initialized with zeros
parameters_values = np.zeros((config.xRange, config.nVariables))

# Create subplots for each parameter with specified plot size
fig, axes = plt.subplots(config.nVariables, 1, figsize=config.plotSize)
fig.subplots_adjust(hspace=0.5)

# Create scatter plots for each parameter
scatter_plots = [ax.plot(x_values, parameters_values[:, i], 'g-')[0] for i, ax in enumerate(axes)]

# Set x-axis label for each subplot
for i in range(config.nVariables):
    axes[i].set_xlabel('Time')

# Set x-axis limits for each subplot
for i in range(config.nVariables):
    axes[i].set_xlim(0, config.xRange)

# Set y-axis label and limits for Engine Speed subplot
axes[0].set_ylabel('Engine Speed')
axes[0].set_ylim(config.engSpeed[0], config.engSpeed[1])

# Set y-axis label and limits for Tractor Speed subplot
axes[1].set_ylabel('Tractor Speed')
axes[1].set_ylim(config.tractSpeed[0], config.tractSpeed[1])

# Set y-axis label and limits for Gear subplot
axes[2].set_ylabel('Gear')
axes[2].set_ylim(config.gears[0], config.gears[1])

# Function to read from the serial port and write to the CSV file
def read_serial_data():
    ser = serial.Serial(config.sPort, config.baudrate)
    
    with open(config.csvPath, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Engine Speed', 'Vehicle Speed', 'Gear'])
        
        while True:
            rx_data = ser.readline().decode('utf-8', 'strict').strip()
            values = rx_data.split(',')
            
            if len(values) == 3:
                try:
                    engine_speed = float(values[0])
                    vehicle_speed = float(values[1])
                    gear = float(values[2])
                    data_queue.put((engine_speed, vehicle_speed, gear))
                    
                    writer.writerow([engine_speed, vehicle_speed, gear])
                    file.flush()  # Ensure data is written to the file immediately
                except ValueError:
                    continue

# Function to update the plot with new data
def run_plot(frames):
    global parameters_values
    
    while not data_queue.empty():
        engine_speed, vehicle_speed, gear = data_queue.get()
        
        new_row = np.array([engine_speed, vehicle_speed, gear])
        parameters_values = np.vstack([parameters_values, new_row])
        parameters_values = parameters_values[-config.xRange:, :]
        
        for j, scatter_plot in enumerate(scatter_plots):
            scatter_plot.set_ydata(parameters_values[:, j])
    
    return scatter_plots

if __name__ == "__main__":
    # Start the serial data reading thread
    thread = threading.Thread(target=read_serial_data)
    thread.daemon = True
    thread.start()
    
    # Create animation
    ani = FuncAnimation(fig, run_plot, frames=range(config.xRange), blit=True)
    plt.show()
