"""
View
"""
from tkinter import Tk, Frame, Button, Label, Entry, font, E, W, N, S, LEFT, CENTER
from system_control import CultureFlow_Control
#default_font = tkFont.nametofont("TkDefaultFont")
#default_font.configure(size=48)
#root.option_add("*Font", default_font)



class App(Tk):

    def __init__(self,*args,**kwargs):

        Tk.__init__(self,*args,**kwargs)

        # background_image=tk.PhotoImage(...)
        # background_label = tk.Label(parent, image=background_image)
        # background_label.place(x=0, y=0, relwidth=1, relheight=1)
        self.app_name = "CultureFlow"
        self.test_mode = True

        self.system = None
        self.load_system = False

        self.afont = font.Font(family='Arial', size=16, weight='bold')
        self.bfont = font.Font(family='Verdana', size=12, weight='bold')
        self.pfont = font.Font(family='Faktos', size=18, weight='bold')

        self.geometry('500x750')
        self.resizable(width=True, height=True)
        container = Frame(self)
        container.pack(side="top", fill="both", expand = True)
        container.grid_rowconfigure(0,weight = 1)
        container.grid_columnconfigure(0,weight = 1)

        self.frames = {}

        for F in (intro_screen_1,intro_screen_2,intro_screen_3):
            frame = F(container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(intro_screen_1)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()

class intro_screen_1(Frame):

    #This screen prompts the user to make sure the main power is on
    def __init__(self, parent, controller):

        Frame.__init__(self,parent)

        ### Heading text

        self.main_label_frame = Frame(self)
        self.main_label_frame['borderwidth'] = 2
        self.main_label_frame['relief'] = 'solid'

        self.main_text ="""Welcome to CultureFlow!"""
        self.main_label = Label(self.main_label_frame, text = self.main_text, font = controller.afont,justify=CENTER)

        self.sub_text = """A dynamic cell culture platform."""
        self.sub_label = Label(self.main_label_frame, text = self.sub_text, font = controller.bfont,justify=CENTER)

        ### Instructions

        self.instructions = """Please confirm that the main power is on \nthen select Continue to begin device setup."""
        self.instructions_label = Label(self, text = self.instructions, font = "Times 12",justify=LEFT)

        ### Buttons

        self.exit_button     = Button(self,text = "Quit Program",command = controller.destroy, font=controller.bfont)
        self.continue_button = Button(self, text = "Continue", font= controller.bfont, command = lambda: self.cont(controller))

        ### Widget Placement

        self.main_label_frame.grid(column = 0, row = 0, columnspan = 2, rowspan = 1)
        self.main_label.grid(column = 0, row = 0, columnspan = 1)
        self.sub_label.grid(column = 0, row = 1, columnspan = 1)

        self.instructions_label.grid(column = 0, row = 3, columnspan = 2)

        self.exit_button.grid(column = 0, row = 4,columnspan = 1)
        self.continue_button.grid(column = 1, row = 4,columnspan = 1)

    def cont(self,controller):
        """Changes load_system to True. It needs to do this so that we don't load on start up since we instantiate all frames then.
        It then sends you to the next frame and calls the function to begin system load via instantiation of controller object.
        """
        controller.load_system = True
        controller.show_frame(intro_screen_2)
        controller.frames[intro_screen_2].prepare_system(controller)

class intro_screen_2(Frame):

    #This screen displays serial communications and a load prompt
    def __init__(self, parent, controller):
        Frame.__init__(self,parent)

        self.loading_status = ''

        self.loading_label = Label(self,text=self.loading_status,justify=LEFT)
        self.loading_label.grid(column=1,row=1)
        self.controller = controller
    #we'll just have a frame here that we'll update with text as components are bring detected.
    def prepare_system(self,controller):
        if controller.load_system:
            controller.system = CultureFlow_Control(controller)
        controller.system.load(self)

    def update_text(self):
            self.loading_status += self.controller.system.view_message + "\n"
            self.loading_label['text'] = self.loading_status

class intro_screen_3(Frame):
    #This screen displays devices found and asks user if they want to continue
    def __init__(self, parent, controller):
        Frame.__init__(self,parent)

app = App()
app.mainloop()
