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
//using System.Threading;
//using System.Threading.Tasks;
using System.Windows.Forms;

namespace Oscilloscope
{
    public partial class OscilloscopeForm : Form
    {
        OscilloscopeGraph analog_graph;
        OscilloscopeGraph[] digital_graphs;
        //GraphUpdate graph_update;

        SciSDK _sdk;// pointer to the scisdk handle
        string _board_name;// name given from user to the board
        string _json_board_path;// path to json project file
        JObject _oscilloscope_obj;// object that describes oscilloscope inside JSON

        int ndigital = 0;// number of digital traces per channel
        int nanalog = 0;// number of analog traces per channel
        int nchannels = 0;// number of channels

        string oscilloscope_base_path = "";// base path of oscilloscope

        bool[] channels_enabled;// vector where is stored if a channel is enable

        Timer update_timer;// timer used to update graph
        SciSDKOscilloscopeDecodedBuffer buffer;

        public OscilloscopeForm(SciSDK sdk, string board_name, string oscilloscope_name, string json_file_path, JObject oscilloscope_obj)
        {
            InitializeComponent();

            // set attributes
            _sdk = sdk;
            _board_name = board_name;
            _json_board_path = json_file_path;
            _oscilloscope_obj = oscilloscope_obj;

            oscilloscope_base_path = board_name + ":/MMCComponents/" + oscilloscope_name;

            // get number of digital traces from SciSDK library
            if (sdk.GetParameter(oscilloscope_base_path + ".ndigital", out ndigital) != 0)
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }
            // get number of analog traces from SciSDK library
            if (sdk.GetParameter(oscilloscope_base_path + ".nanalog", out nanalog) != 0)
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }
            // get number of channels from SciSDK library
            if (sdk.GetParameter(oscilloscope_base_path + ".nchannels", out nchannels) != 0)
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }

            btn_stop.Enabled = false;

            // add types od edge to trigger edge combobox
            cmb_trigger_polarity.Items.Add("pos");
            cmb_trigger_polarity.Items.Add("neg");
            cmb_trigger_polarity.SelectedIndex = 0;

            // display the list of available channels
            check_lst_channels.Items.Add("ALL");
            for (int i = 0; i < nchannels; i++)
            {
                check_lst_channels.Items.Add("Channel " + i.ToString());
            }

            // add sources to trigger source combobox
            for (int i = 0; i < nchannels; i++)
            {
                cmb_trigger_source.Items.Add("Channel " + i.ToString());
            }
            cmb_trigger_source.SelectedIndex = 0;

            // add mode to trigger mode combobox
            cmb_trigger_mode.Items.Add("disabled");
            cmb_trigger_mode.Items.Add("self");
            cmb_trigger_mode.Items.Add("analog");
            cmb_trigger_mode.Items.Add("ext");
            cmb_trigger_mode.Items.Add("digital");
            cmb_trigger_mode.SelectedIndex = 1;

            channels_enabled = new bool[nchannels];
            for (int i = 0; i < channels_enabled.Length; i++)
            {
                channels_enabled[i] = false;
            }
            update_timer = new Timer();
            update_timer.Interval = 100;
            update_timer.Tick += Update_timer_Tick;

            // set board parameters
            _sdk.SetParameter(oscilloscope_base_path + ".data_processing", "decode");
            _sdk.SetParameter(oscilloscope_base_path + ".acq_mode", "blocking");
            _sdk.SetParameter(oscilloscope_base_path + ".timeout", 1000);
            _sdk.SetParameter(oscilloscope_base_path + ".decimator", (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5 + 1) - 1);
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_channel", cmb_trigger_source.SelectedIndex);
            _sdk.SetParameter(oscilloscope_base_path + ".pretrigger", track_pretrigger.Value * (1024 / 100));
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_level", track_trigger_level.Value);
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_mode", cmb_trigger_mode.Text);
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_polarity", cmb_trigger_polarity.Text);
            _sdk.ExecuteCommand(oscilloscope_base_path + ".reset_read_valid_flag", "");

            // allocate buffer
            buffer = new SciSDKOscilloscopeDecodedBuffer();
            _sdk.AllocateBuffer(oscilloscope_base_path, BufferType.BUFFER_TYPE_DECODED, ref buffer);
        }

        private void Update_timer_Tick(object sender, EventArgs e)
        {
            update_timer.Stop();
            // update graph
            if (_sdk.ReadData(oscilloscope_base_path, ref buffer) == 0)
            {
                lbl_status.Text = "Waiting for trigger";
                // remove analog traces from the graph
                analog_graph.GetModel().Series.Clear();
                for (int i = 0; i < analog_series.Length; i++)
                {
                    analog_series[i].Points.Clear();
                }

                int decimator = (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5);

                // plot analog trace
                for (int j = 0; j < buffer.info.channels; j++)
                {
                    if (channels_enabled[j] == true)
                    {
                        for (int i = 0; i < buffer.analog.Length; i++)
                        {
                            analog_series[j].Points.Add(new DataPoint(i * decimator, buffer.analog[(j * buffer.info.samples_analog) + i]));
                        }
                        analog_graph.GetModel().Series.Add(analog_series[j]);
                    }
                }

                // invalidate graph (oxyplot repaints it)
                analog_graph.GetModel().InvalidatePlot(true);

                // remove series from all graphs
                for (int i = 0; i < digital_graphs.Length; i++)
                {
                    digital_graphs[i].GetModel().Series.Clear();
                }

                // plot digital values on graphs
                for (int i = 0; i < buffer.info.channels; i++)
                {
                    if (channels_enabled[i] == true)
                    {
                        for (int j = 0; j < buffer.info.tracks_digital_per_channel; j++)
                        {
                            // clear serie
                            digital_series[i, j].Points.Clear();

                            // add new values to serie
                            for (int k = 0; k < buffer.info.samples_digital; k++)
                            {
                                digital_series[i, j].Points.Add((new DataPoint(k * decimator, buffer.digital[i * buffer.info.tracks_digital_per_channel + j + i])));
                            }
                            digital_graphs[j].GetModel().Series.Add(digital_series[i, j]);
                        }
                    }
                }

                // invalidate digital graphs (oxyplot repaints them)
                for (int i = 0; i < digital_graphs.Length; i++)
                {
                    digital_graphs[i].GetModel().InvalidatePlot(true);
                }
            }
            else
            {
                lbl_status.Text = "INVALID DATA";
            }
            update_timer.Start();
        }

        FunctionSeries[] analog_series;
        FunctionSeries[,] digital_series;

        // method call when form has been loaded
        private void OscilloscopeForm_Load(object sender, EventArgs e)
        {
            int x = 250;
            int y = 65;
            const int analog_trace_graph_height = 350;
            const int digital_trace_graph_height = 90;

            // display analog traces graphs
            Point graph_position = new Point(x, y);
            Size graph_size = new Size(750, analog_trace_graph_height);
            analog_graph = new OscilloscopeGraph(graph_position, graph_size);
            analog_graph.SetYAxisLabel("Analog");
            analog_graph.AddToUI(this);
            analog_graph.SetMaxValueX(12000);
            y += analog_trace_graph_height;

            // display digital traces graphs
            digital_graphs = new OscilloscopeGraph[4];
            for (int i = 0; i < digital_graphs.Length; i++)
            {
                graph_position = new Point(x, y);
                graph_size = new Size(750, digital_trace_graph_height);
                digital_graphs[i] = new OscilloscopeGraph(graph_position, graph_size);
                if (i == 3)
                {
                    digital_graphs[i].SetXAxisLabel("Time (ns)");
                    graph_size.Height += 25;
                    digital_graphs[i].SetSize(graph_size);
                }
                digital_graphs[i].SetMinValueY(-0.1);
                digital_graphs[i].SetMaxValueY(1.1);
                digital_graphs[i].SetYAxisLabel("D" + i.ToString());
                digital_graphs[i].AddToUI(this);
                digital_graphs[i].SetMaxValueX(12000);
                y += digital_trace_graph_height;
            }

            // initialize series arrays
            analog_series = new FunctionSeries[channels_enabled.Length];
            digital_series = new FunctionSeries[channels_enabled.Length, digital_graphs.Length];
            for (int i = 0; i < analog_series.Length; i++)
            {
                analog_series[i] = new FunctionSeries();
            }
            for (int i = 0; i < channels_enabled.Length; i++)
            {
                for (int j = 0; j < digital_graphs.Length; j++)
                {
                    digital_series[i, j] = new FunctionSeries();
                }
            }
        }

        int last_horizontal_divisions = 12000;
        // event called when textbox value has benn changed
        private void txt_horizontal_divisions_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int horizontal_divisions = 0;
                if (Int32.TryParse(txt_horizontal_divisions.Text, out horizontal_divisions))
                {
                    last_horizontal_divisions = horizontal_divisions;
                }
                track_horizontal_divisions.Value = last_horizontal_divisions;
                txt_horizontal_divisions.Text = last_horizontal_divisions.ToString();
            }
            catch (Exception ex) { }
        }

        // event called when slider has been moved
        private void track_horizontal_divisions_Scroll(object sender, EventArgs e)
        {
            // set decimator level on board
            _sdk.SetParameter(oscilloscope_base_path + ".decimator", (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5 + 1) - 1);

            last_horizontal_divisions = track_horizontal_divisions.Value;
            txt_horizontal_divisions.Text = last_horizontal_divisions.ToString();
            // set graphs x values
            analog_graph.SetMaxValueX(last_horizontal_divisions);
            analog_graph.SetMinValueX(0);
            for (int i = 0; i < digital_graphs.Length; i++)
            {
                digital_graphs[i].SetMinValueX(0);
                digital_graphs[i].SetMaxValueX(last_horizontal_divisions);
            }
            analog_graph.ResetZoom();
            for (int i = 0; i < digital_graphs.Length; i++)
            {
                digital_graphs[i].ResetZoom();
            }
        }

        // event called when start button has been clicked
        private void btn_start_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = true;
            btn_start.Enabled = false;
            cmb_trigger_polarity.Enabled = false;
            cmb_trigger_mode.Enabled = false;
            cmb_trigger_source.Enabled = false;
            txt_horizontal_divisions.Enabled = false;
            txt_pretrigger.Enabled = false;
            txt_trigger_level.Enabled = false;
            track_pretrigger.Enabled = false;
            track_horizontal_divisions.Enabled = false;
            track_trigger_level.Enabled = false;
            lbl_status.Text = "Waiting for trigger";
            // start reading data
            update_timer.Start();
        }

        // event called when stop button has been clicked
        private void btn_stop_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = false;
            btn_start.Enabled = true;
            cmb_trigger_polarity.Enabled = true;
            cmb_trigger_mode.Enabled = true;
            cmb_trigger_source.Enabled = true;
            txt_horizontal_divisions.Enabled = true;
            txt_pretrigger.Enabled = true;
            txt_trigger_level.Enabled = true;
            track_pretrigger.Enabled = true;
            track_horizontal_divisions.Enabled = true;
            track_trigger_level.Enabled = true;
            lbl_status.Text = "IDLE";
            // stop reading data
            update_timer.Stop();
        }

        int last_trigger_level = 1000;
        // event called when trigger level track has been scrolled
        private void track_trigger_level_Scroll(object sender, EventArgs e)
        {
            txt_trigger_level.Text = track_trigger_level.Value.ToString();
            last_trigger_level = track_trigger_level.Value;
            // set triger level on board
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_level", track_trigger_level.Value);
        }

        // event called when txt trigger level text has been changed and user has clicked outside from the textbox
        private void txt_trigger_level_Leave(object sender, EventArgs e)
        {
            int value = 0;
            if (Int32.TryParse(txt_trigger_level.Text, out value))
            {
                if (value >= 0 && value < 65535)
                {
                    last_trigger_level = value;
                    track_trigger_level.Value = last_trigger_level;
                }
                else
                {
                    txt_trigger_level.Text = last_horizontal_divisions.ToString();
                }
            }
            else
            {
                txt_trigger_level.Text = last_horizontal_divisions.ToString();
            }
        }

        // event called when mouse left key has been realesed over check list
        private void check_lst_channels_MouseUp(object sender, MouseEventArgs e)
        {
            if (check_lst_channels.SelectedIndex == 0)
            {
                if (check_lst_channels.GetItemCheckState(check_lst_channels.SelectedIndex) == CheckState.Checked)
                {
                    for (int i = 0; i < check_lst_channels.Items.Count; i++)
                    {
                        check_lst_channels.SetItemChecked(i, true);
                    }
                }
            }

            // control which checkbox is checked and enable channels
            bool all_checked = true;
            for (int i = 1; i < check_lst_channels.Items.Count; i++)
            {
                channels_enabled[i - 1] = check_lst_channels.GetItemChecked(i);
                if (!check_lst_channels.GetItemChecked(i))
                {
                    all_checked = false;
                }
            }
            check_lst_channels.SetItemChecked(0, all_checked);

        }

        // event called when pretrigger textbox text has been moved
        private void track_pretrigger_Scroll(object sender, EventArgs e)
        {
            last_pretrigger_value = track_pretrigger.Value;
            txt_pretrigger.Text = last_pretrigger_value.ToString();
            // set pretrigger level on board
            _sdk.SetParameter(oscilloscope_base_path + ".pretrigger", track_pretrigger.Value * (1024 / 100));
           
        }

        int last_pretrigger_value = 20;
        // event called when pretrigger textbox text has been changed
        private void txt_pretrigger_MouseLeave(object sender, EventArgs e)
        {
            int value = 20;
            if (Int32.TryParse(txt_pretrigger.Text, out value))
            {
                if (value > 0 && value < 100)
                {
                    last_pretrigger_value = value;
                    track_pretrigger.Value = last_pretrigger_value;
                }
                else
                {
                    txt_pretrigger.Text = last_pretrigger_value.ToString();
                }
            }
            else
            {
                txt_pretrigger.Text = last_pretrigger_value.ToString();
            }
        }

        private void btn_reset_zoom_Click(object sender, EventArgs e)
        {
            analog_graph.ResetZoom();
            for (int i = 0; i < digital_graphs.Length; i++)
            {
                digital_graphs[i].ResetZoom();
            }
        }

        private void cmb_trigger_mode_SelectedIndexChanged(object sender, EventArgs e)
        {
            // set trigger trigger mode param on the board
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_mode", cmb_trigger_mode.Text);
        }

        private void cmb_trigger_polarity_SelectedIndexChanged(object sender, EventArgs e)
        {
            // set trigger polarity param on the board
            _sdk.SetParameter(oscilloscope_base_path + ".trigger_polarity", cmb_trigger_polarity.Text);
        }
    }


}

