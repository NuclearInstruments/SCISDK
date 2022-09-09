using CSharp_SciSDK;
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
        SciSDK _sdk;
        OscilloscopeGraph _analog_graph;
        OscilloscopeGraph[] _digital_graph;
        Thread t;
        string _oscilloscope_base_path;

        // class that read data from sdk and plot graph
        public GraphUpdate(SciSDK sdk, OscilloscopeGraph analog_graph, OscilloscopeGraph[] digital_graph, string oscilloscope_base_path)
        {
            _sdk = sdk;
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
            _sdk.SetParameter(_oscilloscope_base_path + ".trigger_level", value);
        }

        // method used to set pretrigger
        public void SetPretrigger(int value)
        {
            _sdk.SetParameter(_oscilloscope_base_path + ".pretrigger", value);
        }

        // method used to set decimator register value
        public void SetDecimator(int decimator)
        {
            _sdk.SetParameter(_oscilloscope_base_path + ".decimator", decimator);
        }

        //method called by thread
        private void ThreadMethod(Thread parent_thread, Button stop_btn, Label lbl_status, TextBox txt_horizontal_divisions, TrackBar track_pre, TrackBar track_trigger_level, bool[] channels_enabled,
            ComboBox cmb_trigger_source, ComboBox cmb_trigger_edge, ComboBox cmb_trigger_mode)
        {
            // set sdk parameters
            _sdk.SetParameter(_oscilloscope_base_path + ".data_processing", "decode");
            _sdk.SetParameter(_oscilloscope_base_path + ".acq_mode", "blocking");
            _sdk.SetParameter(_oscilloscope_base_path + ".timeout", 1000);

            // create oscilloscope buffer
            SciSDKOscilloscopeDecodedBuffer buffer = new SciSDKOscilloscopeDecodedBuffer();

            //// allocate buffer
            if (_sdk.AllocateBuffer(_oscilloscope_base_path, BufferType.BUFFER_TYPE_DECODED, ref buffer) == 0)
            {
                _sdk.ExecuteCommand(_oscilloscope_base_path + ".reset_read_valid_flag", "");

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
                        txt_horizontal_divisions.Invoke((MethodInvoker)delegate { _sdk.SetParameter(_oscilloscope_base_path + ".decimator", (int)Math.Round(Int32.Parse(txt_horizontal_divisions.Text) / 12.5 + 1) - 1); });

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
                        _sdk.SetParameter(_oscilloscope_base_path + ".trigger_channel", value);

                        // set pretrigger param
                        track_pre.Invoke((MethodInvoker)delegate { _sdk.SetParameter(_oscilloscope_base_path + ".pretrigger", track_pre.Value * (1024 / 100)); });

                        // set trigger level value
                        track_trigger_level.Invoke((MethodInvoker)delegate { _sdk.SetParameter(_oscilloscope_base_path + ".trigger_level", track_trigger_level.Value); });

                        // set trigger mode param
                        cmb_trigger_mode.Invoke((MethodInvoker)delegate { _sdk.SetParameter(_oscilloscope_base_path + ".trigger_mode", "self"); });

                        if (_sdk.ReadData(_oscilloscope_base_path, ref buffer) == 0)
                        {
                            // remove analog traces from the graph
                            _analog_graph.GetModel().Series.Clear();
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
                                    _analog_graph.GetModel().Series.Add(analog_series[j]);
                                }
                            }

                            // invalidate graph (oxyplot repaints it)
                            _analog_graph.GetModel().InvalidatePlot(true);
                            
                            // remove series from all graphs
                            for (int i = 0; i < _digital_graph.Length; i++)
                            {
                                _digital_graph[i].GetModel().Series.Clear();
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
            _sdk.FreeBuffer(_oscilloscope_base_path, BufferType.BUFFER_TYPE_DECODED, ref buffer);
        }


    }
}