using CSharp_SciSDK;
using Newtonsoft.Json.Linq;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using PlotView = OxyPlot.WindowsForms.PlotView;
using OxyPlot.Wpf;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Printing;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Controls;
using System.Windows.Forms;
using Button = System.Windows.Forms.Button;
using CheckBox = System.Windows.Forms.CheckBox;
using ComboBox = System.Windows.Forms.ComboBox;
using TextBox = System.Windows.Forms.TextBox;
using Label = System.Windows.Forms.Label;
using Microsoft.VisualBasic;
using Timer = System.Timers.Timer;

namespace Plot_Oscilloscope
{
    public partial class Form1 : Form
    {
        private SciSDK sdk;
        private int res = 0;

        private int polarity = 1;
        private int trigger_threshold = 2000;
        private int Nsample = 7;
        private int time_hold_bs = 100;
        private int time_integr = 76;
        private int pre_integr = 5;
        private int gain = 30000;
        private int Offset_int = 0;

        private ComboBox pol;
        private TextBox trigger;
        private TextBox N_samples;
        private TextBox hold_bl;
        private TextBox int_integr;
        private TextBox pre_tr;
        private TextBox Gain;
        private TextBox offset;

        private int decimator = 1;
        SciSDKOscilloscopeDecodedBuffer buffer;
        
        private PlotView plot_view1;
        private PlotView plot_view2;
        private PlotModel model1;
        private PlotModel model2;
        private FunctionSeries analog0;
        private FunctionSeries analog1;
        private FunctionSeries analog2;
        private FunctionSeries analog3;
        private FunctionSeries digital0_1;
        private FunctionSeries digital1_1;
        private FunctionSeries digital0_2;
        private FunctionSeries digital0_3;
        private FunctionSeries digital1_3;
        private FunctionSeries digital2_3;
        private Timer timer;
        
        private bool channel0 = false;
        private bool channel1 = false;
        private bool channel2 = false;
        private bool channel3 = false;

        private CheckBox channel_0;
        private CheckBox channel_1;
        private CheckBox channel_2;
        private CheckBox channel_3;

