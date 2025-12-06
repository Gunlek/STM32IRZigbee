import time

import serial
import serial.tools.list_ports as list_ports

class AT24C:
    def __init__(self, pages, bytes_per_page):
        # --- Generate 4 pages of 256 bytes
        self.memory = [[0 for k in range(bytes_per_page)] for i in range(pages)]

        self.pages = pages
        self.bytes_per_page = bytes_per_page

    def __str__(self):
        str_mmy = ""
        for page in self.memory:
            str_mmy += str(page) + '\n'
        return str_mmy

    def store(self, page, address, value):
        self.memory[page][address] = value

    def commit(self):
        ports = [p.device for p in list_ports.comports()]
        print(ports)
        port = input("Select COM port:")
        ser = serial.Serial(port, 115200, timeout=1)
        print("Connected to " + port)

        # --- Let some time for the Serial port to connect and reset the buffer
        time.sleep(2)
        ser.reset_input_buffer()

        for page in range(self.pages):
            for address in range(self.bytes_per_page):
                value = self.memory[page][address]
                line = f"w{page} {address} {value}\r\n"
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
