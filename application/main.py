import tkinter as tk
from tkinter import ttk
from tkinter.messagebox import showerror
from tkinter.messagebox import showinfo

from serial.tools import list_ports
import serial

ports = list_ports()
serial_object = serial.Serial()
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



window = tk.Tk()

height = 900
width = 900
d_x = 500
d_y = 0

