using System.ComponentModel;
using System.Data;
using System.Net.NetworkInformation;
using System.Text;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using CSharp_SciSDK;
using Timer = System.Timers.Timer;
using System.Timers;



namespace Plot_spectrum
{
    public partial class Form1 : Form
    {
        private PlotModel model;
        private PlotView plotView;
        private Timer timer;
        private LineSeries series;
        
        private SciSDK sdk;
        int res;
        SciSDKSpectrumDecodedBuffer obSpectrum;
        int[] values;

        public Form1()
        {
            this.InitializeComponent();

            // Initialize scisdk library
            sdk = new SciSDK();

            // Add new device DT1260
            res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            // Print error message
            if (res != 0)
            {
                Console.WriteLine("Unable to connect to the device. Error " + res);
                Environment.Exit(-1);
            }

            // Set registers
            res = sdk.SetRegister("board0:/Registers/pol", 1);
            res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
            res = sdk.SetRegister("board0:/Registers/Nsample", 7);
            res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100);
            res = sdk.SetRegister("board0:/Registers/time_integr", 76);
            res = sdk.SetRegister("board0:/Registers/pre_integr", 5);
            res = sdk.SetRegister("board0:/Registers/gain", 30000);
            res = sdk.SetRegister("board0:/Registers/Offset_int", 0);

            if (res != 0)
            {
                Console.WriteLine("Error while trying to write register value");
            }

            // // SPECTRUM
            // Execute command
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");

            // Allocate buffer
            obSpectrum = new SciSDKSpectrumDecodedBuffer();
            res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", 0, ref obSpectrum);

            if (res != 0)
            {
                Console.WriteLine("Error allocating spectrum buffer");
                Environment.Exit(-1);
            }

            // Set parameters spectrum
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");
            if (res != 0)
            {
                Console.WriteLine("Error set spectrum parameters");
            }
            
            // // Plot the data
            model = new PlotModel();
            series = new LineSeries();
            model.Series.Add(series);

            // Set the axis titles
            model.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Title = "Counts" });
            model.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom });

            // Create a timer update the plot every 10 ms
            timer = new Timer(100);
            timer.Elapsed += Timer_Elapsed;

            //Create PlotView
            plotView = new PlotView();
            plotView.Model = model;
            plotView.Dock = DockStyle.Fill;
            Controls.Add(plotView);

            // Table of buttons
            TableLayoutPanel t1 = new TableLayoutPanel();
            t1.Dock= DockStyle.Bottom;
            t1.RowCount = 1;
            t1.ColumnCount = 3;
            t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));
            t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));
            t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));

            // Create the start button and set its properties
            var startButton = new Button();
            startButton.Text = "Start";
            startButton.Click += StartButton_Click;
            startButton.Dock = DockStyle.Fill;

            // Create the stop button and set its properties
            var stopButton = new Button();
            stopButton.Text = "Stop";
            stopButton.Click += StopButton_Click;
            stopButton.Dock = DockStyle.Fill;

            // Create the restart button and set its properties
            var restartButton = new Button();
            restartButton.Text = "Restart";
            restartButton.Click += RestartButton_Click;
            restartButton.Dock = DockStyle.Fill;

            // Set up the flow layout panel for the buttons
            t1.Controls.Add(startButton, 0, 0);
            t1.Controls.Add(stopButton, 1, 0);
            t1.Controls.Add(restartButton, 2, 0);

            Controls.Add(t1);
            timer.Start();

            if (res != 0)
            {
                Console.WriteLine("Error while trying to read spectrum value");
            }
        }

        private void StartButton_Click(object sender, System.EventArgs e)
        {
            timer.Start();
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");
        }

        private void StopButton_Click(object sender, System.EventArgs e)
        {
            timer.Stop();
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "");
        }

        private void RestartButton_Click(object sender, EventArgs e)
        {
            timer.Stop();
            series.Points.Clear();
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");
            timer.Start();
        }

        private void Timer_Elapsed(object? sender, ElapsedEventArgs e)
        {
            res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", ref obSpectrum);
            values = new int[obSpectrum.info.valid_bins];

            if (res == 0)
            {
                for (int i = 0; i < obSpectrum.info.valid_bins; i++)
                {
                    values[i] = (int)obSpectrum.data[i];
                }

                // Update the line series with the new data points
                series.Points.Clear();
                for (int i = 0; i < values.Length; i++)
                {
                    series.Points.Add(new DataPoint(i, values[i]));
                    //plotView.InvalidatePlot(true);
                }

                // Update the plot view
                model.InvalidatePlot(true);
            }
        }
    }
}
