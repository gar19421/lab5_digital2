from tkinter import *
root = Tk()                             # Llame al método Tk () para inicializar una instancia de formulario raíz
root.title('LABORATORIO 5')       # método title () para establecer el texto del título
root.geometry('400x400')                # geometry () establece el tamaño de la ventana, el signo de multiplicación aquí no es *, sino la letra x minúscula en inglés


is_on = True

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
    else:
        
        on_button.config(image = on)
        my_label.config(text = "The Switch is On", fg = "green")
        is_on = True
  
# Define Our Images
on = PhotoImage(file = "on.png")
off = PhotoImage(file = "off.png")
  
# Create A Button
on_button = Button(root, image = on, bd = 0,
                   command = switch)
on_button.pack(pady = 20)

root.mainloop()
