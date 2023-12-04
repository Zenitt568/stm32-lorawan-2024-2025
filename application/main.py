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
helpflag = True


def close_window():
    global master_port, slave_port

    if ports_box_master.get() == ports_box_slave.get():
        showerror("Error", "Check the selected ports, they are probably the same!")
        return
    else:
        master_port = ports_box_master.get()
        slave_port = ports_box_slave.get()
        pop.destroy()


def popup(window):
    global ports_box_master, ports_box_slave, pop
    pop = Toplevel(window)
    pop.geometry(f"{300}x{60}+{670}+{180}")
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


def configure_serial(event):
    serial_object_MASTER.baudrate = 115200
    try:
        serial_object_MASTER.port = ports_box_master.get()
        serial_object_MASTER.open()
    except serial.serialutil.SerialException:
        showerror(title="Error", message='Could not open port "' + ports_box_master.get() + '"')
        return


def receive_data():

    master_text.delete('1.0', tk.END)
    slave_text.delete('1.0', tk.END)

    data_to_show_master = []
    data_to_show_slave = []

    if master_port and slave_port:
        global serial_object_MASTER, serial_object_SLAVE

        serial_object_MASTER = serial.Serial()
        serial_object_SLAVE = serial.Serial()

        serial_object_MASTER.baudrate = 115200
        serial_object_SLAVE.baudrate = 115200
        try:
            serial_object_MASTER.port = master_port
            serial_object_MASTER.open()

        except serial.serialutil.SerialException:
            showerror(title="Error", message='Could not open port "' + master_port + '"')

        try:
            serial_object_SLAVE.port = slave_port
            serial_object_SLAVE.open()
        except serial.serialutil.SerialException:
            showerror(title="Error", message='Could not open port "' + slave_port + '"')

        print(serial_object_MASTER.isOpen())

        if serial_object_MASTER.isOpen and serial_object_SLAVE.isOpen():
            i = 0
            while True:
                size_m = serial_object_MASTER.inWaiting()
                print(size_m)
                if size_m:
                    data_m = str(serial_object_MASTER.read(size_m).decode("utf-8"))
                    data_to_show_master.append(data_m)
                    print(data_m)
                    i = 0

                size_l = serial_object_SLAVE.inWaiting()
                print("_______________________________________")
                print(size_l)
                if size_l:
                    data_l = str(serial_object_SLAVE.read(size_l))
                    data_to_show_slave.append(data_l)
                    print(data_l)
                    i = 0

                i = i + 1
                time.sleep(1)
                if i > 10:
                    break

            for element in data_to_show_master:
                master_text.insert(tk.END, element)
            for element in data_to_show_slave:
                slave_text.insert(tk.END, element)
    else:
        print("Port haven't been chosen")



# -------------------------------------END_WINDOW------------------------------------------
# -----------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------
# ----------------------------------------MAIN--------------------------------------------

window = tk.Tk()

height = 280
width = 600
d_x = 500
d_y = 100

window.title('STM32 LoRa Communication')
window.geometry(f"{height}x{width}+{d_x}+{d_y}")
window.minsize(width, height)
window.maxsize(width, height)

# function for creating popup window with com ports selection
popup(window)


tk.Label(window, text="MASTER",font=("Arial", 40)).grid(row=0, column = 0, padx=30)
tk.Label(window, text="SLAVE", font=("Arial", 40)).grid(row=0, column = 2, padx=50)

master_text = tk.Text(window, height=5, width=30, wrap="none")
master_text.grid(row=2, column=0, pady=5, padx=5, stick = 'ew')

ys = ttk.Scrollbar(window, orient='vertical', command=master_text.yview)
ys.grid(row=2, column=1, stick = 'ns')

xs = ttk.Scrollbar(window, orient='horizontal', command=master_text.xview)
xs.grid(row=3, column=0, columnspan =2, stick = 'ew')

master_text['yscrollcommand'] = ys.set
master_text['xscrollcommand'] = xs.set



slave_text = tk.Text(window, height=5, width=30, wrap="none")
slave_text.grid(row=2, column=2, pady=5, padx=10, stick = 'ew')

ys = ttk.Scrollbar(window, orient='vertical', command=master_text.yview)
ys.grid(row=2, column=3, stick = 'ns')

xs = ttk.Scrollbar(window, orient='horizontal', command=master_text.xview)
xs.grid(row=3, column=2, padx = 10, columnspan =2, stick = 'ew')

master_text['yscrollcommand'] = ys.set
master_text['xscrollcommand'] = xs.set



display_master_btn = tk.Button(window, text="LISTEN TO DATA", command=receive_data)
display_master_btn.grid(row=4, column=0, columnspan = 4, pady=40, padx=30, stick='ew')








window.mainloop()