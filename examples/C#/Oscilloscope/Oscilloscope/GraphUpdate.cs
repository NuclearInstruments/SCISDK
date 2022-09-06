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
        public void StartRead()
        {
            if (t == null)
            {
                t = new Thread(ThreadMethod);
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

        //method called by thread
        private void ThreadMethod()
        {
            // create oscilloscope buffer
            buffer_ptr = Marshal.StringToHGlobalAnsi("t");
            Oscilloscope_decoded_buffer_struct buffer_struct;
            //// allocate buffer
            if (SciSdk_Wrapper.AllocateBuffer(_oscilloscope_base_path, SciSdk_Wrapper.T_BUFFER_DECODED, ref buffer_ptr, _scisdk_handle))
           {
                SciSdk_Wrapper.ExecuteCommand(_oscilloscope_base_path + ".reset_read_valid_flag", "", _scisdk_handle);
                FunctionSeries series = new FunctionSeries();
                while (true)
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

                            // display new values on the graph
                            _analog_graph.GetModel().Series.Clear();// remove all series from analog graph
                            series.Points.Clear();

                            //UInt64 timestamp = buffer_struct.timecode;
                            //Int32 div = (Int32)Int64.Parse(timestamp.ToString()) / analog_values.Length;

                            for (int i = 0; i < analog_values.Length; i++)
                            {
                                series.Points.Add(new DataPoint(i, analog_values[i]));
                            }

                            _analog_graph.GetModel().Series.Add(series);
                            _analog_graph.GetModel().InvalidatePlot(true);
                        }
                        else
                        {
                            MessageBox.Show("Error while trying to read data, probably you have opened previously another connection with the board and you haven't already close it", "Error");
                            Thread.CurrentThread.Abort();
                        }

                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e.ToString());
                    }
                }
            }
            // free buffer
            SciSdk_Wrapper.FreeBuffer(_oscilloscope_base_path, SciSdk_Wrapper.T_BUFFER_DECODED, ref buffer_ptr, _scisdk_handle);
        }


    }
}