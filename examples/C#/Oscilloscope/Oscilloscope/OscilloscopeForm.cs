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

            cmb_data_type.SelectedIndex = 0;
            cmb_data_type.DropDownStyle = ComboBoxStyle.DropDownList;
            btn_stop.Enabled = false;

            // display the list of available channels
            check_lst_channels.Items.Add("ALL");
            for(int i = 0; i < nchannels; i++)
            {
                check_lst_channels.Items.Add("Channel " + i.ToString());
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
            y += analog_trace_graph_height;

            // display digital traces graphs
            digital_graphs = new OscilloscopeGraph[5];
            for (int i = 0; i < 5; i++)
            {
                graph_position = new Point(x, y);
                graph_size = new Size(750, digital_trace_graph_height);
                digital_graphs[i] = new OscilloscopeGraph(graph_position, graph_size);
                if (i == 4)
                {
                    digital_graphs[i].SetXAxisLabel("Time (ns)");
                    graph_size.Height += 25;
                    digital_graphs[i].SetSize(graph_size);
                }
                digital_graphs[i].SetMinValueY(0);
                digital_graphs[i].SetMaxValueY(1);
                digital_graphs[i].SetYAxisLabel("D" + i.ToString());
                digital_graphs[i].AddToUI(this);
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
            }catch(Exception ex) { }
        }

        // event called when slider has been moved
        private void track_horizontal_divisions_Scroll(object sender, EventArgs e)
        {
            last_horizontal_divisions = track_horizontal_divisions.Value;
            txt_horizontal_divisions.Text = last_horizontal_divisions.ToString();
            // set graphs x values
            analog_graph.SetMaxValueX(last_horizontal_divisions);

            for(int i = 0; i < 4; i++)
            {
                digital_graphs[i].SetMaxValueX(last_horizontal_divisions);
            }
        }

        private void btn_start_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = true;
            btn_start.Enabled = false;
            graph_update.StartRead();
        }

        private void btn_stop_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = false;
            btn_start.Enabled = true;
            graph_update.StopRead();
        }
    }


}

