import pyqtgraph as pg
from pyqtgraph.parametertree import parameterTypes as pTypes

###############################################################################
###### ANALYSIS CLASS #########################################################

class Analysis(pTypes.GroupParameter):
    """Class for the analysis of the spectrum.
    It has get/set methods for every parameter.
    This class generate a Group parameter.
    It is associated to a LinearRegionItem in che Line class.
    """
    def __init__(self):
        """Constructor of the class: it generate a Group parameter.
        """
        self.centroid = 0.0
        self.centroid_keV = 0.0
        self.sigma = 0.0
        self.sigma_keV = 0.0
        self.resolution = 0.0
        self.resolution_keV = 0.0
        pTypes.GroupParameter.__init__(self, name='Parameters', type='group', children=[
            dict(name='Centroid', type='str', value=self.centroid, readonly=True),
            dict(name='Sigma', type='str', value=self.sigma, readonly=True),
            dict(name='Resolution', type='str', value=self.resolution, readonly=True),
        ])

    # Set methods
    def setCentroid(self, centroid):
        self.centroid = centroid
        if self.centroid_keV == 0.0:
            self.child('Centroid').setValue('{:,.3f}'.format(centroid))
        else:
            self.child('Centroid').setValue('{:,.3f} (%s keV)'.format(centroid) % self.centroid_keV)
    def setCentroid_keV(self, centroid_keV):
        self.centroid_keV = centroid_keV
        self.child('Centroid').setValue('{:,.3f} (%s keV)'.format(self.centroid) % centroid_keV)
    def setSigma(self, sigma):
        self.sigma = sigma
        if self.sigma_keV == 0.0:
            self.child('Sigma').setValue('{:,.5}'.format(sigma))
        else:
            self.child('Sigma').setValue('{:,.5} (%s keV)'.format(sigma) % self.sigma_keV)
    def setSigma_keV(self, sigma_keV):
        self.sigma_keV = sigma_keV
        self.child('Sigma').setValue('{:,.5} (%s keV)'.format(self.sigma) % sigma_keV)
    def setResolution(self, resolution):
        self.resolution = resolution
        if self.resolution_keV == 0.0:
            self.child('Resolution').setValue('{:,.5}'.format(resolution))
        else:
            self.child('Resolution').setValue('{:,.5} (%s)'.format(resolution) % self.resolution_keV)
    def setResolution_keV(self, resolution_keV):
        self.resolution_keV = resolution_keV
        self.child('Resolution').setValue('{:,.5} (%s)'.format(self.resolution) % resolution_keV)
    
    # Get methods
    def getCentroid(self):
        return self.centroid
    def getCentroid_keV(self):
        return self.centroid_keV
    def getSigma(self):
        return self.sigma
    def getResolution(self):
        return self.resolution

###############################################################################
###### LINE CLASS #############################################################

class Line(pTypes.SimpleParameter):
    """Class for the lines of the spectrum. It generate a Simple parameter.
    Every line has an associated Analysis class, which is a Group parameter;
    a plot, which is the fit of the spectrum; a peak, which is the peak of the gaussian fit;
    an interval which is the edge of the LinearRegionItem associated to the line.
    
    It has get/set methods for every parameter.
    
    It is used to generate a Group parameter in the Region class.
    """
    def __init__(self, name, type, value):
        self.analysis = Analysis()
        self.interval = None
        self.num = 0
        self.plot = None
        self.peak = 0
        pTypes.SimpleParameter.__init__(self, name=name, type=type, value=value, children = [self.analysis], expanded=False)
    # Set methods
    def setAnalysis(self, centroid, sigma, resolution):
        self.analysis.setCentroid(centroid)
        self.analysis.setSigma(sigma)
        self.analysis.setResolution(resolution)
    def setInterval(self, interval):
        self.interval = interval
    def setNum(self, num):  
        self.num = num
    def setPlot(self, plot):
        self.plot = plot
    def setPeak(self, peak):
        self.peak = peak
        self.analysis.setCentroid_keV(peak)
    # Get methods
    def getAnalysis(self):
        return self.analysis
    def getInterval(self):
        return self.interval
    def getNum(self):
        return self.num
    def getPlot(self):
        return self.plot
    def getPeak(self):
        return self.peak

###############################################################################
###### REGION CLASS ###########################################################

