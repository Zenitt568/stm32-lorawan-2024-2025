import time
from tkinter.messagebox import showinfo
import tkinter as tk
from tkinter import ttk
from tkinter import Toplevel
from serial.tools import list_ports
import serial
from tkinter.messagebox import showerror



# --------------------------FUNCTIONS---------------------------

# -------------------------------------POPUP------------------------------------------
master_port, slave_port = '', ''

def close_window():
    global master_port, slave_port

    if ports_box_master.get() == ports_box_slave.get():
        showerror("Error", "Check the selected ports, they are probably the same!")
        return
    else:
        master_port = ports_box_master.get()
        slave_port = ports_box_slave.get()
        print(master_port)
        pop.destroy()


def popup(window):
    global ports_box_master, ports_box_slave, pop
    pop = Toplevel(window)
    pop.geometry(f"{300}x{100}+{700}+{300}")
    pop.title("Choose ports for STM32")
    window.lift()

    ports = list_ports.comports()
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
            position += 1

        if element_to_combox not in all_ports_str:
            all_ports_str.append(element_to_combox)

    tk.Label(pop, text="Master port: ").grid(row=0, column=0, stick='e', padx=10)
    tk.Label(pop, text="Slave port: ").grid(row=1, column=0, stick='e', padx=10)

    ports_box_master = ttk.Combobox(pop, values=all_ports_str)
    ports_box_master.config(state="readonly")
    ports_box_master.current(0)
    ports_box_master.grid(row=0, column=1)

    ports_box_slave = ttk.Combobox(pop, values=all_ports_str)
    ports_box_slave.config(state="readonly")
    ports_box_slave.current(0)
    ports_box_slave.grid(row=1, column=1)

    destroy_btn = tk.Button(pop, text="Received", command=close_window)
    destroy_btn.grid(row=0, column=3, rowspan=2, stick='ns')

# -------------------------------------END_POPUP------------------------------------------

# -------------------------------------WINDOW------------------------------------------

def receive_data():
    print("show port")
    print(master_port, slave_port)


# def configure_serial(event):
#     serial_object_MASTER.baudrate = 115200
#     try:
#         serial_object_MASTER.port = ports_box_master.get()
#         serial_object_MASTER.open()
#     except serial.serialutil.SerialException:
#         showerror(title="Error", message='Could not open port "' + ports_box_master.get() + '"')
#         return


# def receive_data():
#     if serial_object_MASTER.isOpen:
#         while True:
#             size = serial_object_MASTER.inWaiting()
#             print(size)
#             if size:
#
#                 data = str(serial_object_MASTER.read(size)) + '\n'
#                 print(data)
#                 master_text.insert(tk.END, data)
#                 break
#             time.sleep(1)

# -------------------------------------END_WINDOW------------------------------------------
# -----------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------
# ----------------------------------------MAIN--------------------------------------------

window = tk.Tk()

height = 500
width = 680
d_x = 500
d_y = 100

window.title('STM32 LoRa Communication')
window.geometry(f"{height}x{width}+{d_x}+{d_y}")
window.minsize(width, height)
window.maxsize(width, height)

# function for creating popup window with com ports selection
popup(window)

tk.Label(window, text="MASTER",font=("Arial", 40)).grid(row=0, column = 0, stick='e', padx=10)
tk.Label(window, text="SLAVE", font=("Arial", 40)).grid(row=0, column = 1, stick='e', padx=240)

master_text = tk.Text(window, height=5, width=30)
master_text.grid(row=2, column=0, pady=5, padx=5)

display_master_btn = tk.Button(window, text="Received", command=receive_data)
display_master_btn.grid(row=3, column=0, pady=5, padx=5)

window.mainloop()