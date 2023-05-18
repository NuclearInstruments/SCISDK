import pyqtgraph.parametertree.parameterTypes as pTypes

################################################################################
## CHANNELS CLASS ##############################################################

class Channels_class(pTypes.GroupParameter):
    """Class for the oscilloscope's channels.
    It has get methods for every channel, and a method to update
    which channels are plotted.
    """
    CHANNELS = {
        "Signal": False,
        "Baseline": False,
        "Baseline hold interval": False,
        "Triggered signal": False,
        "Trigger": False,
        "Energy": False,
        "Gate integration": False,
    }

    def __init__(self, legend, o1, o2, o3, o4, o2d, o3d, o4d):
        """Constructor of the class: it takes as input the legend of the plot,
        the different plot items for the different data of the oscilloscope.
        It generate a Group parameter.
        """
        self.legend = legend
        self.PLOT_ITEMS = {
            "Signal": (o1, "Signal"),
            "Baseline": (o2, "Baseline"),
            "Baseline hold interval": (o2d, "Baseline hold interval"),
            "Triggered signal": (o3, "Triggered signal"),
            "Trigger": (o3d, "Trigger"),
            "Energy": (o4, "Energy"),
            "Gate integration": (o4d, "Gate integration"),
        }

        children = []
        for name, value in self.CHANNELS.items():
            children.append(dict(name=name, type='bool', value=value))
        
        pTypes.GroupParameter.__init__(self, name='Channels', children=children)

        # Connect the valueChanged signal of every child to the update_channel method
        self.child('Signal').sigValueChanged.connect(lambda signal = self.child('Signal'): self.update_channel(signal))
        self.child('Baseline').sigValueChanged.connect(lambda baseline = self.child('Baseline'): self.update_channel(baseline))
        self.child('Baseline hold interval').sigValueChanged.connect(lambda bs_hold = self.child('Baseline hold interval'): self.update_channel(bs_hold))
        self.child('Triggered signal').sigValueChanged.connect(lambda tr_sig = self.child('Triggered signal'): self.update_channel(tr_sig))
        self.child('Energy').sigValueChanged.connect(lambda en = self.child('Energy'): self.update_channel(en))
        self.child('Gate integration').sigValueChanged.connect(lambda gate = self.child('Gate integration'): self.update_channel(gate))
        self.child('Trigger').sigValueChanged.connect(lambda tr =  self.child('Trigger'): self.update_channel(tr))

    def update_channel(self, child):
        """Update the channel value in the dictionary;
        it also add or remove the plot item from the legend
        """
        name = child.name()
        value = child.value()
        if name not in self.CHANNELS:
            return

        plot_item, legend_name = self.PLOT_ITEMS[name]
        if value:
            self.legend.addItem(plot_item, legend_name)
        else:
            self.legend.removeItem(plot_item)

        self.CHANNELS[name] = value

    def get_channel_value(self, name):
        return self.CHANNELS[name] if name in self.CHANNELS else None


################################################################################
## REGISTER CLASS ##############################################################

class Registers_class(pTypes.GroupParameter):
    """Registers class get the registers and update them;
    it has two methods: update_register and save.
    """

    def __init__(self, register, sdk):
        """Constructor of the class: it takes as inputs the dictionary
        with the value of the registers and the sdk; 
        it generate a Group Parameter.
        """
    
        self.register = register
        self.sdk = sdk
        par = [
            dict(name='Polarity', type='list', limits=['Positive', 'Negative'], value='Positive'),
            dict(name='Trigger threshold', type='int', value=self.register["trigger_threshold"]),
            dict(name='Number of sample', type='int', value=self.register["number_of_samples"]),
            dict(name='Time hold baseline', type='int', value=self.register["time_hold_baseline"]),
            dict(name='Time integration', type='int', value=self.register["time_integration"]),
            dict(name='Pre-integration', type='int', value=self.register["pre_integration"]),
            dict(name='Gain', type='int', value=self.register["gain"], limits=[10, 65500]),
            dict(name='Offset spectrum', type='int', value=self.register["offset_integration"]),
            dict(name='Save State', type= 'action'),
        ]
        pTypes.GroupParameter.__init__(self, name='Registers', children=par)
        # Connect the sigActivated signal of the child 'Save State' to the save method
        self.child('Save State').sigActivated.connect(self.save)
        # Connect the sigValueChanged signal of every child to the update method
        self.child('Polarity').sigValueChanged.connect(lambda : self.update("polarity", self.child('Polarity').value()))
        self.child('Trigger threshold').sigValueChanged.connect(lambda : self.update("trigger_threshold", self.child('Trigger threshold').value()))
        self.child('Number of sample').sigValueChanged.connect(lambda : self.update("number_of_samples", self.child('Number of sample').value()))
        self.child('Time hold baseline').sigValueChanged.connect(lambda : self.update("time_hold_baseline", self.child('Time hold baseline').value()))
        self.child('Time integration').sigValueChanged.connect(lambda : self.update("time_integration", self.child('Time integration').value()))
        self.child('Pre-integration').sigValueChanged.connect(lambda : self.update("pre_integration", self.child('Pre-integration').value()))
        self.child('Gain').sigValueChanged.connect(lambda : self.update("gain", self.child('Gain').value()))
        self.child('Offset spectrum').sigValueChanged.connect(lambda : self.update("offset_integration", self.child('Offset spectrum').value()))


    def update(self, name, value):
        """Update the register value in the dictionary;
        it does't save the value in the skd registers
        (press save to do it)
        """
        if name in self.register:
            self.register[name] = value

            if name == "polarity":
                if value == "Positive":
                    self.register[name] = 1
                else:
                    self.register[name] = 0

    def save(self):
        """Save the register value in the sdk registers.
        """
        res = self.sdk.SetRegister("board0:/Registers/pol", self.register["polarity"])
        if res != 0:
            print("Error setting polarity")

        res = self.sdk.SetRegister("board0:/Registers/trgthrs", self.register["trigger_threshold"])
        if res != 0:
            print("Error setting trigger threshold")

        res = self.sdk.SetRegister("board0:/Registers/gain", self.register["gain"])
        if res != 0:
            print("Error setting gain")

        res = self.sdk.SetRegister("board0:/Registers/Nsample", self.register["number_of_samples"])
        if res != 0:
            print("Error setting number of samples")
            
        res = self.sdk.SetRegister("board0:/Registers/time_hold_bs", self.register["time_hold_baseline"])
        if res != 0:
            print("Error setting time hold baseline")

        res = self.sdk.SetRegister("board0:/Registers/time_integr", self.register["time_integration"])
        if res != 0:
            print("Error setting time integration")

        res = self.sdk.SetRegister("board0:/Registers/pre_integr", self.register["pre_integration"])
        if res != 0:
            print("Error setting pre-integration")

        res = self.sdk.SetRegister("board0:/Registers/Offset_int", self.register["offset_integration"])
        if res != 0:
            print("Error setting offset spectrum")
