using OxyPlot;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using CSharp_SciSDK;
using Timer = System.Timers.Timer;
using System.Net.NetworkInformation;

namespace RegisterReadPlot
{
    public partial class Form1 : Form
    {
        private PlotModel model;
        private Timer timer;
        private LineSeries series;
        int i =0;
        private SciSDK sdk;
        int res;
        int value = 0;

        public Form1()
        {
            this.InitializeComponent();
            // Initialize the plot model and series
            model = new PlotModel();
            series= new LineSeries();
            model.Series.Add(series);

            sdk= new SciSDK();
            res = sdk.AddNewDevice("usb:13250", "dt1260", "registerfile.json", "board0");
            const int ni_ok = 0;

            if (res != ni_ok) { Environment.Exit(-1); }

            res = sdk.SetRegister("board0:/Registers/trgthrs", 2500);
            res = sdk.SetRegister("board0:/Registers/pol", 1);

            // Create a timer update the plot every 10 ms
            timer = new Timer(10);
            timer.Elapsed += Timer_Elapsed;

            // Set up the plot view
            var plotView = new PlotView();
            plotView.Dock = DockStyle.Fill;
            plotView.Model = model;
            Controls.Add(plotView);

            // Create the start button and set its properties
            var startButton = new Button();
            startButton.Text = "Start";
            startButton.Dock = DockStyle.Bottom;
            startButton.Click += StartButton_Click;
            Controls.Add(startButton);

            // Create the stop button and set its properties
            var stopButton = new Button();
            stopButton.Text = "Stop";
            stopButton.Dock = DockStyle.Bottom;
            stopButton.Click += StopButton_Click;
            Controls.Add(stopButton);

            // Create the restart button and set its properties
            var restartButton = new Button();
            restartButton.Text = "Restart";
            restartButton.Dock = DockStyle.Bottom;
            restartButton.Click += RestartButton_Click;
            Controls.Add(restartButton);
        }

        private void StartButton_Click(object sender, System.EventArgs e)
        {
            // Start the timer
            timer.Start();
        }

        private void StopButton_Click(object sender, System.EventArgs e)
        {
            // Stop the timer
            timer.Stop();
        }

        private void RestartButton_Click(object sender, EventArgs e)
        {
            series.Points.Clear();
            i = 0;
            model.InvalidatePlot(true);
            timer.Start();
        }

        private void Timer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            res = sdk.GetRegister("board0:/Registers/trgcnt", out value);

            this.Invoke(new Action(() =>
            {
                series.Points.Add(new DataPoint(i++, value));
                model.InvalidatePlot(true);
            }));
        }
    }
}
