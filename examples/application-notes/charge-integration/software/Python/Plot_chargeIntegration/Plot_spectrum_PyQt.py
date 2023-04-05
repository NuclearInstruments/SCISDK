from scisdk.scisdk import SciSDK
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.animation as animation

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", 1)
res = sdk.SetRegister("board0:/Registers/trgthrs", 2000)
res = sdk.SetRegister("board0:/Registers/Nsample", 7)
res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100)
res = sdk.SetRegister("board0:/Registers/time_integr", 76)
res = sdk.SetRegister("board0:/Registers/pre_integr", 5)
res = sdk.SetRegister("board0:/Registers/gain", 30000)
res = sdk.SetRegister("board0:/Registers/Offset_int", 0)

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
        self.pol = 1
        self.trgthrs = 2000
        self.Nsample = 7
        self.time_hold_bs = 100
        self.time_integr = 76
        self.pre_integr = 5
        self.gain = 30000
        self.Offset_int = 0

        self.canvas = MyCanvas(self, width=5, height=4, dpi=100)
        self.setCentralWidget(self.canvas)
        self.plot()
        self.show()

        # Set up the layout
        self.layout = QHBoxLayout()

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
        main_layout.addWidget(self.canvas)
        button_layout = QHBoxLayout()
        button_layout.addWidget(self.start_button)
        button_layout.addWidget(self.stop_button)
        button_layout.addWidget(self.reset_button)
        main_layout.addLayout(button_layout)

        # Set up the menu
        menu_layout = QGridLayout()
        polarity_name = QLabel("Polarity: ")
        polarity_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(polarity_name, 0, 0)
        trg_name = QLabel("Trigger threshold: ")
        menu_layout.addWidget(trg_name, 1, 0)
        nsample_name = QLabel("Nsample: ")
        nsample_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(nsample_name, 2, 0)
        time_hold_name = QLabel("Time hold: ")
        time_hold_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(time_hold_name, 3, 0)
        time_integr_name = QLabel("Time integration: ")
        time_integr_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(time_integr_name, 4, 0)
        pre_integr_name = QLabel("Pre integration: ")
        pre_integr_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(pre_integr_name, 5, 0)
        gain_name = QLabel("Gain: ")
        gain_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(gain_name, 6, 0)
        offset_name = QLabel("Offset: ")
        offset_name.setFont(QFont("Ariel", 10))
        menu_layout.addWidget(offset_name, 7, 0)

        polarity = QComboBox()
        polarity.addItems(["Positive", "Negative"])
        polarity.setFont(QFont("Ariel", 10))
        polarity.currentIndexChanged.connect(self.pol_changed)
        menu_layout.addWidget(polarity, 0, 1)
        self.trgthrs_widg = QLineEdit()
        self.trgthrs_widg.setMaxLength(10)
        self.trgthrs_widg.setPlaceholderText(str(self.trgthrs))
        self.trgthrs_widg.setFont(QFont("Ariel", 10))
        self.trgthrs_widg.returnPressed.connect(self.return_trigger)
        menu_layout.addWidget(self.trgthrs_widg, 1, 1)
        self.Nsample_widg = QLineEdit()
        self.Nsample_widg.setMaxLength(10)
        self.Nsample_widg.setPlaceholderText(str(self.Nsample))
        self.Nsample_widg.setFont(QFont("Ariel", 10))
        self.Nsample_widg.returnPressed.connect(self.return_Nsample)
        menu_layout.addWidget(self.Nsample_widg, 2, 1)
        self.time_hold_widg = QLineEdit()
        self.time_hold_widg.setMaxLength(10)
        self.time_hold_widg.setPlaceholderText(str(self.time_hold_bs))
        self.time_hold_widg.setFont(QFont("Ariel", 10))
        self.time_hold_widg.returnPressed.connect(self.return_timeh_baseline)
        menu_layout.addWidget(self.time_hold_widg, 3, 1)
        self.time_integr_widg = QLineEdit()
        self.time_integr_widg.setMaxLength(10)
        self.time_integr_widg.setPlaceholderText(str(self.time_integr))
        self.time_integr_widg.setFont(QFont("Ariel", 10))
        self.time_integr_widg.returnPressed.connect(self.return_time_int)
        menu_layout.addWidget(self.time_integr_widg, 4, 1)
        self.pre_integr_widg = QLineEdit()
        self.pre_integr_widg.setMaxLength(10)
        self.pre_integr_widg.setPlaceholderText(str(self.pre_integr))
        self.pre_integr_widg.setFont(QFont("Ariel", 10))
        self.pre_integr_widg.returnPressed.connect(self.return_pre_int)
        menu_layout.addWidget(self.pre_integr_widg, 5, 1)
        self.gain_widg = QLineEdit()
        self.gain_widg.setMaxLength(10)
        self.gain_widg.setPlaceholderText(str(self.gain))
        self.gain_widg.setFont(QFont("Ariel", 10))
        self.gain_widg.returnPressed.connect(self.return_gain)
        menu_layout.addWidget(self.gain_widg, 6, 1)
        self.offset_widg = QLineEdit()
        self.offset_widg.setMaxLength(10)
        self.offset_widg.setPlaceholderText(str(self.Offset_int))
        self.offset_widg.setFont(QFont("Ariel", 10))
        self.offset_widg.returnPressed.connect(self.return_offset)
        menu_layout.addWidget(self.offset_widg, 7, 1)
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
    def pol_changed(self, i):
        if i == 0:
            self.pol = 1
        elif i == 1:
            self.pol = 0
        res = sdk.SetRegister("board0:/Registers/pol", self.pol)
    def return_trigger(self):
        value = self.trgthrs_widg.text()
        self.trgthrs = int(value)
        res = sdk.SetRegister("board0:/Registers/trgthrs", self.trgthrs)
        self.trgthrs_widg.setPlaceholderText(str(self.trgthrs))
    
    def return_Nsample(self):
        value = self.Nsample_widg.text()
        self.Nsample = int(value)
        res = sdk.SetRegister("board0:/Registers/Nsample", self.Nsample)
        self.Nsample_widg.setPlaceholderText(str(self.Nsample))
    
    def return_timeh_baseline(self):
        value = self.time_hold_widg.text()
        self.time_hold_bs = int(value)
        res = sdk.SetRegister("board0:/Registers/time_hold_bs", self.time_hold_bs)
        self.time_hold_widg.setPlaceholderText(str(self.time_hold_bs))
    
    def return_time_int(self):
        value = self.time_integr_widg.text()
        self.time_integr = int(value)
        res = sdk.SetRegister("board0:/Registers/time_integr", self.time_integr)
        self.time_integr_widg.setPlaceholderText(str(self.time_integr))

    def return_pre_int(self):
        value = self.pre_integr_widg.text()
        self.pre_integr = int(value)
        res = sdk.SetRegister("board0:/Registers/pre_integr", self.pre_integr)
        self.pre_integr_widg.setPlaceholderText(str(self.pre_integr))
    def return_gain(self):
        value = self.gain_widg.text()
        self.gain = int(value)
        res = sdk.SetRegister("board0:/Registers/gain", self.gain)
        self.gain_widg.setPlaceholderText(str(self.gain))
    
    def return_offset(self):
        value = self.offset_widg.text()
        self.Offset_int = int(value)
        res = sdk.SetRegister("board0:/Registers/Offset_int", self.Offset_int)
        self.offset_widg.setPlaceholderText(str(self.Offset_int))

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
        res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0", obSpectrum)
        if res == 0:
            xar = []
            yar = []
            for index in range(obSpectrum.info.valid_bins):
                xar.append(index)
                yar.append(obSpectrum.data[index])
        self.canvas.axes.clear()
        self.canvas.axes.plot(xar, yar)
    
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

