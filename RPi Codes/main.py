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

import config  # Import configuration file parameters

# Generate x values based on the defined X_RANGE in the config
x_values = list(range(0, config.X_RANGE))

# Create an array to store parameter values, initialized with zeros
parameters_values = np.zeros((config.X_RANGE, config.NUMBER_OF_VARIABLES))

# Create subplots for each parameter with specified plot size
fig, axes = plt.subplots(config.NUMBER_OF_VARIABLES, 1, figsize=config.PLOT_SIZE)
fig.subplots_adjust(hspace=0.5)

# Create scatter plots for each parameter
scatter_plots = [ax.plot(x_values, parameters_values[:, i], 'g-')[0] for i, ax in enumerate(axes)]

# Set x-axis label for each subplot
for i in range(config.NUMBER_OF_VARIABLES):
    axes[i].set_xlabel('Time')

# Set x-axis limits for each subplot
for i in range(config.NUMBER_OF_VARIABLES):
    axes[i].set_xlim(0, config.X_RANGE)

# Set y-axis label and limits for Engine Speed subplot
axes[0].set_ylabel('Engine Speed')
axes[0].set_ylim(config.ENGINE_SPEED_BOUNDS[0] - config.ENGINE_SPEED_BOUNDS[0], config.ENGINE_SPEED_BOUNDS[1] + config.ENGINE_SPEED_BOUNDS[0])

# Set y-axis label and limits for Vehicle Speed subplot
axes[1].set_ylabel('Vehicle Speed')
axes[1].set_ylim(config.VEHICLE_SPEED_BOUNDS[0] - config.VEHICLE_SPEED_BOUNDS[0], config.VEHICLE_SPEED_BOUNDS[1] + config.VEHICLE_SPEED_BOUNDS[0])

# Set y-axis label and limits for Gear subplot
axes[2].set_ylabel('Gear')
axes[2].set_ylim(config.GEAR_BOUNDS[0] - config.GEAR_BOUNDS[0], config.GEAR_BOUNDS[1] + config.GEAR_BOUNDS[0])

# Open the CSV file for writing and write headers
with open(config.CSV_FILE_PATH, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Engine Speed', 'Vehicle Speed', 'Gear'])

# Initialize serial communication
ser = serial.Serial(config.SERIAL_PORT, config.BAUDRATE)

# Function to update the plot with new data
def run_plot(frames):
    global parameters_values, ser

    # Read data from serial port
    rx_data = ser.readline().decode('utf_8', 'strict').strip()

    # Split the received data into individual values
    values = rx_data.split(',')

    # If complete data is received
    if len(values) == 3:
        print("Data received: ", rx_data)

        # Extract engine speed, vehicle speed, and gear values
        engine_speed = float(values[0])
        vehicle_speed = float(values[1])
        gear = float(values[2])

        # Write the received data to the CSV file
        with open(config.CSV_FILE_PATH, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([engine_speed, vehicle_speed, gear])

        # Append the new row to parameter values and keep only the last X_RANGE rows
        new_row = np.array([engine_speed, vehicle_speed, gear])
        parameters_values = np.vstack([parameters_values, new_row])
        parameters_values = parameters_values[-config.X_RANGE:, :]

        # Update the scatter plots with the new data
        for j, scatter_plot in enumerate(scatter_plots):
            scatter_plot.set_ydata(parameters_values[:, j])

    # If incomplete data is received
    else:
        print("Incomplete data received: ", rx_data)
        engine_speed = 0
        vehicle_speed = 0
        gear = 0

    # Return scatter plots to update the animation
    return scatter_plots

if __name__ == "__main__":
    # Create animation
    ani = FuncAnimation(fig, run_plot, frames=range(config.X_RANGE), blit=True)
    plt.show()
