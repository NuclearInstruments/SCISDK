using OxyPlot;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Oscilloscope
{
    class GraphUpdate
    {
        IntPtr _scisdk_handle;
        OscilloscopeGraph _analog_graph;
        OscilloscopeGraph[] _digital_graph;
        Thread t;
        string _oscilloscope_base_path;

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
            if(t == null)
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
            // set sdk parameters
            SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".trigger_mode", "self", _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".trigger_level", 1000, _scisdk_handle);
            SciSdk_Wrapper.SetParamInt(_oscilloscope_base_path + ".pretrigger", 20, _scisdk_handle);
            SciSdk_Wrapper.SetParamString(_oscilloscope_base_path + ".data_processing", "decode", _scisdk_handle);


            // start reading
            while (true)
            {
                // read data from sdk

            }
        }
    }
}

/*
 //sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode"));
	////sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking"));
	//sdk.p_error(sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 5000));
	//SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
	//SCISDK_OSCILLOSCOPE_RAW_BUFFER *rb;
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**) &ob));
	//sdk.p_error(sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb));
	//sdk.p_error(sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""));
	////while (1) {
	//	std::ofstream out("c:/temp/output.txt");
	//	sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);
	//	for (int i = 0; i < ob->info.samples_analog; i++) {
	//	out << ob->analog[i] << endl;
	//	}
	//	out.close();
	////}
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
	//	sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_RAW, (void**)&rb);
     */
