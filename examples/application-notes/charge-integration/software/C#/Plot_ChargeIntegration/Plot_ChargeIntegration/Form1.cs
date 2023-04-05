using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.WindowsForms;
using CSharp_SciSDK;
using System.Windows.Forms.DataVisualization.Charting;
using System.Net.NetworkInformation;
using System.Threading;
using Timer = System.Timers.Timer;
using System.Timers;

namespace Plot_ChargeIntegration
{
    public partial class Form1 : Form
    {
        private SciSDK sdk;
        int res;
        SciSDKSpectrumDecodedBuffer obSpectrum;
        private Chart chart;
        private Timer timer;
        int i = 0;
        private CategoryAxis categoryAxis;
        private LinearAxis valueAxis;
        
        public Form1()
        {
            InitializeComponent();
            // Initialize the plot model and series
            

            // Initialize scisdk library and add device
            sdk = new SciSDK();
            res = sdk.AddNewDevice("usb:13250", "dt1260", "registerfile.json", "board0");
            if (res != 0)
            {
                Console.WriteLine("Error in adding the device");
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

            if (res == 0)
            {
                Console.WriteLine("Register value has been successully set");
            }
            else
            {
                Console.WriteLine("Error while trying to write register value");
            }

            // // SPECTRUM
            // Set parameters spectrum
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");

            if (res != 0)
            {
                Console.WriteLine("Error set parameters");
            }

            // Execute command reset
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");

            // Execute command start
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");

            // Allocate buffer
            obSpectrum = new SciSDKSpectrumDecodedBuffer();
            res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", 0, ref obSpectrum);

            if (res != 0)
            {
                Console.WriteLine("Error allocating buffer");
                Environment.Exit(-1);
            }
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");

            // Create a timer update the plot every 10 ms
            timer = new Timer(1);
            timer.Elapsed += Timer_Elapsed;

            // Plot data
            chart = new Chart();
            chart.ChartAreas.Add(new ChartArea());
            chart.Series.Add(new Series());
            chart.Series[0].ChartType = SeriesChartType.Line;

            timer.Start();

            Application.Run(chart);

            res = sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", 0, ref obSpectrum);
            // Create a table pannel for the button
            //TableLayoutPanel t1 = new TableLayoutPanel();
            //t1.Dock = DockStyle.Bottom;
            //t1.RowCount = 1;
            //t1.ColumnCount = 3;
            //t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));
            //t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));
            //t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.33f));

            //// Create the start button and set its properties
            //var startButton = new Button();
            //startButton.Text = "Start";
            //startButton.Click += StartButton_Click;
            //startButton.Dock = DockStyle.Fill;

            //// Create the stop button and set its properties
            //var stopButton = new Button();
            //stopButton.Text = "Stop";
            //stopButton.Click += StopButton_Click;
            //stopButton.Dock = DockStyle.Fill;

            //// Create the restart button and set its properties
            //var restartButton = new Button();
            //restartButton.Text = "Restart";
            //restartButton.Click += RestartButton_Click;
            //restartButton.Dock = DockStyle.Fill;

            //// Set up the flow layout panel for the buttons
            //t1.Controls.Add(startButton, 0, 0);
            //t1.Controls.Add(stopButton, 1, 0);
            //t1.Controls.Add(restartButton, 2, 0);

            //Controls.Add(t1);
            timer.Start();
        }

        private void StartButton_Click(object sender, System.EventArgs e)
        {
            // Start the timer
            timer.Start();
            res = sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");
        }

        private void StopButton_Click(object sender, System.EventArgs e)
        {
            // Stop the timer
            timer.Stop();
            res = sdk.ExecuteCommand("board0:/MMCComponets/Spectrum_0.stop", "");
        }

        private void RestartButton_Click(object sender, System.EventArgs e)
        {
            res = sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "");
            res = sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");
            res = sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");
            i = 0;
            
            timer.Start();
        }
        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", ref obSpectrum);
            if (res == 0)
            {
                double[] x = new double[obSpectrum.info.valid_bins];
                double[] y = new double[obSpectrum.info.valid_bins];

                for (int i = 0; i < obSpectrum.info.valid_bins; i++)
                {
                    x[i] = i;
                    y[i] = obSpectrum.data[i];
                };
                chart.Series[0].Points.DataBindXY(x, y);
            }
        }
    }
}
