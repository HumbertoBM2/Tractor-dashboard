# Library imports

import numpy as np
import csv
import serial

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Code imports

import plot_settings

# List and matrix for live plotting

x_values = list(range(0, plot_settings.X_RANGE))
parameters_values = np.zeros((plot_settings.X_RANGE, plot_settings.NUMBER_OF_VARIABLES))

# Figure and axes declaration

fig, axes = plt.subplots(plot_settings.NUMBER_OF_VARIABLES, 1, figsize=plot_settings.PLOT_SIZE)
fig.subplots_adjust(hspace=0.5)
scatter_plots = [ax.plot(x_values, parameters_values[:, i], 'g-')[0] for i, ax in enumerate(axes)]

for i in range(plot_settings.NUMBER_OF_VARIABLES):
  axes[i].set_xlabel('Time')

for i in range(plot_settings.NUMBER_OF_VARIABLES):
  axes[i].set_xlim(0, plot_settings.X_RANGE)

axes[0].set_ylabel('Engine Speed')
axes[0].set_ylim(plot_settings.ENGINE_SPEED_BOUNDS[0] - plot_settings.ENGINE_SPEED_BOUNDS[0], plot_settings.ENGINE_SPEED_BOUNDS[1] + plot_settings.ENGINE_SPEED_BOUNDS[0])

axes[1].set_ylabel('Vehicle Speed')
axes[1].set_ylim(plot_settings.VEHICLE_SPEED_BOUNDS[0] - plot_settings.VEHICLE_SPEED_BOUNDS[0], plot_settings.VEHICLE_SPEED_BOUNDS[1] + plot_settings.VEHICLE_SPEED_BOUNDS[0])

axes[2].set_ylabel('Gear')
axes[2].set_ylim(plot_settings.GEAR_BOUNDS[0] - plot_settings.GEAR_BOUNDS[0], plot_settings.GEAR_BOUNDS[1] + plot_settings.GEAR_BOUNDS[0])

# Create CSV file

with open(plot_settings.CSV_FILE_PATH, mode = 'w', newline = '') as file:
    writer = csv.writer(file)
    writer.writerow(['Engine Speed', 'Vehicle Speed', 'Gear'])

# Instantiate Serial object

ser = serial.Serial(plot_settings.SERIAL_PORT, plot_settings.BAUDRATE)

def run_plot(frame):

  global parameters_values, ser

  # Read values, decode bytes to string and remove leading/trailing whitespaces

  rx_data = ser.readline().decode('utf_8', 'strict').strip()
  
  # Split the received string by commas

  values = rx_data.split(',')

  # Check if all values are received

  if len(values) == 3:
      
    print("Data received: ", rx_data)

    # Assign values to variables

    engine_speed = float(values[0])
    vehicle_speed = float(values[1])
    gear = float(values[2])

    # Write values to CSV

    with open(plot_settings.CSV_FILE_PATH, mode = 'a', newline = '') as file:
      writer = csv.writer(file)
      writer.writerow([engine_speed, vehicle_speed, gear])

    # Add new values to parameters values matrix

    new_row = np.array([engine_speed, vehicle_speed, gear])
    parameters_values = np.vstack([parameters_values, new_row])

    # Limit parameters values matrix to set number of items

    parameters_values = parameters_values[-plot_settings.X_RANGE : , :]

    # Update lines with new values

    for j, scatter_plot in enumerate(scatter_plots):
      scatter_plot.set_ydata(parameters_values[:, j])

  else:
      
    # Handle the case when all values are not received properly

    print("Incomplete data received: ", rx_data)

    engine_speed = 0
    vehicle_speed = 0
    gear = 0

  return scatter_plots

if __name__ == "__main__":
  ani = FuncAnimation(fig, run_plot, frames=range(plot_settings.X_RANGE), blit=True)
  plt.show()
