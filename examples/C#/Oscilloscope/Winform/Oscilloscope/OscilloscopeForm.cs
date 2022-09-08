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
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Oscilloscope
{
    public partial class OscilloscopeForm : Form
    {
        OscilloscopeGraph analog_graph;
        OscilloscopeGraph[] digital_graphs;
        GraphUpdate graph_update;

        IntPtr _scisdk_handle;// pointer to the scisdk handle
        string _board_name;// name given from user to the board
        string _json_board_path;// path to json project file
        JObject _oscilloscope_obj;// object that describes oscilloscope inside JSON

        int ndigital = 0;// number of digital traces per channel
        int nanalog = 0;// number of analog traces per channel
        int nchannels = 0;// number of channels

        string oscilloscope_base_path = "";// base path of oscilloscope

        bool[] channels_enabled;// vector where is stored if a channel is enable

        public OscilloscopeForm(IntPtr scisdk_handle, string board_name, string oscilloscope_name, string json_file_path, JObject oscilloscope_obj)
        {
            InitializeComponent();

            // set attributes
            _scisdk_handle = scisdk_handle;
            _board_name = board_name;
            _json_board_path = json_file_path;
            _oscilloscope_obj = oscilloscope_obj;

            oscilloscope_base_path = board_name + ":/MMCComponents/" + oscilloscope_name;

            // get number of digital traces from SciSDK library
            if (!SciSdk_Wrapper.GetParamInt(oscilloscope_base_path + ".ndigital", out ndigital, _scisdk_handle))
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }
            // get number of analog traces from SciSDK library
            if (!SciSdk_Wrapper.GetParamInt(oscilloscope_base_path + ".nanalog", out nanalog, _scisdk_handle))
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }
            // get number of channels from SciSDK library
            if (!SciSdk_Wrapper.GetParamInt(oscilloscope_base_path + ".nchannels", out nchannels, _scisdk_handle))
            {
                MessageBox.Show("Error while trying to get number of digital traces from scisdk", "Error");
                this.Close();
            }

            btn_stop.Enabled = false;

            // add types od edge to trigger edge combobox
            cmb_trigger_edge.Items.Add("Rising");
            cmb_trigger_edge.Items.Add("Falling");
            cmb_trigger_edge.SelectedIndex = 0;

            // display the list of available channels
            check_lst_channels.Items.Add("ALL");
            for (int i = 0; i < nchannels; i++)
            {
                check_lst_channels.Items.Add("Channel " + i.ToString());
            }

            // add sources to trigger source combobox
            cmb_trigger_source.Items.Add("External");
            cmb_trigger_source.Items.Add("Free running");
            for (int i = 0; i < nchannels; i++)
            {
                cmb_trigger_source.Items.Add("Channel " + i.ToString());
            }
            cmb_trigger_source.SelectedIndex = 0;

            // add mode to trigger mode combobox
            cmb_trigger_mode.Items.Add("Auto");
            cmb_trigger_mode.Items.Add("Single");
            cmb_trigger_mode.SelectedIndex = 0;

            channels_enabled = new bool[nchannels];
            for (int i = 0; i < channels_enabled.Length; i++)
            {
                channels_enabled[i] = false;
            }
        }

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
            graph_update = new GraphUpdate(_scisdk_handle, analog_graph, digital_graphs, oscilloscope_base_path);
        }

        bool autoscale = true;
        // method called when checkbox autoscale has been checked or unchecked
        private void check_autoscale_CheckedChanged(object sender, EventArgs e)
        {
            autoscale = !autoscale;
            if (autoscale)
            {
                analog_graph.EnableAutoscaleY();
            }
            else
            {
                analog_graph.SetMinValueY(0);
                analog_graph.SetMaxValueY(100000);
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
            // start reading data
            graph_update.StartRead(Thread.CurrentThread, btn_stop, lbl_status, txt_horizontal_divisions, track_pretrigger, track_trigger_level, channels_enabled, cmb_trigger_source, cmb_trigger_edge, cmb_trigger_mode);
        }

        // event called when stop button has been clicked
        private void btn_stop_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = false;
            btn_start.Enabled = true;
            graph_update.StopRead();
            lbl_status.Text = "IDLE";
        }

        int last_trigger_level = 1000;
        // event called when trigger level track has been scrolled
        private void track_trigger_level_Scroll(object sender, EventArgs e)
        {
            txt_trigger_level.Text = track_trigger_level.Value.ToString();
            last_trigger_level = track_trigger_level.Value;


        }

        // event called when txt trigger level text has been changed and user has clicked outside from the textbox
        private void txt_trigger_level_Leave(object sender, EventArgs e)
        {
            int value = 0;
            if (Int32.TryParse(txt_trigger_level.Text, out value))
            {
                if(value >= 0 && value < 65535)
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
        }

        int last_pretrigger_value = 20;
        // event called when pretrigger textbox text has been changed
        private void txt_pretrigger_MouseLeave(object sender, EventArgs e)
        {
            int value = 20;
            if (Int32.TryParse(txt_pretrigger.Text, out value))
            {
                if(value > 0 && value < 100)
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
            for(int i = 0; i < digital_graphs.Length; i++)
            {
                digital_graphs[i].ResetZoom();
            }
        }
    }


}