class Region(pTypes.GroupParameter):
    """Class for the regions: LinearRegionItem. It generate a Group parameter of Line class.
    It has a plot associated to it, which is the plot of the spectrum.
    It has two methods: addLine, to add a line in the list,
    and updateLines, it creates the LinearRegionItem.
    """
    def __init__(self, name, num_lines, plot):
        """Constructor of the class: it generate a Group parameter of Line class.
        """
        self.num_lines = num_lines
        self.lines = []
        self.plot = plot
        for i in range(1, num_lines+1):
            line = Line(name=f'Line {i}', type='bool', value=False)
            line.setNum(i)
            line.sigValueChanged.connect(lambda line=line: self.updateLines(line, self.plot))
            self.lines.append(line)
        pTypes.GroupParameter.__init__(self, name=name, type='group', children=self.lines)

    def addLine(self, plot):
        """Add a line in the list of lines.
        """
        self.num_lines += 1
        line = Line(name=f'Line {self.num_lines}', type='bool', value=False)
        line.setNum(self.num_lines)
        line.sigValueChanged.connect(lambda line=line: self.updateLines(line, plot))
        self.addChild(line)
        self.lines.append(line)
    
    def updateLines(self, line, plot):
        """Update the LinearRegionItem associated to the line.
        """
        value = line.value()
        if value:
            line.setOpts(expanded=value)

            line.setInterval(pg.LinearRegionItem([0, 1000], bounds=[0,16384], movable=True))
            print(line.getInterval())
            plot.addItem(line.getInterval())
            label = pg.InfLineLabel(line.getInterval().lines[1], f'line {line.getNum()}' , position=0.95, rotateAxis=(1,0), anchor=(1, 1))
        else:
            line.setOpts(expanded=value)
            plot.removeItem(line.getInterval())


###############################################################################
###### PEAK CLASS ############################################################

class Peak(pTypes.ListParameter):
    """Class for the peaks of the spectrum. It generate a List parameter 
    with the name of the possible lines to choose to do the calibration.
    It has a region associated to it, which is the LinearRegionItem. 
    
    Every item in the list is associated to a Line class.
    It has three methods: update_limits, which update the list of the possible lines;
    on_list_param_change, which is the method called when the user choose a line;
    and change_peak_value, which sets the value of the peak with which do the calibration.
    """
    def __init__(self, num, region):
        """Constructor of the class: it generate a List parameter
        with the name of the possible lines to choose to do the calibration.
        """
        self.energy_peak = 0
        self.region = region
        self.selected_line = None
        values = ["Choose a line"] + [line.name() for line in region.lines]
        pTypes.ListParameter.__init__(
            self,
            name=f"Peak {num}",
            limits=values,
            type="list",
            children=[dict(name='Energy: ', type="str", value=self.energy_peak)],
            expanded=False
        )
        # Connect the sigActivated signal when a parameter in the list is selected
        self.sigValueChanged.connect(self.on_list_param_change)
        # Connect the sigActivated signal of the "Add line" of region to set new limits of the list
        region.param('Add a Line').sigActivated.connect(self.update_limits)

    def update_limits(self):
        """Update the list of the possible lines to choose to do the calibration.
        """
        values = ["Choose a line"] + [line.name() for line in self.region.lines]
        self.setLimits(values)
    
    def on_list_param_change(self, param, changes):
        """Method called when the user choose a line.
        """
        if param.value() == "Choose a line":
            self.setOpts(expanded=False)
            self.energy_peak = 0
            self.children()[0].setValue(self.energy_peak)
            self.selected_line = None

        else:
            self.setOpts(expanded=True)

            for line in self.region.lines:
                if line.name() == param.value():
                    self.energy_peak = line.getPeak()
                    self.children()[0].setValue(self.energy_peak)
                    self.selected_line = line
    
    def change_peak_value(self, param):
        """Set the value of the peak with which do the calibration.
        """
        self.energy_peak = param.value()
        if self.selected_line is not None:
            self.selected_line.setPeak(self.energy_peak)
            self.children()[0].setValue(self.energy_peak)

###############################################################################
###### PEAKS CLASS ###########################################################

class Peaks(pTypes.GroupParameter):
    """Class for the peaks: it generate a Group parameter of Peak class.
    It has two Peak parameters, Peak 1 and Peak 2, and the calibration button.
    """
    def __init__(self, params_line):
        """Constructor of the class: it generate a Group parameter of Peak class, 
        plus a calibration action.
        It as two internal parameters, a and b, which are the parameters of the 
        linear calibration.
        """
        self.a = 0
        self.b = 0
        pTypes.GroupParameter.__init__(
            self, 
            name="Calibration", 
            type='group', 
            children=[Peak(1, params_line), Peak(2, params_line), dict(name='Calibrate', type= 'action'),]
            )
        # Connect the sigActivated signal of the child 'Calibrate' to the calibrate method
        self.param("Calibrate").sigActivated.connect(self.calibrate)

        # Connect the sigValueChanged signal of the child 'Energy: ' to the change_peak_value method
        self.peak1 = self.param("Peak 1").child('Energy: ')
        self.peak2 = self.param("Peak 2").child('Energy: ')

        self.peak1.sigValueChanged.connect(lambda param: self.param('Peak 1').change_peak_value(param))
        self.peak2.sigValueChanged.connect(lambda param: self.param('Peak 2').change_peak_value(param))
    
    def calibrate(self):
        """Calibrate the spectrum using the two peaks.
        """
        energy1 = float(self.peak1.value())
        energy2 = float(self.peak2.value())
        x0Fit1 = float(self.param("Peak 1").selected_line.getAnalysis().getCentroid())
        x0Fit2 = float(self.param("Peak 2").selected_line.getAnalysis().getCentroid())

        if energy1 != 0 and energy2 != 0:
            self.a = (energy2 - energy1)/(x0Fit2 - x0Fit1)
            self.b = (energy1*x0Fit2 - energy2*x0Fit1)/(x0Fit2 - x0Fit1)
            