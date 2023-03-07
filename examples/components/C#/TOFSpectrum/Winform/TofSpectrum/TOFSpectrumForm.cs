using CSharp_SciSDK;
using Newtonsoft.Json.Linq;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Oscilloscope
{
    public partial class TOFSpectrumForm : Form
    {
        TofSpectrumGraph graph;

        SciSDK _sdk;// pointer to the scisdk handle
        string _board_name;// name given from user to the board
        string _json_board_path;// path to json project file
        JObject _TOF_spectrum_obj;// object that describes oscilloscope inside JSON

        string TOF_spectrum_base_path = "";// base path of oscilloscope

        Timer update_timer;// timer used to update graph
        SciSDKSpectrumDecodedBuffer buffer;

        FunctionSeries series;

        // series colors
        OxyColor[] series_colors = { OxyColor.FromRgb(51, 102, 255), OxyColor.FromRgb(0, 153, 51), OxyColor.FromRgb(255, 0, 0),
                                    OxyColor.FromRgb(204, 51, 153), OxyColor.FromRgb(204, 153, 0), OxyColor.FromRgb(102, 153, 153)};

        public TOFSpectrumForm(SciSDK sdk, string board_name, string TOF_spectrum_name, string json_file_path, JObject TOF_spectrum_obj)
        {
            InitializeComponent();

            // set attributes
            _sdk = sdk;
            _board_name = board_name;
            _json_board_path = json_file_path;
            _TOF_spectrum_obj = TOF_spectrum_obj;
            TOF_spectrum_base_path = board_name + ":/MMCComponents/" + TOF_spectrum_name;

            // initialize and start timer
            update_timer = new Timer();
            update_timer.Interval = 100;
            update_timer.Tick += Update_timer_Tick;

            // set board parameters
            _sdk.SetParameter(TOF_spectrum_base_path, "0");
            _sdk.SetParameter(TOF_spectrum_base_path, "freerun");
            _sdk.SetParameter(TOF_spectrum_base_path, "100");
            // execute reset command
            _sdk.ExecuteCommand(TOF_spectrum_base_path + ".reset", "");
            // execute start command
            _sdk.ExecuteCommand(TOF_spectrum_base_path + ".start", "");
            buffer = new SciSDKSpectrumDecodedBuffer();

            // add update time choices to cmb refresh time
            cmb_refresh_time.Items.Add("100 ms");
            cmb_refresh_time.Items.Add("500 ms");
            cmb_refresh_time.Items.Add("1 s");
            cmb_refresh_time.Items.Add("5 s");
            cmb_refresh_time.Items.Add("10 s");
            cmb_refresh_time.SelectedIndex = 0;
        }

        private void Update_timer_Tick(object sender, EventArgs e)
        {
            update_timer.Stop();
            // allocate buffer
            int res = _sdk.AllocateBuffer(TOF_spectrum_base_path, BufferType.BUFFER_TYPE_DECODED, ref buffer);
            if (res == 0)
            {
                // read data
                res = _sdk.ReadData(TOF_spectrum_base_path, ref buffer);
                if (res == 0)
                {
                    // clear graph
                    series.Points.Clear();
                    graph.GetModel().Series.Clear();
                    for (int i = 0; i < buffer.info.valid_bins - 2; i++)
                    {
                        series.Points.Add(new DataPoint(i, buffer.data[i]));
                    }
                    graph.GetModel().Series.Add(series);
                }
                else
                {
                    // print error message
                    Console.WriteLine("Read data error");
                    string str_tmp = "";
                    _sdk.s_error(res, out str_tmp);
                    Console.WriteLine(str_tmp);
                }

                // free buffer
                _sdk.FreeBuffer(TOF_spectrum_base_path, BufferType.BUFFER_TYPE_DECODED, ref buffer);
            }
            else
            {
                // print error message
                Console.WriteLine("Allocate buffer error");
                string str_tmp = "";
                _sdk.s_error(res, out str_tmp);
                Console.WriteLine(str_tmp);
            }
            update_timer.Start();
        }


        // method call when form has been loaded
        private void OscilloscopeForm_Load(object sender, EventArgs e)
        {
            // display graph
            Point graph_position = new Point(7, 65);
            Size graph_size = new Size(1000, 500);
            graph = new TofSpectrumGraph(graph_position, graph_size);
            graph.SetYAxisLabel("Counts");
            graph.SetXAxisLabel("Channels");
            graph.AddToUI(this);
            graph.SetMaxValueX(1024);
            //graph.SetMinValueY(-1);
            //graph.SetMaxValueY(1);

            // initialize series array
            series = new FunctionSeries
            {
                LineStyle = LineStyle.Solid
            };
        }


        // event called when start button has been clicked
        private void btn_start_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = true;
            btn_start.Enabled = false;
            // start reading data
            update_timer.Start();
        }

        // event called when stop button has been clicked
        private void btn_stop_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = false;
            btn_start.Enabled = true;
            // stop reading data
            update_timer.Stop();
        }

        private void btn_reset_zoom_Click(object sender, EventArgs e)
        {
            graph.ResetZoom();
        }

        private void cmb_refresh_time_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cmb_refresh_time.SelectedIndex)
            {
                case 0:
                    update_timer.Interval = 100;
                    break;

                case 1:
                    update_timer.Interval = 500;
                    break;

                case 2:
                    update_timer.Interval = 1000;
                    break;

                case 3:
                    update_timer.Interval = 5000;
                    break;

                case 4:
                    update_timer.Interval = 10000;
                    break;
            }
        }
    }


}

