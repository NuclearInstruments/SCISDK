using CSharp_SciSDK;
using Newtonsoft.Json;
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
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
//using System.Threading;
//using System.Threading.Tasks;
using System.Windows.Forms;

namespace Digitizer
{
    public partial class DigitizerForm : Form
    {
        DigitizerGraph graph;
        SciSDK _sdk;                        // pointer to the scisdk handle
        string _board_name;                 // name given from user to the board
        string _json_board_path;            // path to json project file
        string _digitizer_name;             // name of digitizer component
        JObject _digitizer_obj;             // object that describes oscilloscope inside JSON
        Timer update_timer;                 // timer used to update graph
        string digitizer_full_path;         // full path of digitizer component
        string filename;                    // name of output string filename
        string[] file_content;       //
        // series colors
        OxyColor[] series_colors = { OxyColor.FromRgb(51, 102, 255), OxyColor.FromRgb(0, 153, 51), OxyColor.FromRgb(255, 0, 0),
                                    OxyColor.FromRgb(204, 51, 153), OxyColor.FromRgb(204, 153, 0), OxyColor.FromRgb(102, 153, 153)};

        FunctionSeries[] series;            // series displayed on graph
        int n_channels = 0;                 // number of available channels
        List<int> enabled_channels_options; // list of option of channels that can be enabled
        int wave_counter = 0;
        int n_samples;

        public DigitizerForm(SciSDK sdk, string board_name, string digitizer_name, string json_file_path, JObject digitizer_obj)
        {
            InitializeComponent();
            // set attributes
            _sdk = sdk;
            _board_name = board_name;
            _json_board_path = json_file_path;
            _digitizer_obj = digitizer_obj;
            _digitizer_name = digitizer_name;

            // load the number of available channels
            n_channels = (int)digitizer_obj["Channels"];
            
            // load number of nsamples
            n_samples = (int)digitizer_obj["nsamples"];

            // get available channels options and add children to enabled channels combobox
            string ret = "";
            digitizer_full_path = board_name + ":/MMCComponents/" + digitizer_name;
            int res = sdk.GetParameterListOfValues(digitizer_full_path + ".enabledch", out ret);
            enabled_channels_options = JsonConvert.DeserializeObject<List<int>>(ret);

            for (int i = 0; i < enabled_channels_options.Count - 1; i++)
            {
                int option_count = (int)enabled_channels_options[i];
                string option_label = "";
                for (int j = 0; j < option_count; j++)
                {
                    option_label += (j + 1).ToString();
                    if (j < option_count - 1)
                    {
                        option_label += "-";
                    }
                }
                cmb_enabled_ch.Items.Add(option_label);
            }
            // select last available item of enabled channels combobox
            cmb_enabled_ch.SelectedIndex = cmb_enabled_ch.Items.Count - 1;

         

            // initialize timer
            update_timer = new Timer();
            update_timer.Tick += Update_timer_Tick;
            update_timer.Interval = 100;

            filename = txt_filename.Text;
        }

        // method call when form has been loaded
        private void OscilloscopeForm_Load(object sender, EventArgs e)
        {
            int x = 250;
            int y = 65;
            // display analog traces graphs
            Point graph_position = new Point(x, y);
            Size graph_size = new Size(750, 500);
            graph = new DigitizerGraph(graph_position, graph_size);
            graph.SetYAxisLabel("Analog");
            graph.SetXAxisLabel("Time (ns)");
            graph.AddToUI(this);
        }

        private void Update_timer_Tick(object sender, EventArgs e)
        {
            update_timer.Stop();
            if (wave_counter < num_number_of_waves.Value)
            {
                // allocate buffer
                SciSDKDigitizerDecodedBuffer buffer = new SciSDKDigitizerDecodedBuffer();
                int res = _sdk.AllocateBuffer(digitizer_full_path, BufferType.BUFFER_TYPE_DECODED, ref buffer);
                if (res == 0)
                {
                    // read data 
                    res = _sdk.ReadData(digitizer_full_path, ref buffer);
                    if (res == 0)
                    {
                        for (int i = 0; i < check_displayed_channels.Items.Count; i++)
                        {
                            series[i].Points.Clear();
                        }
                        graph.GetModel().Series.Clear();
                        // plot graph
                        for (int i = 0; i < check_displayed_channels.Items.Count; i++)
                        {
                            file_content[i] = "";
                            if (check_displayed_channels.GetItemChecked(i))
                            {
                                for (int j = 0; j < buffer.info.valid_samples; j++)
                                {
                                    series[i].Points.Add(new DataPoint(j, buffer.analog[j + i * buffer.info.valid_samples]));
                                    file_content[i] += buffer.analog[j + i * buffer.info.valid_samples].ToString() + "\n";
                                }
                                graph.GetModel().Series.Add(series[i]);
                            }
                        }
                        // invalidate graph (oxyplot repaints it)
                        graph.GetModel().InvalidatePlot(true);
                    }
                    else
                    {
                        Console.WriteLine("read data error");
                    }
                }
                else
                {
                    Console.WriteLine("allocate buffer error");
                }
                wave_counter++;
                if (wave_counter < num_number_of_waves.Value)
                {
                    update_timer.Start();
                }
                else
                {
                    // stop
                    btn_stop.Enabled = false;
                    btn_start.Enabled = true;
                    lbl_status.Text = "IDLE";
                    _sdk.ExecuteCommand(digitizer_full_path + ".stop", "");

                    if (check_store_on_file.Checked == true)
                    {
                        // store on file
                        string file_content_str = "";
                        for (int i = 0; i < file_content.Length; i++)
                        {
                            if (file_content[i] != "")
                                file_content_str += "CHANNEL " + (i + 1) + ":\n" + file_content[i];
                        }
                        File.WriteAllText(filename, file_content_str);
                    }
                }
            }
            else
            {
                // stop
                btn_stop.Enabled = false;
                btn_start.Enabled = true;
                lbl_status.Text = "IDLE";
                _sdk.ExecuteCommand(digitizer_full_path + ".stop", "");
                if (check_store_on_file.Checked == true)
                {
                    // store on file
                    string file_content_str = "";
                    for (int i = 0; i < file_content.Length; i++)
                    {
                        if (file_content[i] != "")
                            file_content_str += "CHANNEL " + (i + 1) + ":\n" + file_content[i];
                    }
                    File.WriteAllText(filename, file_content_str);
                }
            }
        }

