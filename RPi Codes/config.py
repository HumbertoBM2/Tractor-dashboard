# Team 1
# Humberto Barrera
# Catalina Muñoz
# Mauricio Zavala
# Erick Campo

# Libraries
import os

# Define the number of variables in the data
nVariables = 3

# Define bounds for engine speed, vehicle speed/tractor speed, and gear
engSpeed = [0, 6000]
tractSpeed = [0, 200]
gears = [1, 4]

# Define size for plot
plotSize = (12, 8)

# Define range for x-axis in plot (for time)
xRange = 20

# Get the base path of the current file
currentPath = os.path.dirname(os.path.realpath(__file__))

# Create a path to the CSV file within the base path
csvPath = os.path.join(currentPath, "E2Data.csv")

# Define the serial port based on the operating system
sPort = "COM7"            # Windows
# sPort = "/dev/ttyS0"    # Debian

# Define baudrate for serial communication
baudrate = 115200
