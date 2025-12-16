import time

import serial
import serial.tools.list_ports as list_ports

class M24M01:
    def __init__(self):
        self.size_in_bytes = 128000
        # --- Generate 4 pages of 256 bytes
        self.memory = [0 for k in range(self.size_in_bytes)]

        self.max_written_address = 0

    def __str__(self):
        return str(self.memory)

    def store(self, address=0, value=0):
        if address > self.max_written_address:
            self.max_written_address = address + 1
        self.memory[address]= value

    def commit(self):
        ports = [p.device for p in list_ports.comports()]
        print(ports)
        port = input("Select COM port:")
        ser = serial.Serial(port, 115200, timeout=1)
        print("Connected to " + port)

        # --- Let some time for the Serial port to connect and reset the buffer
        time.sleep(2)
        ser.reset_input_buffer()
        for address in range(self.max_written_address + 1):
            value = self.memory[address]
            line = f"w{address} {value}\r\n"
            written = ser.write(line.encode('utf-8'))

            if written > 0:
                response = ser.readline().decode(errors="ignore")
                timeout = 0
                while "OK" not in response and timeout < 1000:
                    if response == '':
                        timeout += 1
                    response = ser.readline().decode(errors="ignore")
                    print("timeout: ", timeout)

                print(f"Wrote: {line.strip()}")
                time.sleep(0.01)
