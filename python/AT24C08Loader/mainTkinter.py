"""
    Generated with ChatGPT based on original implementation on main.py
    Fabien AUBRET - 2025
"""
import time
import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports as list_ports
import re


class SerialWriterGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Writer GUI")
        self.root.geometry("600x500")

        self.ser = None   # now an instance variable

        self.rw = tk.BooleanVar()

        # --- Build UI ---
        self.build_ui()

        # Load available COM ports
        self.refresh_ports()

    # ---------------- UI Setup ---------------- #

    def build_ui(self):
        frame = ttk.Frame(self.root, padding=10)
        frame.pack(fill="both", expand=True)

        # === COM PORT SELECTION ===
        ttk.Label(frame, text="Select COM Port:").grid(row=0, column=0, sticky="w")
        self.port_combo = ttk.Combobox(frame, width=20)
        self.port_combo.grid(row=0, column=1, sticky="w")

        refresh_btn = ttk.Button(frame, text="Refresh", command=self.refresh_ports)
        refresh_btn.grid(row=0, column=2, padx=5)

        connect_btn = ttk.Button(frame, text="Connect", command=self.connect_serial)
        connect_btn.grid(row=0, column=3, padx=5)

        # === BUFFER ENTRY ===
        ttk.Label(frame, text="Values (space-separated, 0–255):") \
            .grid(row=1, column=0, columnspan=2, sticky="w", pady=10)
        self.buffer_entry = ttk.Entry(frame, width=40)
        self.buffer_entry.grid(row=1, column=2, columnspan=2, sticky="w")

        # === ADDRESS + PAGE ===
        ttk.Label(frame, text="Start Address (0–255):").grid(row=2, column=0, sticky="w")
        self.address_entry = ttk.Entry(frame, width=10)
        self.address_entry.grid(row=2, column=1, sticky="w")

        ttk.Label(frame, text="Page (0–8):").grid(row=2, column=2, sticky="w")
        self.page_entry = ttk.Entry(frame, width=10)
        self.page_entry.grid(row=2, column=3, sticky="w")

        read_chkbx = tk.Checkbutton(frame, text="Read/Write (Write if checked)", variable=self.rw, onvalue=True)
        read_chkbx.grid(row=3, column=0, columnspan=2, pady=10)

        # === SEND BUTTON ===
        send_btn = ttk.Button(frame, text="Send Data", command=self.send_data)
        send_btn.grid(row=3, column=2, columnspan=2, pady=10)

        # === OUTPUT LOG ===
        ttk.Label(frame, text="Output Log:").grid(row=4, column=0, columnspan=4, sticky="w")
        self.output_box = tk.Text(frame, height=15)
        self.output_box.grid(row=5, column=0, columnspan=4, sticky="nsew")

        frame.rowconfigure(5, weight=1)
        frame.columnconfigure(3, weight=1)

    # ---------------- Utility Functions ---------------- #

    def log(self, msg):
        self.output_box.insert(tk.END, msg + "\n")
        self.output_box.see(tk.END)

    def refresh_ports(self):
        ports = [p.device for p in list_ports.comports()]
        self.port_combo["values"] = ports
        if ports:
            self.port_combo.current(0)

    def connect_serial(self):
        port = self.port_combo.get()

        if not port:
            messagebox.showerror("Error", "No COM port selected!")
            return

        try:
            self.ser = serial.Serial(port, 115200, timeout=1)
            self.log("Connected to " + port)

            # --- Let some time for the Serial port to connect and reset the buffer
            time.sleep(2)
            self.ser.reset_input_buffer()
        except Exception as e:
            messagebox.showerror("Error", f"Cannot connect: {e}")

    # ---------------- Send Data ---------------- #

    def send_data(self):
        if self.ser is None:
            messagebox.showerror("Error", "Serial port not connected.")
            return

        # Read buffer values
        try:
            values = self.buffer_entry.get().strip().split()
            values = [int(v) for v in values]
            for v in values:
                if v < 0 or v > 255:
                    raise ValueError()
        except:
            messagebox.showerror("Error", "Buffer must contain integers 0–255.")
            return

        # Address + page
        try:
            address = int(self.address_entry.get())
            page = int(self.page_entry.get())
            if not (0 <= address <= 255 and 0 <= page <= 8):
                raise ValueError()
        except:
            messagebox.showerror("Error", "Invalid address or page.")
            return

        # Insert buffer length at beginning
        values = [len(values)] + values

        self.log("=== Sending Data ===")
        self.log(f"Page: {page}, Start Address: {address}")
        self.log(f"Buffer: {values}")

        curr_addr = address

        for val in values:
            if curr_addr > 255:
                self.log("Address exceeded 255, stopping.")
                break

            if self.rw.get():
                line = f"w{page} {curr_addr} {val}\n\r"
            else:
                line = f"r{page} {curr_addr}\n\r"
            written = self.ser.write(line.encode('utf-8'))

            if written > 0:
                response = self.ser.readline().decode(errors="ignore")

                if self.rw.get():
                    # --- Handle value write
                    while "OK" not in response:
                        response = self.ser.readline().decode(errors="ignore")
                    self.log(f"Wrote: {line.strip()}")

                else:
                    # --- Handle value read
                    while "VAL" not in response:
                        response = self.ser.readline().decode(errors="ignore")

                    pattern = r"VAL\s+(\d{1,3})"
                    match = re.search(pattern, response)
                    # --- Response should look like "Read value <value> on page <page>, address <address>"
                    # --- So we are parsing to get the <value>
                    if match:
                        print(response)
                        self.log(f"Value: {match.group(1)}")

            curr_addr += 1

        self.log("=== Done ===")



# ---------------- Run Application ---------------- #

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialWriterGUI(root)
    root.mainloop()
