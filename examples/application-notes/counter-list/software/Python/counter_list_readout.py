import os
from scisdk.scisdk import SciSDK
from struct import unpack
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button


# initialize scisdk library
sdk = SciSDK()

# Add the device DT1260
script_path = os.path.dirname(os.path.realpath(__file__))
res = sdk.AddNewDevice("usb:13250", "dt1260", script_path +
                       "/RegisterFile.json", "board0")

if res != 0:
    print("Program exit due connection error")
    exit(-1)

# Set registers
err = sdk.SetRegister("board0:/Registers/trgthrs", 2000)
err = sdk.SetRegister("board0:/Registers/pol", 1)
err = sdk.SetRegister("board0:/Registers/PulsePeriod", 6250000)

# allocate buffer raw, size 1024
buffer_info = sdk.AllocateBuffer("board0:/MMCComponents/List_0", 1024)
if buffer_info is None:
    print("Failed to allocate buffer")
else:
    res, buf = buffer_info

# Set parameters of the List_0
res = sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false")
res = sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode",
                             "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500)

if res != 0:
    print("Error allocating buffer")
    exit(-1)

res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "")

# Read the data buffered
res, buf = sdk.ReadData("board0:/MMCComponents/List_0", buf)

# Print a single value
data = unpack('<I', buf.data[0:4])[0]
print("Data: " + str(data))

# Print an array of values
data = []
for i in range(0, int(buf.info.valid_samples/4)):
    data.append(unpack('<I', buf.data[i*4:(i+1)*4])[0])
print("Data: " + str(data))


# Start the acquisition
res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "")

# Create a figure and axis object for the plot
fig, ax = plt.subplots()
ax.set_xlim(0, 200)
line, = ax.plot([], [], lw=0.7)
x, y = [], []
vline = None


# Read data from the buffer and plot it
def update(frame, buf):
    global x, y, vline
    res, buf = sdk.ReadData("board0:/MMCComponents/List_0", buf)
    if res == 0:
        for i in range(0, int(buf.info.valid_samples/4)):
            data = unpack('<I', buf.data[i*4:(i+1)*4])
            y.append(data)
            x.append(len(y))
            if vline:
                vline.remove()
            vline = ax.axvline(x[-1], color='r', linewidth=0.7)

            if x[-1] >= ax.get_xlim()[-1]:
                line.set_data(x[-1], [y[-1]])
                ax.set_xlim(x[-1], x[-1] + 200)

            line.set_data(x, y)
            ax.relim()
            ax.autoscale_view()

    return line, vline


ani = animation.FuncAnimation(fig, update, fargs=[buf, ], interval=10,)


def stop(event):
    ani.event_source.stop()


def start(event):
    ani.event_source.start()


def restart(event):
    ani.event_source.stop()
    x.clear()
    y.clear()
    ax.set_xlim(0, 200)
    ani.event_source.start()


stop_button = Button(plt.axes([0.7, 0.9, 0.1, 0.075]), 'Stop',
                     hovercolor='red')
stop_button.on_clicked(stop)

start_button = Button(plt.axes([0.8, 0.9, 0.1, 0.075]), 'Start',
                      hovercolor='green')
start_button.on_clicked(start)

restart_button = Button(plt.axes([0.6, 0.9, 0.1, 0.075]), 'Restart',
                        hovercolor='turquoise')
restart_button.on_clicked(restart)

plt.show()
