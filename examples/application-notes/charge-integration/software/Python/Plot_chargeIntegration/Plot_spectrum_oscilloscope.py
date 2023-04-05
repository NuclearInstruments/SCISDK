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

# ## OSCILLOSCOPE

# set oscilloscope parameters
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decoded")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 2000)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","ext")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 2000)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
decimator = 1
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000)

# ## SPECTRUM

# Set spectrum parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")

sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# Define global buffer
global bufSpectrum
global bufOscilloscope

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
        self.channel0 = 0
        self.channel1 = 0
        self.channel2 = 0
        self.channel3 = 0
        
        # Set up the canvas
        self.canvas = MyCanvas(self, width=5, height=4, dpi=100)
        self.canvas1 = MyCanvas(self, width=5, height=4, dpi=100)
        self.canvas2 = MyCanvas(self, width=5, height=2, dpi=100)
        self.canvas3 = MyCanvas(self, width=5, height=2, dpi=100)
        self.canvas4 = MyCanvas(self, width=5, height=2, dpi=100)
        
        self.show()

        # Set up oscilloscope channel buttons
        self.ch0_button = QCheckBox("CH0 - Signal")
        self.ch0_button.setFont(QFont("Ariel", 10))
        self.ch0_button.stateChanged.connect(self.ch0)
        self.ch1_button = QCheckBox("CH1 - Baseline")
        self.ch1_button.setFont(QFont("Ariel", 10))
        self.ch1_button.stateChanged.connect(self.ch1)
        self.ch2_button = QCheckBox("CH2 - Triggered signal")
        self.ch2_button.setFont(QFont("Ariel", 10))
        self.ch2_button.stateChanged.connect(self.ch2)
        self.ch3_button = QCheckBox("CH3 - Charge")
        self.ch3_button.setFont(QFont("Ariel", 10))
        self.ch3_button.stateChanged.connect(self.ch3)
 

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
        self.layout = QHBoxLayout()

        # Set up the main window
        main_layout = QVBoxLayout()
        main_layout.addWidget(self.canvas1, 3)
        main_layout.addWidget(self.canvas2, 1)
        main_layout.addWidget(self.canvas3, 1)
        main_layout.addWidget(self.canvas4, 1)
        button_osc_layout = QHBoxLayout()
        button_osc_layout.addWidget(self.ch0_button)
        button_osc_layout.addWidget(self.ch1_button)
        button_osc_layout.addWidget(self.ch2_button)
        button_osc_layout.addWidget(self.ch3_button)
        main_layout.addLayout(button_osc_layout)
        main_layout.addWidget(self.canvas, 3)
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
        self.layout.addLayout(main_layout, 8)

        # Set up the main window
        self.widget = QWidget()
        self.widget.setLayout(self.layout)
        self.setCentralWidget(self.widget)

        # Set up the timer
        self.timer = QTimer()
        self.timer.setInterval(100) # Update the plot every 100 milliseconds
        self.timer.timeout.connect(self.plot_spectrum)
        self.timer.timeout.connect(self.plot_oscilloscope)

        # Start the timer
        self.timer.stop()
        

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
     

    # ## Functions for the plot
    # Plot the spectrum
    def plot_spectrum(self):
        res, bufSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

        if res != 0:
            res, err = sdk.s_error(res)
            print("Allocate spectrum buffer error:", err)
            exit(-1)
        # Clear the plot
        self.canvas.axes.cla()
        self.update_plot_sct(bufSpectrum)
        self.canvas.draw()
    
    def update_plot_sct(self, bufSpectrum):
        res, bufSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0", bufSpectrum)
        if res == 0:
            xar = []
            yar = []
            for index in range(bufSpectrum.info.valid_bins):
                xar.append(index)
                yar.append(bufSpectrum.data[index])
        self.canvas.axes.clear()
        self.canvas.axes.plot(xar, yar)

    # Plot the oscilloscope    
    def plot_oscilloscope(self):
        res, bufOscilloscope = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")
        if res != 0:
            res, err = sdk.s_error(res)
            print("Allocate oscilloscope buffer error:", err)
            exit(-1)
        self.canvas1.axes.cla()
        self.canvas2.axes.cla()
        self.canvas3.axes.cla()
        self.canvas4.axes.cla()
        self.update_plot_osc(bufOscilloscope)
        self.canvas1.draw()
        self.canvas2.draw()
        self.canvas3.draw()
        self.canvas4.draw()

    def update_plot_osc(self, bufOscilloscope):
        res, bufOscilloscope= sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", bufOscilloscope)
        if res == 0:
            x, y0, y1, y2, y3 = [], [], [], [], []
            y11, y12, y21, y31, y32, y33 = [], [], [], [], [], []

            offset_channel_dig = bufOscilloscope.info.samples_digital * bufOscilloscope.info.tracks_digital_per_channel
            offset_channel_an = bufOscilloscope.info.samples_analog

            for index in range(bufOscilloscope.info.samples_analog):
                x.append(index * decimator)
                y0.append(bufOscilloscope.analog[index])
                y1.append(bufOscilloscope.analog[index + offset_channel_an * 1])
                y2.append(bufOscilloscope.analog[index + offset_channel_an * 2])
                y3.append(bufOscilloscope.analog[index + offset_channel_an * 3])
                y11.append((bufOscilloscope.digital[index + offset_channel_dig*1]) )
                y12.append((bufOscilloscope.digital[index + offset_channel_dig*1 + bufOscilloscope.info.samples_digital]) )       
                y21.append((bufOscilloscope.digital[index + offset_channel_dig*2]) )             
                y31.append((bufOscilloscope.digital[index + offset_channel_dig*3]) )
                y32.append((bufOscilloscope.digital[index + offset_channel_dig*3 + bufOscilloscope.info.samples_digital]) )
                y33.append((bufOscilloscope.digital[index + offset_channel_dig*3 + bufOscilloscope.info.samples_digital*2]) )

            self.canvas1.axes.clear()
            self.canvas2.axes.clear()
            self.canvas3.axes.clear()
            self.canvas4.axes.clear()

            if self.channel0 == 1:               
                self.canvas1.axes.plot(x, y0, color="dodgerblue", label="Signal")
            if self.channel1 == 1:
                self.canvas1.axes.plot(x, y1, color="darkorange", label="Baseline")
                self.canvas2.axes.plot(x, y11, color="darkorange", label="Valid BL")
                self.canvas3.axes.plot(x, y12, color="darkorange", label="Run/hold BL")
            if self.channel2 == 1:
                self.canvas1.axes.plot(x, y2, color="forestgreen", label="Triggered \n signal")
                self.canvas2.axes.plot(x, y21, color="forestgreen", label="Trigger")
            if self.channel3 == 1:
                self.canvas1.axes.plot(x, y3, color="crimson", label="Charge")
                self.canvas2.axes.plot(x, y31, color="crimson", label="Valid Charge")
                self.canvas3.axes.plot(x, y32, color="crimson", label="Time integr")
                self.canvas4.axes.plot(x, y33, color="crimson", label="Pileup")
            
            self.canvas1.axes.legend(loc='upper left', bbox_to_anchor=(1, 1), fancybox=True, shadow=True)
            self.canvas2.axes.legend(loc='upper left', bbox_to_anchor=(1, 1), fancybox=True, shadow=True)
            self.canvas3.axes.legend(loc='upper left', bbox_to_anchor=(1, 1), fancybox=True, shadow=True)
 
    # ## Buttons
    def start(self):
        self.timer.start()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

    def stop(self):
        self.timer.stop()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")

    def reset(self):
        self.timer.stop()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")
        self.timer.start()
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

    def ch0(self):
        if self.ch0_button.isChecked():
            self.channel0 = 1
        else:
            self.channel0 = 0
    
    def ch1(self):
        if self.ch1_button.isChecked():
            self.channel1 = 1
        else:
            self.channel1 = 0

    def ch2(self):
        if self.ch2_button.isChecked():
            self.channel2 = 1
        else:
            self.channel2 = 0
    
    def ch3(self):
        if self.ch3_button.isChecked():
            self.channel3 = 1
        else:
            self.channel3 = 0

# Create the application
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RealTimePlot()
    window.show()
    sys.exit(app.exec_())
