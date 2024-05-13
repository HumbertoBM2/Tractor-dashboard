# Team 1
# Humberto Barrera
# Catalina Muñoz
# Mauricio Zavala
# Erick Campo

# Libraries
import os

# Define the number of variables in the data
NUMBER_OF_VARIABLES = 3

# Define bounds for engine speed, vehicle speed, and gear
ENGINE_SPEED_BOUNDS = [0, 6000]
VEHICLE_SPEED_BOUNDS = [0, 200]
GEAR_BOUNDS = [1, 4]

# Define size for plot
PLOT_SIZE = (6, 7)

# Define range for x-axis in plot
X_RANGE = 20

# Get the base path of the current file
BASE_PATH = os.path.dirname(os.path.realpath(__file__))

# Create a path to the CSV file within the base path
CSV_FILE_PATH = os.path.join(BASE_PATH, "E2Data.csv")

# Define the serial port based on the operating system
SERIAL_PORT = "COM7"            # Windows
# SERIAL_PORT = "/dev/ttyS0"    # Debian

# Define baudrate for serial communication
BAUDRATE = 115200
