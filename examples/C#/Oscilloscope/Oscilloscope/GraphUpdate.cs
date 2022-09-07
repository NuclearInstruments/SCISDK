using OxyPlot;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;

namespace Oscilloscope
{
    class GraphUpdate
    {
        IntPtr _scisdk_handle;
        OscilloscopeGraph _analog_graph;
        OscilloscopeGraph[] _digital_graph;
        Thread t;
        string _oscilloscope_base_path;
        IntPtr buffer_ptr;

        // class that read data from sdk and plot graph
        public GraphUpdate(IntPtr scisdk_handle, OscilloscopeGraph analog_graph, OscilloscopeGraph[] digital_graph, string oscilloscope_base_path)
        {
            _scisdk_handle = scisdk_handle;
            _analog_graph = analog_graph;
            _digital_graph = digital_graph;
            _oscilloscope_base_path = oscilloscope_base_path;
        }

        // method called for start to read data from sdk
        public void StartRead(Thread parent_thread, Button stop_btn, Label lbl_status, TextBox txt_horizontal_divisions, TextBox txt_pre)
        {
            if (t == null)
            {
                t = new Thread(() => ThreadMethod(parent_thread, stop_btn, lbl_status, txt_horizontal_divisions, txt_pre));
            }
            t.Start();
        }

        // method called for stop to read data from sdk
        public void StopRead()
        {
            t.Abort();
            t = null;
        }

        // method used to set trigger level
        public void SetTriggerLevel(int value)
        {
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_level", value, _scisdk_handle);
        }

        // method used to set pretrigger
        public void SetPretrigger(int value)
        {
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".pretrigger", value, _scisdk_handle);
        }

        // method used to set decimator register value
        public void SetDecimator(int decimator)
        {
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".decimator", decimator, _scisdk_handle);
        }

        //method called by thread
        private void ThreadMethod(Thread parent_thread, Button stop_btn, Label lbl_status, TextBox txt_horizontal_divisions, TextBox txt_pre)
        {
            // set sdk parameters
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".decimator", (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5 + 1) - 1, _scisdk_handle);
            SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".trigger_mode", "self", _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_level", 1000, _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".pretrigger", 20 * (1024 / 100), _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_channel", 0, _scisdk_handle);
            SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".data_processing", "decode", _scisdk_handle);
            SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".acq_mode", "blocking", _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".timeout", 1000, _scisdk_handle);

            // create oscilloscope buffer
            buffer_ptr = Marshal.StringToHGlobalAnsi("t");
            Oscilloscope_decoded_buffer_struct buffer_struct;

            //// allocate buffer
            if (SciSdk_Wrapper.AllocateBuffer(_oscilloscope_base_path, SciSdk_Wrapper.T_BUFFER_DECODED, ref buffer_ptr, _scisdk_handle))
            {
                SciSdk_Wrapper.ExecuteCommand(_oscilloscope_base_path + ".reset_read_valid_flag", "", _scisdk_handle);

                const int nchannels = 1;

                FunctionSeries[] analog_series = new FunctionSeries[nchannels];
                FunctionSeries[,] digital_series = new FunctionSeries[_digital_graph.Length, nchannels];

                // initialize series arrays
                for (int i = 0; i < analog_series.Length; i++)
                {
                    analog_series[i] = new FunctionSeries();
                }

                for (int i = 0; i < _digital_graph.Length; i++)
                {
                    for (int j = 0; j < nchannels; j++)
                    {
                        digital_series[i, j] = new FunctionSeries();
                    }
                }

                // change status label text
                lbl_status.Invoke((MethodInvoker)delegate { lbl_status.Text = "Waiting for trigger"; });

                while (parent_thread.IsAlive)
                {
                    try
                    {
                        if (SciSdk_Wrapper.ReadData(_oscilloscope_base_path, ref buffer_ptr, _scisdk_handle))
                        {
                            // convert buffer pointer to buffer struct
                            buffer_struct = new Oscilloscope_decoded_buffer_struct();
                            buffer_struct = (Oscilloscope_decoded_buffer_struct)Marshal.PtrToStructure(buffer_ptr, typeof(Oscilloscope_decoded_buffer_struct));

                            // read memory and store analog values inside an integer type array
                            int[] analog_values = new int[buffer_struct.info.samples_analog];
                            Marshal.Copy(buffer_struct.analog, analog_values, 0, analog_values.Length);

                            // remove analog traces from the graph
                            _analog_graph.GetModel().Series.Clear();
                            for (int i = 0; i < analog_series.Length; i++)
                            {
                                analog_series[i].Points.Clear();
                            }

                            int decimator = (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5);

                            // plot analog trace
                            for (int i = 0; i < analog_values.Length; i++)
                            {
                                analog_series[0].Points.Add(new DataPoint(i * decimator, analog_values[i]));
                            }
                            for (int i = 0; i < analog_series.Length; i++)
                            {
                                _analog_graph.GetModel().Series.Add(analog_series[i]);
                            }
                            
                            // read memory and store digital values inside an integer type array
                            int[] digital_values = new int[buffer_struct.info.samples_digital * buffer_struct.info.channels * 4];
                            Marshal.Copy(buffer_struct.digital, digital_values, 0, digital_values.Length);


                            string tmp = "";
                            for (int i = 0; i < digital_values.Length; i++)
                            {
                                tmp += digital_values[i] + " \n";
                            }
                            File.WriteAllText("C:\\tmp\\tmp4.txt", tmp);

                            // plot digital traces
                            for(int i = 0; i < buffer_struct.info.channels; i++)
                            {
                                int[] channel_digital_value = new int[buffer_struct.info.samples_digital];
                                for(int j = 0; j < channel_digital_value.Length; j++)
                                {

                                }
                            }


                            // invalidate graph (oxyplot repaints it)
                            _analog_graph.GetModel().InvalidatePlot(true);
                        }
                        else
                        {
                            lbl_status.Invoke((MethodInvoker)delegate { lbl_status.Text = "INVALID DATA"; });
                            stop_btn.PerformClick();
                        }
                        Thread.Sleep(50);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e.ToString());
                    }
                }
            }
            stop_btn.PerformClick();
            // free buffer
            SciSdk_Wrapper.FreeBuffer(_oscilloscope_base_path, SciSdk_Wrapper.T_BUFFER_DECODED, ref buffer_ptr, _scisdk_handle);
        }


    }
}