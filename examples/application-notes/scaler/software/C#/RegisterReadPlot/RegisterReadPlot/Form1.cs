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

            //Initialize scisdk library and add device
            sdk = new SciSDK();
            res = sdk.AddNewDevice("usb:13250", "dt1260", "registerfile.json", "board0");
            const int ni_ok = 0;

            if (res != ni_ok) { Environment.Exit(-1); }

            // Set registers
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

            // Create a table pannel for the button
            TableLayoutPanel t1 = new TableLayoutPanel();
            t1.Dock = DockStyle.Bottom;
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