        public Form1()
        {
            InitializeComponent();

            // Initialize Scisdk library
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
            res = sdk.SetRegister("board0:/Registers/pol", polarity);
            res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold);
            res = sdk.SetRegister("board0:/Registers/Nsample", Nsample);
            res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_bs);
            res = sdk.SetRegister("board0:/Registers/time_integr", time_integr);
            res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integr);
            res = sdk.SetRegister("board0:/Registers/gain", gain);
            res = sdk.SetRegister("board0:/Registers/Offset_int", Offset_int);

            if (res != 0)
            {
                Console.WriteLine("Error while trying to write register value");
            }

            // // OSCILLOSCOPE
            // Set parameters oscilloscope
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext");
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", decimator);
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
            res = sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 3000);

            // Allocate buffer
            buffer = new SciSDKOscilloscopeDecodedBuffer();
            res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);
            if (res != 0)
            {
                Console.WriteLine("Error allocting buffer");
            }

            sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "");

            // Set up the plot models and series
            //====================================================
            model1 = new PlotModel();
            model2 = new PlotModel();
            analog0 = new FunctionSeries { Color = OxyColors.Green };
            analog1 = new FunctionSeries { Color = OxyColors.Orange };
            analog2 = new FunctionSeries { Color = OxyColors.Red };
            analog3 = new FunctionSeries { Color = OxyColors.Blue };
            

            model1.Series.Add(analog0);
            model1.Series.Add(analog1);
            model1.Series.Add(analog2);
            model1.Series.Add(analog3);

            model1.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom });
            model1.Axes.Add(new LinearAxis { Position = AxisPosition.Left , Title = "Analog"});

            //====================================================

            digital0_1 = new FunctionSeries { Color = OxyColors.Orange };
            digital0_2 = new FunctionSeries { Color = OxyColors.Red };
            digital0_3 = new FunctionSeries { Color = OxyColors.Blue };
            var linearAxis1 = new LinearAxis { Title = "Digital 0", Key = "Digital 0", AbsoluteMaximum = 1, AbsoluteMinimum = 0};
            linearAxis1.EndPosition = 0.99;
            linearAxis1.StartPosition = 0.71;
            digital0_1.YAxisKey = "Digital 0";
            digital0_2.YAxisKey = "Digital 0";
            digital0_3.YAxisKey = "Digital 0";

            model2.Axes.Add(linearAxis1);
            model2.Series.Add(digital0_1);
            model2.Series.Add(digital0_2);
            model2.Series.Add(digital0_3);

            //====================================================

            digital1_1 = new FunctionSeries { Color = OxyColors.Orange };
            digital1_3 = new FunctionSeries { Color = OxyColors.Blue };
            var linearAxis2 = new LinearAxis { Title = "Digital 1", Key = "Digital 1", AbsoluteMaximum = 1, AbsoluteMinimum = 0 };
            linearAxis2.EndPosition = 0.64;
            linearAxis2.StartPosition = 0.36;
            digital1_1.YAxisKey = "Digital 1";
            digital1_3.YAxisKey = "Digital 1";

            model2.Axes.Add(linearAxis2);
            model2.Series.Add(digital1_1);
            model2.Series.Add(digital1_3);

            //===================================================

            digital2_3 = new FunctionSeries { Color = OxyColors.Blue };
            var linearAxis3 = new LinearAxis { Title = "Digital 2", Key = "Digital 2", AbsoluteMaximum = 1, AbsoluteMinimum = 0 };
            linearAxis3.EndPosition = 0.28;
            linearAxis3.StartPosition = 0.02;
            digital2_3.YAxisKey = "Digital 2";

            model2.Axes.Add(linearAxis3);
            model2.Series.Add(digital2_3);

            //===================================================

            var linearAxis4 = new LinearAxis();
            linearAxis4.Position = AxisPosition.Bottom;
            model2.Axes.Add(linearAxis4);

            // Set up the timer
            timer = new Timer();
            timer.Elapsed += UpdateGraph;

            // Set up the plotviews
            //===================================================
            TableLayoutPanel table = new TableLayoutPanel { Dock = DockStyle.Fill };
            table.RowCount= 2;
            table.ColumnCount = 1;
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            plot_view1 = new OxyPlot.WindowsForms.PlotView { Dock = DockStyle.Fill, Model = model1};
            plot_view2 = new OxyPlot.WindowsForms.PlotView { Dock = DockStyle.Fill, Model = model2};

            table.Controls.Add(plot_view1);
            table.Controls.Add(plot_view2);

            Controls.Add(table);

            //===================================================
            // Set up the start and stop buttons
            TableLayoutPanel t1 = new TableLayoutPanel();
            t1.Dock = DockStyle.Bottom;
            t1.Size = new Size(70, 100);
            t1.RowCount = 1;
            t1.ColumnCount = 2;
            t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));
            t1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));

            // Create the start button and set its properties
            var startButton = new Button { Text = "Start", Dock = DockStyle.Fill };
            startButton.Click += StartButton_Click;

            // Create the stop button and set its properties
            var stopButton = new Button { Text = "Stop", Dock = DockStyle.Fill };
            stopButton.Click += StopButton_Click;

            // Set up the flow layout panel for the buttons
            t1.Controls.Add(startButton, 0, 0);
            t1.Controls.Add(stopButton, 1, 0);

            Controls.Add(t1);

            //===================================================
            // Set up the channel check boxes
            TableLayoutPanel t3 = new TableLayoutPanel();
            t3.Dock = DockStyle.Left;
            t3.Size = new Size(200, 30);
            t3.RowCount = 13;
            t3.ColumnCount = 2;
            t3.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));
            t3.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 10f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));
            t3.RowStyles.Add(new RowStyle(SizeType.Percent, 5f));

            // Create the channel0 buttons and set its properties
            Label channel0_name = new Label { Text = "Signal", Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleCenter };
            channel_0 = new CheckBox { Dock = DockStyle.Fill };
            channel_0.CheckedChanged += ch_0;
            t3.Controls.Add(channel0_name, 0, 0);
            t3.Controls.Add(channel_0, 1, 0);

            // Create the channel1 buttons  and set its properties
            Label channel1_name = new Label { Text = "Baseline", Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleCenter };
            channel_1 = new CheckBox { Dock = DockStyle.Fill };
            channel_1.CheckedChanged += ch_1;
            t3.Controls.Add(channel1_name, 0, 1);
            t3.Controls.Add(channel_1, 1, 1);

            // Create the channel2 buttons  and set its properties
            Label channel2_name = new Label { Text = "Trigger", Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleCenter };
            channel_2 = new CheckBox { Dock = DockStyle.Fill };
            channel_2.CheckedChanged += ch_2;
            t3.Controls.Add(channel2_name, 0, 2);
            t3.Controls.Add(channel_2, 1, 2);

            // Create the channel3 buttons and set its properties
            Label channel3_name = new Label { Text = "Integral", Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleCenter };
            channel_3 = new CheckBox { Dock = DockStyle.Fill };
            channel_3.CheckedChanged += ch_3;
            t3.Controls.Add(channel3_name, 0, 3);
            t3.Controls.Add(channel_3, 1, 3);

            //===================================================
            // Set up parameters boxes


            // Create the polarity setter
            Label pol_name = new Label { Text = "Polarity: ", Dock = DockStyle.Fill };
            pol = new ComboBox { Text = "Negative", Dock = DockStyle.Fill };
            pol.Items.AddRange(new object[] { "Positive", "Negative" });
            pol.SelectedIndexChanged += pol_change;
            t3.Controls.Add(pol_name, 0, 5);
            t3.Controls.Add(pol, 1, 5);

            // Create the trigger setter
            Label trigger_name = new Label { Text = "Trigger: ", Dock = DockStyle.Fill };
            trigger = new TextBox { Text = trigger_threshold.ToString()};
            trigger.KeyDown += new KeyEventHandler(return_trigger);
            t3.Controls.Add(trigger_name, 0, 6);
            t3.Controls.Add(trigger, 1, 6);

            // Create the N samples setter
            Label sample_name = new Label { Text = "N samples: ", Dock = DockStyle.Fill };
            N_samples = new TextBox { Text = Nsample.ToString() };
            N_samples.KeyDown += new KeyEventHandler(return_Nsample);
            t3.Controls.Add(sample_name, 0, 7);
            t3.Controls.Add(N_samples, 1, 7);

            // Create the time hold baseline
            Label bs_name = new Label { Text = "Hold baseline: ", Dock = DockStyle.Fill };
            hold_bl = new TextBox { Text = time_hold_bs.ToString() };
            hold_bl.KeyDown += new KeyEventHandler(return_hold_bl);
            t3.Controls.Add(bs_name, 0, 8);
            t3.Controls.Add(hold_bl, 1, 8);

            // Create the interval integration setter
            Label int_name = new Label { Text = "Int integration: ", Dock = DockStyle.Fill };
            int_integr = new TextBox { Text = time_integr.ToString() };
            int_integr.KeyDown += new KeyEventHandler(return_time_integr);
            t3.Controls.Add(int_name, 0, 9);
            t3.Controls.Add(int_integr, 1, 9);

            // Create the pre integration setter
            Label pre_name = new Label { Text = "Pre trigger: ", Dock = DockStyle.Fill };
            pre_tr = new TextBox { Text = pre_integr.ToString() };
            pre_tr.KeyDown += new KeyEventHandler(return_pre_integr);
            t3.Controls.Add(pre_name, 0, 10);
            t3.Controls.Add(pre_tr, 1, 10);

            // Create the gain setter
            Label gain_name = new Label { Text = "Gain: ", Dock = DockStyle.Fill };
            Gain = new TextBox { Text = gain.ToString() };
            Gain.KeyDown += new KeyEventHandler(return_gain);
            t3.Controls.Add(gain_name, 0, 11);
            t3.Controls.Add(Gain, 1, 11);

            // Create the offset setter
            Label off_name = new Label { Text = "Offset integration: ", Dock = DockStyle.Fill };
            offset = new TextBox { Text = Offset_int.ToString() };
            offset.KeyDown += new KeyEventHandler(return_offset);
            t3.Controls.Add(off_name, 0, 12);
            t3.Controls.Add(offset, 1, 12);


            Controls.Add(t3);

            timer.Start();

            if (res != 0)
            {
                Console.WriteLine("Error while trying to read register value");
            }

        }
        private void StartButton_Click(object sender, System.EventArgs e)
        {
            // Start the timer
            timer.Start();
            sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.start", "");
        }

        private void StopButton_Click(object sender, System.EventArgs e)
        {
            // Stop the timer
            timer.Stop();
            sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.stop", "");
        }

        private void ch_0(object sender, System.EventArgs e)
        {
            if (channel_0.Checked)
            {
                channel0= true;
            }
            else
            {
                channel0= false;
            }
        }

        private void ch_1(object sender, System.EventArgs e)
        {
            if (channel_1.Checked)
            {
                channel1 = true;
            }
            else
            {
                channel1 = false;
            }
        }
        private void ch_2(object sender, System.EventArgs e)
        {
            if (channel_2.Checked)
            {
                channel2 = true;
            }
            else
            {
                channel2 = false;
            }
        }
        private void ch_3(object sender, System.EventArgs e)
        {
            if (channel_3.Checked)
            {
                channel3 = true;
            }
            else
            {
                channel3 = false;
            }
        }

        private void pol_change(object sender, System.EventArgs e)
        {
            var item = pol.GetItemText(pol.SelectedItem);

            if (item == "positive")
            {
                polarity = 0;
            }
            else
            {
                polarity = 1;
            }

            res = sdk.SetRegister("board0:/Registers/pol", polarity);
        }

        private void return_trigger(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = trigger.Text;
                trigger_threshold = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold);
                trigger.Text = trigger_threshold.ToString();
            }
        }
        
        private void return_Nsample(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = N_samples.Text;
                Nsample = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/Nsample", Nsample);
                N_samples.Text = Nsample.ToString();
            }
        }

        private void return_hold_bl(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = hold_bl.Text;
                time_hold_bs = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_bs);
                hold_bl.Text = time_hold_bs.ToString();
            }
        }
        private void return_time_integr(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = int_integr.Text;
                time_integr = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/time_integr", time_integr);
                int_integr.Text = time_integr.ToString();
            }
        }
        private void return_pre_integr(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = pre_tr.Text;
                pre_integr = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integr);
                pre_tr.Text = pre_integr.ToString();
            }
        }
        private void return_gain(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = Gain.Text;
                gain = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/gain", gain);
                Gain.Text = gain.ToString();
            }
        }
        private void return_offset(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string value = offset.Text;
                Offset_int = Int32.Parse(value);
                res = sdk.SetRegister("board0:/Registers/Offset_int", Offset_int);
                offset.Text = Offset_int.ToString();
            }
        }
        
        private void UpdateGraph(object? sender, ElapsedEventArgs e)
        {
            res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", ref buffer);

            if (res == 0 )
            {
                int offsetAn = (int)buffer.info.samples_analog;
                int offsetDig = (int)buffer.info.samples_digital * (int)buffer.info.tracks_digital_per_channel;

                int[] x = new int[offsetAn];

                int[] y0 = new int[offsetAn];
                int[] y1 = new int[offsetAn];
                int[] y2 = new int[offsetAn];
                int[] y3 = new int[offsetAn];

                int[] y0_1 = new int[offsetAn];
                int[] y1_1 = new int[offsetAn];
                int[] y0_2 = new int[offsetAn];
                int[] y0_3 = new int[offsetAn];
                int[] y1_3 = new int[offsetAn];
                int[] y2_3 = new int[offsetAn];

                for (int i = 0; i < buffer.info.samples_analog; i++)
                {
                    x[i] = i * decimator;
                    y0[i] = buffer.analog[i];
                    y1[i] = buffer.analog[i + offsetAn];
                    y2[i] = buffer.analog[i + offsetAn * 2];
                    y3[i] = buffer.analog[i + offsetAn * 3];

                    y0_1[i] = buffer.digital[i + offsetDig * 1] ? 1 : 0;
                    y1_1[i] = buffer.digital[i + offsetDig * 1 + offsetAn] ? 1 : 0;

                    y0_2[i] = buffer.digital[i + offsetDig * 2] ? 1 : 0;

                    y0_3[i] = buffer.digital[i + offsetDig * 3] ? 1 : 0;
                    y1_3[i] = buffer.digital[i + offsetDig * 3 + offsetAn] ? 1 : 0;
                    y2_3[i] = buffer.digital[i + offsetDig * 3 + offsetAn * 2] ? 1 : 0;
                    
                }

                analog0.Points.Clear();
                analog1.Points.Clear();
                analog2.Points.Clear();
                analog3.Points.Clear();
                digital0_1.Points.Clear();
                digital1_1.Points.Clear();
                digital0_2.Points.Clear();
                digital0_3.Points.Clear();
                digital1_3.Points.Clear();
                digital2_3.Points.Clear();

                if (channel0 == true)
                {
                    for (int i = 0; i < x.Length; i++)
                    {
                        analog0.Points.Add(new DataPoint(x[i], y0[i]));
                    }
                }
                if (channel1 == true)
                {
                    for (int i = 0; i < x.Length; i++)
                    {
                        analog1.Points.Add(new DataPoint(x[i], y1[i]));
                        digital0_1.Points.Add(new DataPoint(x[i], y0_1[i]));
                        digital1_1.Points.Add(new DataPoint(x[i], y1_1[i]));
                    }
                }
                if (channel2 == true)
                {
                    for (int i = 0; i < x.Length; i++)
                    {
                        analog2.Points.Add(new DataPoint(x[i], y2[i]));
                        digital0_2.Points.Add(new DataPoint(x[i], y0_2[i]));
                    }
                }
                if (channel3 == true)
                {
                    for (int i = 0; i<x.Length; i++)
                    {
                        analog3.Points.Add(new DataPoint(x[i], y3[i]));
                        digital0_3.Points.Add(new DataPoint(x[i], y0_3[i]));
                        digital1_3.Points.Add(new DataPoint(x[i], y1_3[i]));
                        digital2_3.Points.Add(new DataPoint(x[i], y2_3[i]));
                    }
                }
                
                model1.InvalidatePlot(true);
                model2.InvalidatePlot(true);
            }
        }

    };
}
