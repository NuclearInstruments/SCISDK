from scisdk.scisdk import SciSDK
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.animation as animation
import math
import numpy as np
import scipy.optimize as opt

tau = 1000.0
centroid = 0.0
sigma_fit = 0.0

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("192.168.102.220:8888", "DT5560", "./RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set register values
res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)

res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", 500)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", 1)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN", 5000)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST", 25)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB", 50)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS", 800)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", 5)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", 7)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", 100)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", 110)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/tau)-1)))
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", 3000)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", 7)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", 107)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.PUR_MODE", 2)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.PUR_INIB", 50)

res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)

# ## SPECTRUM

# Set spectrum parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")

# Execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# Execute command start
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# Allocate buffer
global obSpectrum

class MyCanvas(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        super(MyCanvas, self).__init__(fig)


class RealTimePlot(QMainWindow):
    def __init__(self):
        super(RealTimePlot, self).__init__()

        # Set the parameters
        self.centroid=0.0
        self.sigma_fit=0.0
        self.centroid_fit=0.0

        self.RESET_THRS = 500
        self.POL = 1
        self.RESET_LEN = 5000
        self.HIST = 25
        self.INIB = 50
        self.THRS = 800
        self.AN_OFS = 0
        self.TRIGGER_K = 5
        self.TRIGGER_M = 7
        self.TRAP_K = 100
        self.TRAP_M = 110
        self.TAU = 1000.0
        self.TRAP_GAIN = 3000
        self.BLLEN = 7
        self.BLINIB = 150
        self.SAMP_POS = 107
        self.TRG_MODE = 0   
        

        self.canvas = MyCanvas(self, width=5, height=4, dpi=100)
        self.setCentralWidget(self.canvas)
        self.plot()
        self.show()

        # Set up the layout
        self.layout = QHBoxLayout()

        # Set up the text boxes
        self.centroid_in_box = QLineEdit()
        self.centroid_in_box.setPlaceholderText(str(self.centroid))
        self.centroid_in_box.returnPressed.connect(self.set_centroid)

        self.sigma_box = QLineEdit(self)
        self.sigma_box.setPlaceholderText("Sigma")
        self.sigma_box.setDisabled(True)

        self.centroid_box = QLineEdit(self)
        self.centroid_box.setPlaceholderText("Centroid")
        self.centroid_box.setDisabled(True)

        # Set up the buttons
        self.start_button = QPushButton("Start")
        self.start_button.setFont(QFont("Ariel", 10))
        self.start_button.clicked.connect(self.start)
        self.stop_button = QPushButton("Stop")
        self.stop_button.setFont(QFont("Ariel", 10))
        self.stop_button.clicked.connect(self.stop)
        self.reset_button = QPushButton("Reset")
        self.reset_button.setFont(QFont("Ariel", 10))
        self.reset_button.clicked.connect(self.reset)

        # Set up the layout
        main_layout = QVBoxLayout()
        analysis_layout = QHBoxLayout()
        analysis_layout.addWidget(self.centroid_in_box)
        analysis_layout.addWidget(self.sigma_box)
        analysis_layout.addWidget(self.centroid_box)
        main_layout.addLayout(analysis_layout)
        main_layout.addWidget(self.canvas)
        button_layout = QHBoxLayout()
        button_layout.addWidget(self.start_button)
        button_layout.addWidget(self.stop_button)
        button_layout.addWidget(self.reset_button)
        main_layout.addLayout(button_layout)

        # Set up the menu
        menu_layout = QGridLayout()
        RESET_THRS_name = QLabel("RESET_THRS: ")
        RESET_THRS_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(RESET_THRS_name, 0, 0)
        POL_name = QLabel("POL: ")
        menu_layout.addWidget(POL_name, 1, 0)
        RESET_LEN_name = QLabel("RESET_LEN: ")
        RESET_LEN_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(RESET_LEN_name, 2, 0)
        HIST_name = QLabel("HIST: ")
        HIST_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(HIST_name, 3, 0)
        INIB_name = QLabel("INIB: ")
        INIB_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(INIB_name, 4, 0)
        THRS_name = QLabel("THRS: ")
        THRS_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(THRS_name, 5, 0)
        AN_OFS_name = QLabel("AN_OFS: ")
        AN_OFS_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(AN_OFS_name, 6, 0)
        TRIGGER_K_name = QLabel("TRIGGER_K: ")
        TRIGGER_K_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRIGGER_K_name, 7, 0)
        TRIGGER_M_name = QLabel("TRIGGER_M: ")
        TRIGGER_M_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRIGGER_M_name, 8, 0)
        TRAP_K_name = QLabel("TRAP_K: ")
        TRAP_K_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRAP_K_name, 9, 0)
        TRAP_M_name = QLabel("TRAP_M: ")
        TRAP_M_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRAP_M_name, 10, 0)
        DECONV_M_name = QLabel("TAU: ")
        DECONV_M_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(DECONV_M_name, 11, 0)
        TRAP_GAIN_name = QLabel("TRAP_GAIN: ")
        TRAP_GAIN_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRAP_GAIN_name, 12, 0)
        BLLEN_name = QLabel("BLLEN: ")
        BLLEN_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(BLLEN_name, 13, 0)
        BLINIB_name = QLabel("BLINIB: ")
        BLINIB_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(BLINIB_name, 14, 0)
        SAMP_POS_name = QLabel("SAMP_POS: ")
        SAMP_POS_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(SAMP_POS_name, 15, 0)
        TRG_MODE_name = QLabel("TRG_MODE: ")
        TRG_MODE_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(TRG_MODE_name, 16, 0)


        self.RESET_THRS_widg = QLineEdit()
        self.RESET_THRS_widg.setMaxLength(10)
        self.RESET_THRS_widg.setPlaceholderText(str(self.RESET_THRS))
        self.RESET_THRS_widg.setFont(QFont("Ariel", 10))
        self.RESET_THRS_widg.returnPressed.connect(self.return_RESET_THRS)
        menu_layout.addWidget(self.RESET_THRS_widg, 0, 1)
        POL_box = QComboBox()
        POL_box.addItems(["Positive", "Negative"])
        POL_box.setFont(QFont("Ariel", 10))
        POL_box.currentIndexChanged.connect(self.POL_changed)
        menu_layout.addWidget(POL_box, 1, 1)
        self.RESET_LEN_widg = QLineEdit()
        self.RESET_LEN_widg.setMaxLength(10)
        self.RESET_LEN_widg.setPlaceholderText(str(self.RESET_LEN))
        self.RESET_LEN_widg.setFont(QFont("Ariel", 10))
        self.RESET_LEN_widg.returnPressed.connect(self.return_RESET_LEN)
        menu_layout.addWidget(self.RESET_LEN_widg, 2, 1)
        self.HIST_widg = QLineEdit()
        self.HIST_widg.setMaxLength(10)
        self.HIST_widg.setPlaceholderText(str(self.HIST))
        self.HIST_widg.setFont(QFont("Ariel", 10))
        self.HIST_widg.returnPressed.connect(self.return_HIST)
        menu_layout.addWidget(self.HIST_widg, 3, 1)
        self.INIB_widg = QLineEdit()
        self.INIB_widg.setMaxLength(10)
        self.INIB_widg.setPlaceholderText(str(self.INIB))
        self.INIB_widg.setFont(QFont("Ariel", 10))
        self.INIB_widg.returnPressed.connect(self.return_INIB)
        menu_layout.addWidget(self.INIB_widg, 4, 1)
        self.THRS_widg = QLineEdit()
        self.THRS_widg.setMaxLength(10)
        self.THRS_widg.setPlaceholderText(str(self.THRS))
        self.THRS_widg.setFont(QFont("Ariel", 10))
        self.THRS_widg.returnPressed.connect(self.return_THRS)
        menu_layout.addWidget(self.THRS_widg, 5, 1)
        self.AN_OFS_widg = QLineEdit()
        self.AN_OFS_widg.setMaxLength(10)
        self.AN_OFS_widg.setPlaceholderText(str(self.AN_OFS))
        self.AN_OFS_widg.setFont(QFont("Ariel", 10))
        self.AN_OFS_widg.returnPressed.connect(self.return_AN_OFS)
        menu_layout.addWidget(self.AN_OFS_widg, 6, 1)
        self.TRIGGER_K_widg = QLineEdit()
        self.TRIGGER_K_widg.setMaxLength(10)
        self.TRIGGER_K_widg.setPlaceholderText(str(self.TRIGGER_K))
        self.TRIGGER_K_widg.setFont(QFont("Ariel", 10))
        self.TRIGGER_K_widg.returnPressed.connect(self.return_TRIGGER_K)
        menu_layout.addWidget(self.TRIGGER_K_widg, 7, 1)
        self.TRIGGER_M_widg = QLineEdit()
        self.TRIGGER_M_widg.setMaxLength(10)
        self.TRIGGER_M_widg.setPlaceholderText(str(self.TRIGGER_M))
        self.TRIGGER_M_widg.setFont(QFont("Ariel", 10))
        self.TRIGGER_M_widg.returnPressed.connect(self.return_TRIGGER_M)
        menu_layout.addWidget(self.TRIGGER_M_widg, 8, 1)
        self.TRAP_K_widg = QLineEdit()
        self.TRAP_K_widg.setMaxLength(10)
        self.TRAP_K_widg.setPlaceholderText(str(self.TRAP_K))
        self.TRAP_K_widg.setFont(QFont("Ariel", 10))
        self.TRAP_K_widg.returnPressed.connect(self.return_TRAP_K)
        menu_layout.addWidget(self.TRAP_K_widg, 9, 1)
        self.TRAP_M_widg = QLineEdit()
        self.TRAP_M_widg.setMaxLength(10)
        self.TRAP_M_widg.setPlaceholderText(str(self.TRAP_M))
        self.TRAP_M_widg.setFont(QFont("Ariel", 10))
        self.TRAP_M_widg.returnPressed.connect(self.return_TRAP_M)
        menu_layout.addWidget(self.TRAP_M_widg, 10, 1)
        self.DECONV_M_widg = QLineEdit()
        self.DECONV_M_widg.setMaxLength(10)
        self.DECONV_M_widg.setPlaceholderText(str(self.TAU))
        self.DECONV_M_widg.setFont(QFont("Ariel", 10))
        self.DECONV_M_widg.returnPressed.connect(self.return_DECONV_M)
        menu_layout.addWidget(self.DECONV_M_widg, 11, 1)
        self.TRAP_GAIN_widg = QLineEdit()
        self.TRAP_GAIN_widg.setMaxLength(10)
        self.TRAP_GAIN_widg.setPlaceholderText(str(self.TRAP_GAIN))
        self.TRAP_GAIN_widg.setFont(QFont("Ariel", 10))
        self.TRAP_GAIN_widg.returnPressed.connect(self.return_TRAP_GAIN)
        menu_layout.addWidget(self.TRAP_GAIN_widg, 12, 1)
        self.BLLEN_widg = QLineEdit()
        self.BLLEN_widg.setMaxLength(10)
        self.BLLEN_widg.setPlaceholderText(str(self.BLLEN))
        self.BLLEN_widg.setFont(QFont("Ariel", 10))
        self.BLLEN_widg.returnPressed.connect(self.return_BLLEN)
        menu_layout.addWidget(self.BLLEN_widg, 13, 1)
        self.BLINIB_widg = QLineEdit()
        self.BLINIB_widg.setMaxLength(10)
        self.BLINIB_widg.setPlaceholderText(str(self.BLINIB))
        self.BLINIB_widg.setFont(QFont("Ariel", 10))
        self.BLINIB_widg.returnPressed.connect(self.return_BLINIB)
        menu_layout.addWidget(self.BLINIB_widg, 14, 1)
        self.SAMP_POS_widg = QLineEdit()
        self.SAMP_POS_widg.setMaxLength(10)
        self.SAMP_POS_widg.setPlaceholderText(str(self.SAMP_POS))
        self.SAMP_POS_widg.setFont(QFont("Ariel", 10))
        self.SAMP_POS_widg.returnPressed.connect(self.return_SAMP_POS)
        menu_layout.addWidget(self.SAMP_POS_widg, 15, 1)
        self.TRG_MODE_widg = QLineEdit()
        self.TRG_MODE_widg.setMaxLength(10)
        self.TRG_MODE_widg.setPlaceholderText(str(self.TRG_MODE))
        self.TRG_MODE_widg.setFont(QFont("Ariel", 10))
        self.TRG_MODE_widg.returnPressed.connect(self.return_TRG_MODE)
        menu_layout.addWidget(self.TRG_MODE_widg, 16, 1)

        self.layout.addLayout(menu_layout, 2)


        self.layout.addLayout(menu_layout, 2)
        self.layout.addLayout(main_layout, 8)

        # Set up the main window
        self.widget = QWidget()
        self.widget.setLayout(self.layout)
        self.setCentralWidget(self.widget)

        # Set up the timer
        self.timer = QTimer()
        self.timer.setInterval(100) # Update the plot every 100 milliseconds
        self.timer.timeout.connect(self.plot)

        # Start the timer
        self.timer.start()
    

    # ## Functions for the menu
    def POL_changed(self, i):
        if i == 0:
            self.POL = 1
        elif i == 1:
            self.POL = 0
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", self.POL)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)

    def return_RESET_THRS(self):
        value = self.RESET_THRS_widg.text()
        self.RESET_THRS = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", self.RESET_THRS)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.RESET_THRS_widg.setPlaceholderText(str(self.RESET_THRS))
    
    def return_RESET_LEN(self):
        value = self.RESET_LEN_widg.text()
        self.RESET_LEN = int(value) 
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN", self.RESET_LEN)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.RESET_LEN_widg.setPlaceholderText(str(self.RESET_LEN))     
    
    def return_HIST(self):
        value = self.HIST_widg.text()
        self.HIST = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST", self.HIST)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.HIST_widg.setPlaceholderText(str(self.HIST))
    
    def return_INIB(self):
        value = self.INIB_widg.text()
        self.INIB = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB", self.INIB)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.INIB_widg.setPlaceholderText(str(self.INIB))

    def return_THRS(self):
        value = self.THRS_widg.text()
        self.THRS = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS", self.THRS)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.THRS_widg.setPlaceholderText(str(self.THRS))

    def return_AN_OFS(self):
        value = self.AN_OFS_widg.text()
        self.AN_OFS = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS", self.AN_OFS)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.AN_OFS_widg.setPlaceholderText(str(self.AN_OFS))

    def return_TRIGGER_K(self):
        value = self.TRIGGER_K_widg.text()
        self.TRIGGER_K = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", self.TRIGGER_K)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRIGGER_K_widg.setPlaceholderText(str(self.TRIGGER_K))

    def return_TRIGGER_M(self):
        value = self.TRIGGER_M_widg.text()
        self.TRIGGER_M = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", self.TRIGGER_M)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRIGGER_M_widg.setPlaceholderText(str(self.TRIGGER_M))

    def return_TRAP_K(self):
        value = self.TRAP_K_widg.text()
        self.TRAP_K = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", self.TRAP_K)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRAP_K_widg.setPlaceholderText(str(self.TRAP_K))
    
    def return_TRAP_M(self):
        value = self.TRAP_M_widg.text()
        self.TRAP_M = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", self.TRAP_M)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRAP_M_widg.setPlaceholderText(str(self.TRAP_M))

    def return_DECONV_M(self):
        value = self.DECONV_M_widg.text()
        self.TAU = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/self.TAU)-1)))
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.DECONV_M_widg.setPlaceholderText(str(self.TAU))

    def return_TRAP_GAIN(self):
        value = self.TRAP_GAIN_widg.text()
        self.TRAP_GAIN = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", self.TRAP_GAIN)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRAP_GAIN_widg.setPlaceholderText(str(self.TRAP_GAIN))

    def return_BLLEN(self):
        value = self.BLLEN_widg.text()
        self.BLLEN = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", self.BLLEN)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.BLLEN_widg.setPlaceholderText(str(self.BLLEN))

    def return_BLINIB(self):
        value = self.BLINIB_widg.text()
        self.BLINIB = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", self.BLINIB)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.BLINIB_widg.setPlaceholderText(str(self.BLINIB))

    def return_SAMP_POS(self):
        value = self.SAMP_POS_widg.text()
        self.SAMP_POS = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", self.SAMP_POS)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.SAMP_POS_widg.setPlaceholderText(str(self.SAMP_POS))
    
    def return_TRG_MODE(self):
        value = self.TRG_MODE_widg.text()
        self.TRG_MODE = int(value)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", self.TRG_MODE)
        res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)
        self.TRG_MODE_widg.setPlaceholderText(str(self.TRG_MODE))

    def gaussian(x, a, x0, sigma):
        return a * np.exp(-(x-x0)**2/(2*sigma**2))
    
    def set_centroid(self):
        self.centroid = self.centroid_in_box.text()
        self.centroid_in_box.setPlaceholderText(str(self.centroid))

    def plot(self):
        res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

        if res != 0:
            res, err = sdk.s_error(res)
            print("Allocate spectrum buffer error:", err)
            exit(-1)
        # Clear the plot
        self.canvas.axes.cla()
        self.update_plot(obSpectrum)
        self.canvas.draw()
    
    def update_plot(self, obSpectrum):
        centroid = float(self.centroid)
        res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0", obSpectrum)
        if res == 0:
            xar = []
            yar = []
            for index in range(obSpectrum.info.valid_bins):
                xar.append(index)
                yar.append(obSpectrum.data[index])
            self.canvas.axes.clear()
            self.canvas.axes.plot(xar, yar)

            if centroid != 0.0:
                a_guess = max(yar)
                x0_guess = centroid  # initial guess for centroid (mean)
                sigma_guess = 10  # initial guess for standard deviation

                min_bin = int(centroid - 300)
                max_bin = int(centroid + 300)
                x_fit = xar[min_bin:max_bin]
                y_fit = yar[min_bin:max_bin]
                self.sigma_fit = 1
                self.centroid_fit = centroid
                #popt, pcov = opt.curve_fit(self.gaussian, x_fit, y_fit, p0=[a_guess, x0_guess, sigma_guess])
                # Extract the fitted parameters
                #a_fit, self.centroid_fit, self.sigma_fit = popt
                #self.canvas.axes.axvline(x=self.centroid_fit, color='g', label='Centroid')
                # plot the fitted curve
                #self.canvas.axes.plot(x_fit, self.gaussian(x_fit, *popt), 'r-', label='fit')
                self.sigma_box.setText(str(self.sigma_fit))
                self.centroid_box.setText(str(self.centroid_fit))
    
    def start(self):
        self.timer.start()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

    def stop(self):
        self.timer.stop()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")

    def reset(self):
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")
    

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RealTimePlot()
    window.show()
    sys.exit(app.exec_())

