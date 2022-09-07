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
        public void StartRead(Thread parent_thread, Button stop_btn, Label lbl_status, TextBox txt_horizontal_divisions, TrackBar track_pre, TrackBar track_trigger_level, bool[] channels_enabled,
            ComboBox cmb_trigger_source, ComboBox cmb_trigger_edge, ComboBox cmb_trigger_mode)
        {
            if (t == null)
            {
                t = new Thread(() => ThreadMethod(parent_thread, stop_btn, lbl_status, txt_horizontal_divisions, track_pre, track_trigger_level, channels_enabled, cmb_trigger_source, cmb_trigger_edge, cmb_trigger_mode));
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
        private void ThreadMethod(Thread parent_thread, Button stop_btn, Label lbl_status, TextBox txt_horizontal_divisions, TrackBar track_pre, TrackBar track_trigger_level, bool[] channels_enabled,
            ComboBox cmb_trigger_source, ComboBox cmb_trigger_edge, ComboBox cmb_trigger_mode)
        {
            // set sdk parameters
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

                FunctionSeries[] analog_series = new FunctionSeries[channels_enabled.Length];
                FunctionSeries[,] digital_series = new FunctionSeries[channels_enabled.Length, _digital_graph.Length];

                // initialize series arrays
                for (int i = 0; i < analog_series.Length; i++)
                {
                    analog_series[i] = new FunctionSeries();
                }

                for (int i = 0; i < channels_enabled.Length; i++)
                {
                    for (int j = 0; j < _digital_graph.Length; j++)
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
                        // set decimator param
                        txt_horizontal_divisions.Invoke((MethodInvoker)delegate { SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".decimator", (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5 + 1) - 1, _scisdk_handle); });

                        // set trigger channel param
                        int value = 0;
                        int cmb_trigger_source_selected_index = 0;
                        cmb_trigger_source.Invoke((MethodInvoker)delegate { cmb_trigger_source_selected_index = cmb_trigger_source.SelectedIndex; });
                        if (cmb_trigger_source_selected_index == 0)
                        {
                            cmb_trigger_edge.Invoke((MethodInvoker)delegate { value = Convert.ToInt32(cmb_trigger_edge.SelectedIndex.ToString() + "000", 2); });
                        }
                        else if (cmb_trigger_source_selected_index == 1)
                        {
                            cmb_trigger_edge.Invoke((MethodInvoker)delegate { value = Convert.ToInt32("1000" + cmb_trigger_edge.SelectedIndex.ToString() + "010", 2); });
                        }
                        else
                        {
                            int ch_addr = cmb_trigger_source_selected_index - 2;
                            cmb_trigger_edge.Invoke((MethodInvoker)delegate { value = (ch_addr << 8) + (cmb_trigger_edge.SelectedIndex << 3) + 1; });
                        }
                        SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_channel", value, _scisdk_handle);

                        // set pretrigger param
                        track_pre.Invoke((MethodInvoker)delegate { SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".pretrigger", track_pre.Value * (1024 / 100), _scisdk_handle); });

                        // set trigger level value
                        track_trigger_level.Invoke((MethodInvoker)delegate { SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_level", track_trigger_level.Value, _scisdk_handle); });

                        // set trigger mode param
                        cmb_trigger_mode.Invoke((MethodInvoker)delegate { SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".trigger_mode", "self", _scisdk_handle); });

                        if (SciSdk_Wrapper.ReadData(_oscilloscope_base_path, ref buffer_ptr, _scisdk_handle))
                        {
                            // convert buffer pointer to buffer struct
                            buffer_struct = new Oscilloscope_decoded_buffer_struct();
                            buffer_struct = (Oscilloscope_decoded_buffer_struct)Marshal.PtrToStructure(buffer_ptr, typeof(Oscilloscope_decoded_buffer_struct));

                            // read memory and store analog values inside an integer type array
                            int[] analog_values = new int[buffer_struct.info.samples_analog * buffer_struct.info.channels];
                            Marshal.Copy(buffer_struct.analog, analog_values, 0, analog_values.Length);

                            // remove analog traces from the graph
                            _analog_graph.GetModel().Series.Clear();
                            for (int i = 0; i < analog_series.Length; i++)
                            {
                                analog_series[i].Points.Clear();
                            }

                            int decimator = (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5);

                            // plot analog trace
                            for (int j = 0; j < buffer_struct.info.channels; j++)
                            {
                                if (channels_enabled[j] == true)
                                {
                                    for (int i = 0; i < analog_values.Length; i++)
                                    {
                                        analog_series[j].Points.Add(new DataPoint(i * decimator, analog_values[(j * buffer_struct.info.samples_analog) + i]));
                                    }
                                    _analog_graph.GetModel().Series.Add(analog_series[j]);
                                }
                            }

                            // invalidate graph (oxyplot repaints it)
                            _analog_graph.GetModel().InvalidatePlot(true);

                            // read memory and store digital values inside an integer type array
                            byte[] digital_values = new byte[buffer_struct.info.samples_digital * buffer_struct.info.channels];
                            Marshal.Copy(buffer_struct.digital, digital_values, 0, digital_values.Length);

                            // remove series from all graphs
                            for (int i = 0; i < _digital_graph.Length; i++)
                            {
                                _digital_graph[i].GetModel().Series.Clear();
                            }

                            // plot digital values on graphs
                            for (int i = 0; i < buffer_struct.info.channels; i++)
                            {
                                if (channels_enabled[i] == true)
                                {
                                    for (int j = 0; j < buffer_struct.info.tracks_digital_per_channel; j++)
                                    {
                                        // clear serie
                                        digital_series[i, j].Points.Clear();

                                        // add new values to serie
                                        for (int k = 0; k < buffer_struct.info.samples_digital; k++)
                                        {
                                            digital_series[i, j].Points.Add((new DataPoint(k * decimator, digital_values[i * buffer_struct.info.tracks_digital_per_channel + j + i])));
                                        }
                                        _digital_graph[j].GetModel().Series.Add(digital_series[i, j]);
                                    }
                                }
                            }

                            // invalidate digital graphs (oxyplot repaints them)
                            for (int i = 0; i < _digital_graph.Length; i++)
                            {
                                _digital_graph[i].GetModel().InvalidatePlot(true);
                            }
                        }
                        else
                        {
                            lbl_status.Invoke((MethodInvoker)delegate { lbl_status.Text = "INVALID DATA"; });
                            lbl_status.Invoke((MethodInvoker)delegate { stop_btn.PerformClick(); });
                        }
                        Thread.Sleep(75);
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