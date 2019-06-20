from tkinter import Frame, Canvas, Scrollbar, Button, Label
from tkinter import LEFT, BOTH, font, DISABLED, NORMAL

class Scrolling_Canvas(Frame):

    def __init__(self, root):

        Frame.__init__(self, root)
        self.canvas = Canvas(root, borderwidth=0, background="#ffffff")
        self.frame = Frame(self.canvas, background="#ffffff")
        self.vsb = Scrollbar(root, orient="vertical", command=self.canvas.yview)
        self.canvas.configure(yscrollcommand=self.vsb.set)

        self.vsb.pack(side="right", fill="y")
        self.canvas.pack(side="top", fill="both", expand=True)
        self.canvas.create_window((4,4), window=self.frame, anchor="nw",
                                  tags="self.frame")

        self.frame.bind("<Configure>", self.onFrameConfigure)


    def onFrameConfigure(self, event):
        '''Reset the scroll region to encompass the inner frame'''
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

class Horizontal_Button_Frame(Frame):

    def __init__(self,root,button_functions = {}):

        my_font = font.Font(family='Verdana', size=12, weight='bold')

        Frame.__init__(self, root)

        self.button = {}

        for name in button_functions:

            next_button = Button(self,text=name,font = my_font, padx=50)
            next_button['command'] = button_functions[name]
            next_button.config(height=3,width=3)
            next_button.pack(side=LEFT,fill=BOTH)
            self.button[name] = next_button


    def set_command(self,name,command):
        self.button[name]['command'] = command

class Page(Frame):
    #This screen displays devices found and asks user if they want to continue
    def __init__(self, parent, controller):

        Frame.__init__(self,parent)

        self.title = str(self.__class__.__name__).replace("_"," ")
        print("LOOK!")
        self.title_label = Label(self,text = self.title, font=controller.pfont)
        self.title_label.pack(side="top")

        self.my_canvas = Scrolling_Canvas(self)
        self.my_canvas.pack(side="top")
        #now we can create as many panels as we want!
        self.page_buttons = Horizontal_Button_Frame(self,controller.navigation_buttons)
        self.page_buttons.pack(side="bottom",fill='x')
# if __name__ == "__main__":
#     root=tk.Tk()
#     Example(root).pack(side="top", fill="both", expand=True)
