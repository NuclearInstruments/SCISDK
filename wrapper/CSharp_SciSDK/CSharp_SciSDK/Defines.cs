using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK
{
    // board models
    public enum SciSDK_DeviceModel : uint
    {
        DT1260 = 0,
        SCIDK = 1,
        DT5550 = 2,
        DT5550W = 3,
        R5560 = 4,
        R5560SE = 5,
        DT5560 = 6,
        V2495 = 7,
        DT2495 = 8,
        V2740 = 9,
        DT2740 = 10
    }

    // buffer types
    public enum BufferType : int
    {
        BUFFER_TYPE_RAW = 0,
        BUFFER_TYPE_DECODED = 1
    }

    // oscilloscope decoded buffer
    public struct SciSDKOscilloscopeDecodedBufferInfo
    {
        public UInt32 samples_analog;
        public UInt32 samples_digital;
        public UInt32 tracks_analog_per_channel;
        public UInt32 tracks_digital_per_channel;
        public UInt32 channels;
    }
    public struct SciSDKOscilloscopeDecodedBuffer
    {
        public UInt32 magic;
        public int[] analog;
        public bool[] digital;
        public UInt32 trigger_position;
        public UInt64 timecode;
        public SciSDKOscilloscopeDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // oscilloscope raw buffer
    public struct SciSDKOscilloscopeRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 samples_analog;
        public UInt32 samples_digital;
        public UInt32 tracks_analog_per_channel;
        public UInt32 tracks_digital_per_channel;
        public UInt32 channels;
    }

    public struct SciSDKOscilloscopeRawBuffer
    {
        public UInt32 magic;
        public UInt32[] data;
        public UInt32 zero_position;
        public UInt32 trigger_position;
        public UInt64 timecode;
        public SciSDKOscilloscopeRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // oscilloscope dual decoded buffer
    public struct SciSDKOscilloscopeDualDecodedBufferInfo
    {
        public UInt32 samples_analog;
        public UInt32 samples_digital;
        public UInt32 tracks_analog_per_channel;
        public UInt32 tracks_digital_per_channel;
        public UInt32 channels;
    }

    public struct SciSDKOscilloscopeDualDecodedBuffer
    {
        public UInt32 magic;
        public Int32[] analog;
        public int[] digital;
        public UInt32 trigger_position;
        public UInt64 timecode;
        public SciSDKOscilloscopeDualDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // oscilloscope dual raw buffer
    public struct SciSDKOscilloscopeDualRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 samples_analog;
        public UInt32 samples_digital;
        public UInt32 tracks_analog_per_channel;
        public UInt32 tracks_digital_per_channel;
        public UInt32 channels;
    }

    public struct SciSDKOscilloscopeDualRawBuffer
    {
        public UInt32 magic;
        public Int32[] data;
        public UInt32 zero_position;
        public UInt32 trigger_position;
        public UInt64 timecode;
        public SciSDKOscilloscopeDualRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // digitizer decoded buffer
    public struct SciSDKDigitizerDecodedBufferInfo
    {
        public UInt32 samples;
        public UInt32 valid_samples;
        public UInt32 channels;
    }

    public struct SciSDKDigitizerDecodedBuffer
    {
        public UInt32 magic;
        public Int32[] analog;
        public UInt64 hits;
        public UInt64 timecode;
        public UInt32 counter;
        public UInt32 user;
        public SciSDKDigitizerDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // digitizer raw buffer
    public struct SciSDKDigitizerRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 samples;
        public UInt32 valid_samples;
        public UInt32 channels;
    }

    public struct SciSDKDigitizerRawBuffer
    {
        public UInt32 magic;
        public Int32[] data;
        public SciSDKDigitizerRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // list raw buffer
    public struct SciSDKListRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 samples;
        public UInt32 valid_samples;
        public UInt32 channels;
    }

    public struct SciSDKListRawBuffer
    {
        public UInt32 magic;
        public byte[] data;
        public SciSDKListRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // custom packet decoded buffer
    public struct SciSDKCPPacket
    {
        public UInt32[] row;
        public UInt32 n;
    }

    public struct SciSDKCPDecodedBufferInfo
    {
        public  UInt32 buffer_size;
        public  UInt32 packet_size;
        public UInt32 valid_data;
    }

    public struct SciSDKCPDecodedBuffer
    {
        public UInt32 magic;
        public SciSDKCPPacket[] data;
        public SciSDKCPDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // custom packet raw buffer
    public struct SciSDKCPRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 valid_data;
    }

    public struct SciSDKCPRawBuffer
    {
        public UInt32 magic;
        public UInt32[] data;
        public SciSDKCPRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // rate meter raw buffer 
    public struct SciSDKRMRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 valid_data;
        public UInt32 nchannels;
    }

    public struct SciSDKRMRawBuffer
    {
        public UInt32 magic;
        public double[] data;
        public SciSDKRMRawBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // spectrum decoded buffer
    public struct SciSDKSpectrumDecodedBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 total_bins;
        public UInt32 valid_bins;
    }

    public struct SciSDKSpectrumDecodedBuffer
    {
        public UInt32 magic;
        public UInt32[] data;
        public UInt64 timecode;
        public UInt32 inttime;
        public SciSDKSpectrumDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // FFT decoded buffer
    public struct SciSDKFFTDecodedBufferInfo
    {
        public UInt32 samples;
        public UInt32 channels;
    }

    public struct SciSDKFFTDecodedBuffer
    {
        public UInt32 magic;
        public double[] mag;
        public double[] ph;
        public UInt64 timecode;
        public SciSDKFFTDecodedBufferInfo info;
        public IntPtr buffer_ptr;
    }

    // FFT raw buffer
    public struct SciSDKFFTRawBufferInfo
    {
        public UInt32 buffer_size;
        public UInt32 samples;
        public UInt32 channels;
    }

    public struct SciSDKFFTRawBuffer
    {
        public UInt32 magic;
        public UInt32[] data;
        public UInt64 timecode;
        public SciSDKFFTRawBufferInfo info;
        public IntPtr buffer_ptr;
    }


}
