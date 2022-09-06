using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Oscilloscope
{
    public partial class LoadForm : Form
    {
        IntPtr scisdk_handle;
        public LoadForm()
        {
            InitializeComponent();

            // add items to board models combobox
            cmb_model.Items.Add("V2495");
            cmb_model.Items.Add("DT5550");
            cmb_model.Items.Add("DT5550W - PETIROC");
            cmb_model.Items.Add("DT5550W - CITIROC");
            cmb_model.Items.Add("R5560");
            cmb_model.Items.Add("R5560SE");
            cmb_model.Items.Add("DT5560");
            cmb_model.Items.Add("SCIDK");
            cmb_model.Items.Add("V/DT274X");
            cmb_model.SelectedIndex = 0;

            // initialize library handle
            scisdk_handle = SciSDK.SCISDK_InitLib();
        }

        // event called when the combobox used to select board model has been changed
        private void cmb_model_SelectedIndexChanged(object sender, EventArgs e)
        {
            // display connection type options for the selected board
            if (cmb_model.Text == "V2495")
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("USB");
                cmb_connection_type.Items.Add("Ethernet");
                cmb_connection_type.Items.Add("VME");
            }
            else if (cmb_model.Text.Contains("DT5550"))
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("USB");
            }
            else if (cmb_model.Text == "R5560" || cmb_model.Text == "R5560SE")
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("Ethernet");
            }
            else if (cmb_model.Text == "DT5560")
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("Ethernet");
            }
            else if (cmb_model.Text == "SCIDK")
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("USB");
            }
            else if (cmb_model.Text == "V/DT274X")
            {
                cmb_connection_type.Items.Clear();
                cmb_connection_type.Items.Add("Ethernet");
            }
            cmb_connection_type.SelectedIndex = 0;
        }

        // event called when browse button has been clicked
        private void btn_browse_Click(object sender, EventArgs e)
        {
            // ask to user to select the JSON file
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "JSON files (*.json) |*.json|All files (*.*) |*.*";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                txt_json_file.Text = ofd.FileName;
            }
        }

        // event called when connect button has been clicked
        private void btn_connect_Click(object sender, EventArgs e)
        {
            JObject json_obj = null;
            JObject oscilloscope_obj = null;
            string oscilloscope_name = "";
            // control if json file path is valid
            if (!File.Exists(txt_json_file.Text))
            {
                MessageBox.Show("Entered json file path doesn't exists, check if file exists", "Error");
                return;
            }
            else
            {
                // read string from file
                string file_content = File.ReadAllText(txt_json_file.Text);

                try
                {
                    // try to parse json file
                    json_obj = JObject.Parse(file_content);

                    // check if json object contains the declaration of an oscilliscope
                    JArray mmc_components_obj = (JArray)json_obj["MMCComponents"];

                    bool found = false;
                    for(int i = 0; i < mmc_components_obj.Count; i++)
                    {
                        string component_type = (string)mmc_components_obj[i]["Type"];
                        oscilloscope_name = (string)mmc_components_obj[i]["Name"];
                        if (component_type == "Oscilloscope")
                        {
                            oscilloscope_obj = (JObject)mmc_components_obj[i];
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        MessageBox.Show("Loaded configuration from json file does not contain an oscilloscope", "Error");
                        return;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Selected json file does not contain a valid json string", "Error");
                    return;
                }
            }

            if (cmb_connection_type.Text == "USB")
            {
                // if connection type is USB: check if serial number is valid (only integer numbers)
                string board_sn = txt_sn.Text;
                int serial_number = 0;
                if (int.TryParse(board_sn, out serial_number))
                {
                    // generate board path
                    string board_path = "usb:" + board_sn;
                    string board_name = "board0";

                    // save all AddNewDevice function's parameters into IntPtr variables
                    IntPtr board_path_ptr = Marshal.StringToHGlobalAnsi(board_path);
                    IntPtr board_model_ptr = Marshal.StringToHGlobalAnsi(cmb_model.Text);
                    IntPtr json_file_path_ptr = Marshal.StringToHGlobalAnsi(txt_json_file.Text);
                    IntPtr board_name_ptr = Marshal.StringToHGlobalAnsi(board_name);//name of board

                    // open connection with board
                    int res = SciSDK.SCISDK_AddNewDevice(board_path_ptr, board_model_ptr, json_file_path_ptr, board_name_ptr, scisdk_handle);
                    if (res == 0)
                    {
                        // if connection has been successfully opened open oscilloscope form
                        OscilloscopeForm oscilloscope_form = new OscilloscopeForm(scisdk_handle, board_name, oscilloscope_name, txt_json_file.Text, oscilloscope_obj);
                        this.Hide();
                        oscilloscope_form.ShowDialog();
                        this.Close();
                    }
                    else
                    {
                        // if an error occurs while trying to open connection show in a messagebox the error
                        IntPtr res_string_ptr = new IntPtr(0);
                        SciSDK.SCISDK_s_error(res, res_string_ptr, scisdk_handle);
                        string error_description = Marshal.PtrToStringAnsi(res_string_ptr);
                        MessageBox.Show("Cannot open communication with selected board, check serial number and connection.\n" + res.ToString() + " - " + error_description, "Error");
                    }
                }
                else
                {
                    MessageBox.Show("Entered serial number is not valid", "Error");
                }
            }
            else
            {
                // if connection type is Ethernet or VME: check if entered ip address is valid
                string board_ip = txt_ip.Text;
                IPAddress ip_addr;
                int port = 0;
                if (board_ip.IndexOf(':') != -1)
                {
                    if (int.TryParse(board_ip.Split(':')[1], out port))
                    {
                        if (IPAddress.TryParse(board_ip.Split(':')[0], out ip_addr))
                        {
                            // generate board path
                            string board_path = board_ip;
                            return;
                        }
                    }
                }
                MessageBox.Show("Entered ip address is not valid", "Error");

            }
        }

        // event called when the combobox used to select connection type has been changed
        private void cmb_connection_type_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmb_connection_type.Text == "USB")
            {
                // if connection type is USB disable ip address text box
                txt_ip.Enabled = false;
                txt_sn.Enabled = true;
            }
            else
            {
                // if connection type is Ethernet or VME disable serial number text box
                txt_ip.Enabled = true;
                txt_sn.Enabled = false;
            }
        }
    }
}
