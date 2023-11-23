import tkinter as tk
from tkinter import ttk
from tkinter.messagebox import showerror
from tkinter.messagebox import showinfo

from serial.tools import list_ports
import serial

ports = list_ports.comports()
serial_object_MASTER = serial.Serial()
serial_object_SLAVE = serial.Serial()
all_ports_str = ['Port not selected']
element_to_combox = ''

for element in ports:
    element_str = str(element)

    position = 0
    element_to_combox = ''
    while position <= len(element_str) + 1:
        if element_str[position] == ' ':
            break
        element_to_combox = element_to_combox + element_str[position]
        position = position + 1

    all_ports_str.append(element_to_combox)


def configure_serial(event,serial_object, box):
    serial_object.baudrate = 115200
    try:
        serial_object.port = box.get()
        serial_object.open()
    except serial.serialutil.SerialException:
        showerror(title="Error", message='Could not open port "' + box.get() + '"')
        return


window = tk.Tk()

height = 500
width = 680
d_x = 500
d_y = 100

window.title('STM32 LoRa Communication')
window.geometry(f"{height}x{width}+{d_x}+{d_y}")
window.minsize(width, height)
window.maxsize(width, height)

tk.Label(window, text="MASTER",font=("Arial", 40)).grid(row=0, column = 0, stick='e', padx=10)
tk.Label(window, text="SLAVE", font=("Arial", 40)).grid(row=0, column = 1, stick='e', padx=240)

ports_box_master = ttk.Combobox(window, values=all_ports_str)
ports_box_master.config(state="readonly")
ports_box_master.bind("<<ComboboxSelected>>", lambda event: configure_serial(event, serial_object_MASTER, ports_box_master))
ports_box_master.current(0)
ports_box_master.grid(row=1, column=0)

ports_box_slave = ttk.Combobox(window, values=all_ports_str)
ports_box_slave.config(state="readonly")
ports_box_slave.bind("<<ComboboxSelected>>", lambda event: configure_serial(event, serial_object_SLAVE, ports_box_slave))
ports_box_slave.current(0)
ports_box_slave.grid(row=1, column=1)

master_text = tk.Text(window, height=5, width=30).grid(row=2, column=0, pady =5, padx=5)
if (serial_object_MASTER.isOpen() == True):
    FACT_M = serial_object_MASTER.readline()
    master_text.insert(tk.END, FACT_M)

slave_text = tk.Text(window, height=5, width=30).grid(row=2, column=1, pady =5, padx=5)
if (serial_object_SLAVE.isOpen() == True):
    FACT_S = serial_object_SLAVE.readline()
    master_text.insert(tk.END, FACT_S)


window.mainloop()