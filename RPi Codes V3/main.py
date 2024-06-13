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
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.animation import FuncAnimation
import threading
import queue
import tkinter as tk

import config  # Import configuration file parameters

# Create a queue to hold incoming data
data_queue = queue.Queue()

# Generate x values based on the defined X_RANGE in the config
x_values = list(range(0, config.xRange))

breakGUI = 0
customThrottle = 0
button_width = 15
button_height = 1
colors = ['blue', 'red', 'green', 'orange']

# Create an array to store parameter values, initialized with zeros
parameters_values = np.zeros((config.xRange, config.nVariables))

# Create subplots for each parameter with specified plot size
fig, axes = plt.subplots(config.nVariables, 1, figsize=config.plotSize)
fig.subplots_adjust(hspace=0.5)

# Create scatter plots for each parameter
scatter_plots = [ax.plot(x_values, parameters_values[:, i], color=colors[i])[0] for i, ax in enumerate(axes)]

# Set x-axis label for each subplot
for i in range(config.nVariables):
    axes[i].set_xlabel('Time')

# Set x-axis limits for each subplot
for i in range(config.nVariables):
    axes[i].set_xlim(0, config.xRange)

# Set y-axis label and limits for Engine Speed subplot
axes[0].set_ylabel('Engine Speed (RPM)')
axes[0].set_ylim(config.engSpeed[0], config.engSpeed[1])

# Set y-axis label and limits for Tractor Speed subplot
axes[1].set_ylabel('Tractor Speed (km/h)')
axes[1].set_ylim(config.tractSpeed[0], config.tractSpeed[1])

# Set y-axis label and limits for Gear subplot
axes[2].set_ylabel('Gear')
axes[2].set_ylim(config.gears[0], config.gears[1])

# Set y-axis label and limits for Throttle subplot
axes[3].set_ylabel('Throttle')
axes[3].set_ylim(config.throttle[0], config.throttle[1])

# Function to read from the serial port and write to the CSV file
def read_serial_data():
    global ser
    ser = serial.Serial(config.sPort, config.baudrate)
    
    with open(config.csvPath, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Engine Speed', 'Vehicle Speed', 'Gear', 'Throttle'])
        
        while True:
            rx_data = ser.readline().decode('utf-8', 'strict').strip()
            values = rx_data.split(',')
            
            if len(values) == 4:
                try:
                    engine_speed = float(values[0])
                    vehicle_speed = float(values[1])
                    gear = float(values[2])
                    throttle = float(values[3])
                    data_queue.put((engine_speed, vehicle_speed, gear, throttle))
                    
                    writer.writerow([engine_speed, vehicle_speed, gear, throttle])
                    file.flush()  # Ensure data is written to the file immediately
                except ValueError:
                    continue

# Function to update the plot with new data
def run_plot(frames):
    global parameters_values
    
    while not data_queue.empty():
        engine_speed, vehicle_speed, gear, throttle = data_queue.get()
        
        new_row = np.array([engine_speed, vehicle_speed, gear, throttle])
        parameters_values = np.vstack([parameters_values, new_row])
        parameters_values = parameters_values[-config.xRange:, :]
        
        for j, scatter_plot in enumerate(scatter_plots):
            scatter_plot.set_ydata(parameters_values[:, j])
    
    return scatter_plots

def store_break():
    global breakGUI
    breakGUI = b'b'

def store_releaseBreak():
    global breakGUI
    breakGUI = b'r'

def moreThrottle():
    global customThrottle
    customThrottle = b'p'

def lessThrottle():
    global customThrottle
    customThrottle = b'm'

def resetCustomThrottle():
    global customThrottle
    customThrottle = b'r'

def check_and_send_values():
    if breakGUI != 0:
        ser.write(breakGUI)
    if customThrottle != 0:
        ser.write(customThrottle)
        #resetCustomThrottle()
    # Schedule the function to run again after 100 milliseconds
    root.after(100, check_and_send_values)


if __name__ == "__main__":
    # Create a full screen window for the GUI
    root = tk.Tk()
    root.title("Tractor Dashboard")
    root.attributes('-fullscreen', True)

    # Create a frame for the Matplotlib figure
    frame = tk.Frame(root)
    frame.place(relwidth=0.75, relheight=1)

    # Create a Matplotlib figure canvas in the TK window
    canvas = FigureCanvasTkAgg(fig, master = frame)
    canvas.draw()
    canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand = True)

    # Buttons and GUI elements
    control_frame = tk.Frame(root, bg="gray")
    control_frame.place(relx=0.75, relwidth=0.25, relheight=1)

    # Title
    title_label = tk.Label(control_frame, text="TRACTOR DASHBOARD", font=("Arial", 28, "bold"), fg="white", bg="gray")
    title_label.pack(pady=(40, 10))
    team_label = tk.Label(control_frame, text="by Team1", font=("Arial", 14), fg="white", bg="gray")
    team_label.pack(pady=(0, 170))

    # Break buttons
    break_frame = tk.Frame(control_frame, bg="gray")
    break_frame.pack(pady=40)
    break_label = tk.Label(break_frame, text="Break Control", font=("Arial", 18, "bold"), fg="white", bg="gray")
    break_label.pack(pady=(0, 10))
    break_button = tk.Button(break_frame, text="Break", font=("bold"), width=button_width, height=button_height, fg="white", bg="#00008B", command=store_break)
    break_button.pack(side=tk.LEFT)
    release_button = tk.Button(break_frame, text="Release Break", font=("bold"), width=button_width, height=button_height, fg="white", bg="#00008B", command=store_releaseBreak)
    release_button.pack(side=tk.LEFT)

    # Throttle buttons
    throttle_frame = tk.Frame(control_frame, bg="gray")
    throttle_frame.pack(pady=40)
    throttle_label = tk.Label(throttle_frame, text="Throttle Control", font=("Arial", 18, "bold"), fg="white", bg="gray")
    throttle_label.pack(pady=(0, 10))
    throttle_plus_button = tk.Button(throttle_frame, text="Throttle +", font =("bold"), command=moreThrottle, fg="white", bg="#00008B", width=button_width, height=button_height)
    throttle_plus_button.pack(side=tk.LEFT)
    throttle_minus_button = tk.Button(throttle_frame, text="Throttle -", font=("bold"), command=lessThrottle, fg="white", bg="#00008B", width=button_width, height=button_height)
    throttle_minus_button.pack(side=tk.LEFT)

    # Exit button
    exit_button = tk.Button(control_frame, text="EXIT", font=("bold"), command=root.destroy, width=button_width, height=button_height, fg="white", bg="red")
    exit_button.pack(pady=(230, 0))


    # Start the serial data reading thread
    thread = threading.Thread(target=read_serial_data)
    thread.daemon = True
    thread.start()

    check_and_send_values()

    # Create animation
    ani = FuncAnimation(fig, run_plot, frames=range(config.xRange), blit=True)
    root.mainloop()
    
    
    