        // event called when start button has been clicked
        private void btn_start_Click(object sender, EventArgs e)
        {
            file_content = new string[n_channels];
            btn_stop.Enabled = true;
            btn_start.Enabled = false;
            lbl_status.Text = "Waiting for trigger";

            // set board parameters
            _sdk.SetParameter(digitizer_full_path + ".acq_mode", "blocking");
            _sdk.SetParameter(digitizer_full_path + ".data_processing", "decode");
            _sdk.SetParameter(digitizer_full_path + ".timeout", 1000);
       
            // change value of acq lenght on board
            _sdk.SetParameter(digitizer_full_path + ".acq_len", track_wave_length.Value);
            _sdk.SetParameter(digitizer_full_path + ".enabledch", cmb_enabled_ch.SelectedIndex + 1);

            Console.WriteLine(_sdk.ExecuteCommand(digitizer_full_path + ".start", ""));
            wave_counter = 0;
            // start reading data
            update_timer.Start();
        }

        // event called when stop button has been clicked
        private void btn_stop_Click(object sender, EventArgs e)
        {
            btn_stop.Enabled = false;
            btn_start.Enabled = true;
            lbl_status.Text = "IDLE";
            _sdk.ExecuteCommand(digitizer_full_path + ".stop", "");
            // stop reading data
            update_timer.Stop();
            if (check_store_on_file.Checked == true)
            {
                // store on file
                string file_content_str = "";
                for (int i = 0; i < file_content.Length; i++)
                {
                    if (file_content[i] != "")
                        file_content_str += "CHANNEL " + (i + 1) + ":\n" + file_content[i];
                }
                File.WriteAllText(filename, file_content_str);
            }
        }


        private void btn_reset_zoom_Click(object sender, EventArgs e)
        {
            graph.ResetZoom();
        }

        private void btn_all_Click(object sender, EventArgs e)
        {
            // display all channels
            for (int i = 0; i < check_displayed_channels.Items.Count; i++)
            {
                check_displayed_channels.SetItemChecked(i, true);
            }
        }

        private void btn_none_Click(object sender, EventArgs e)
        {
            // hide all channels
            for (int i = 0; i < check_displayed_channels.Items.Count; i++)
            {
                check_displayed_channels.SetItemChecked(i, false);
            }
        }

        private void track_wave_length_Scroll(object sender, EventArgs e)
        {
            double nch = Math.Pow(2, cmb_enabled_ch.SelectedIndex);
            num_wave_length.Value = Convert.ToDecimal(Math.Floor(track_wave_length.Value * (nch / n_channels)) * (n_channels / nch));
        }

        private void num_wave_length_ValueChanged(object sender, EventArgs e)
        {
            track_wave_length.Value = (int)num_wave_length.Value;
        }

        // event called when combobox enabled channel selected index has been changed
        private void cmb_enabled_ch_SelectedIndexChanged(object sender, EventArgs e)
        {
            // set enabled channels parameter on the board
            int exp = (cmb_enabled_ch.SelectedIndex + 1);
         
            // update the list of channels that can be displayed
            check_displayed_channels.Items.Clear();
            for (int i = 0; i < enabled_channels_options[cmb_enabled_ch.SelectedIndex]; i++)
            {
                check_displayed_channels.Items.Add("Channel" + i);
            }

            // initialize series arrays
            series = new FunctionSeries[check_displayed_channels.Items.Count];
            for (int i = 0; i < check_displayed_channels.Items.Count; i++)
            {
                series[i] = new FunctionSeries();
            }

            // change maximum value of wave length track
            double nch = Math.Pow(2, cmb_enabled_ch.SelectedIndex);

            track_wave_length.Maximum = Convert.ToInt32((n_samples / (nch / n_channels)) - 64);
            num_wave_length.Maximum = track_wave_length.Maximum;
        }

        private void btn_select_file_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "*.txt|(*.txt)";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                filename = sfd.FileName;
                txt_filename.Text = filename;
            }
        }

        private void num_number_of_waves_ValueChanged(object sender, EventArgs e)
        {

        }
    }
}