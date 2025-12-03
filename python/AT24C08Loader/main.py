"""
    Work by Fabien AUBRET
    December 2025
"""

import serial
import serial.tools.list_ports as list_ports

# List all available COM ports
com_ports = list_ports.comports()

# Select a port available from the list (by index)
print("Select port:")
index = 0
for port in com_ports:
    print(str(index) + ". " + port.device)
    index += 1
port_index = int(input("Enter value between 0 and " + str(index-1) + ": "))

selected_port = com_ports[port_index]
print("Selected port: " + selected_port.device)

# Open serial connection to the selected COM
ser = serial.Serial(selected_port.device, 115200)

# Request values to be written
in_buffer = input("Values to be written (integer, space separated, 8 bits): ")
in_buffer_lst = in_buffer.split(" ")

for value in in_buffer_lst:
    assert(int(value) < 256)

buffer_length = len(in_buffer_lst)

print("Buffer length: " + str(buffer_length))

# Request address and page the Arduino will write on
address = int(input("Starting at address: "))
assert(address < 256)
page = int(input("Of page: "))
assert(page <= 8)

# Data to be written:
# [buffer_length, value0, value1, value2, etc...]
#    address, address+1, address+2, etc...
print("WARN: First data written will be the length of the buffer then the actual buffer values")

in_buffer_lst = [len(in_buffer_lst)] + in_buffer_lst
for value in in_buffer_lst:
    if address > 255:
        break

    str_line = str(page) + " " + str(address) + " " + str(value) + "\n\r"
    written = ser.write(str_line.encode('utf-8'))
    if written > 0:
        # Wait for response
        response = str(ser.readline())

        while 'Byte written successfully' not in response:
            response = str(ser.readline())
        print(str(written) + " bytes written: " + str_line.replace('\n\r', ''))

    address += 1

