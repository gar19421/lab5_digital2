# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Diego Morales

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed

#threading
import time
import threading

#LIBRERÍAS
from tkinter import *
from tkinter.font import Font
from time import sleep 
import serial
import sys

import tkinter as tk
from tkinter import colorchooser


ADAFRUIT_IO_USERNAME = "Brandon_g"
ADAFRUIT_IO_KEY = "aio_nQhX57DX33sA5TCBKH6sXGt4S4Su"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)


#CREAR PUERTO SERIAL PARA LA COMUNICACION  USANDO PYTHON
puerto= serial.Serial('COM3',9600)

root=Tk()
#TITULO DE LA INTERFAZ
root.title("     COMUNICACION SERIAL Y ADAFRUIT    ")
#DIMENSIONES DE LA INTERFAZ
root.geometry('400x400')


#extraido de https://www.geeksforgeeks.org/on-off-toggle-button-switch-in-tkinter/
is_on = True
adc_value = 0

# Create Label
my_label = Label(root, 
    text = "The Switch Is On!", 
    fg = "green", 
    font = ("Helvetica", 20))
  
my_label.pack(pady = 10)
  
# Define our switch function
def switch():
    global is_on
      
    # Determin is on or off
    if is_on:
        on_button.config(image = off)
        my_label.config(text = "The Switch is Off", 
                        fg = "grey")
        is_on = False
        #Digital Feed
        digital_feed = aio.feeds('indicador')
        aio.send_data(digital_feed.key, 0)
        digital_data = aio.receive(digital_feed.key)
        print(f'digital signal: {digital_data.value}')
        
        
    else:
        
        on_button.config(image = on)
        my_label.config(text = "The Switch is On", fg = "green")
        is_on = True
        #Digital Feed
        digital_feed = aio.feeds('indicador')
        aio.send_data(digital_feed.key, 1)
        digital_data = aio.receive(digital_feed.key)
        print(f'digital signal: {digital_data.value}')
        
  
# Define Our Images
on = PhotoImage(file = "on.png")
off = PhotoImage(file = "off.png")
  
# Create A Button
on_button = Button(root, image = on, bd = 0,
                   command = switch)
on_button.pack(pady = 20)


# extraído de https://www.python-course.eu/tkinter_sliders.php
def show_values():
    #Analog Feed
    analog_feed = aio.feeds('adc')
    aio.send_data(analog_feed.key, w.get())
    analog_data = aio.receive(analog_feed.key)
    print(f'analog signal: {analog_data.value}')

# Create Label
my_label1 = Label(root, 
    text = "ADC Control", 
    fg = "red", 
    font = ("Helvetica", 20))
  
my_label1.pack(pady = 5)

w = Scale(root, from_=0, to=255, orient=HORIZONTAL)
w.pack(pady = 5)

Button(root, text='Send Values', command=show_values).pack()


#extraído de https://www.geeksforgeeks.org/python-tkinter-choose-color-dialog/
# Function that will be invoked when the
# button will be clicked in the main window
def choose_color():
    # variable to store hexadecimal code of color
    color_code = colorchooser.askcolor(title ="Choose color")
    #Analog Feed
    analog_feed = aio.feeds('color')
    aio.send_data(analog_feed.key, color_code[1])
    analog_data = aio.receive(analog_feed.key)
    print(f'analog signal: {analog_data.value}')
 
# Create Label
my_label1 = Label(root, 
    text = "Color Picker", 
    fg = "blue", 
    font = ("Helvetica", 20))
my_label1.pack(pady = 5)

Button(root, text = 'Select color', command=choose_color).pack(pady = 5)


#extraído de https://es.stackoverflow.com/questions/369312/python-ejecutar-script-cada-cierto-tiempo-y-al-mismo-tiempo-ejecutar-lo-que-qued
def ejecucion_horaria(segundos):
    """
    Este es un thread con parametros.
    
    @param segundos: Ejecuta un print cada tantos segundos. 
    """
    #print("Leyendo el puerto serial")
    while(1):
        print("Leyendo el puerto serial")
        #ABRIR UNA CONEXION SERIAL
        puerto.open()
        time.sleep(segundos)
        rawString = puerto.readline()
        print(rawString)
        puerto.close()

# Aqui creamos el thread.
# El primer argumento es el nombre de la funcion que contiene el codigo.
# El segundo argumento es una lista de argumentos para esa funcion .
# Ojo con la coma al final!

hilo = threading.Thread(target=ejecucion_horaria, args=(1,))
hilo.start()   # Iniciamos la ejecución del thread,



root.mainloop()


    