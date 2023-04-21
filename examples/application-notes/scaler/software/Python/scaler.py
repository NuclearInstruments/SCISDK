import os
from scisdk.scisdk import SciSDK
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button

# initialize scisdk library
sdk = SciSDK()

# Add the device DT1260
script_path = os.path.dirname(os.path.realpath(__file__))
res = sdk.AddNewDevice("usb:13250", "dt1260", script_path +
                       "/RegisterFile.json", "board0")

if not res == 0:
    print("Program exit due connection error")
    exit()

# Set registers
err = sdk.SetRegister("board0:/Registers/trgthrs", 2000)
err = sdk.SetRegister("board0:/Registers/pol", 1)

value = 0
# Get register single value
# err, value = sdk.GetRegister("board0:/Registers/trgcnt")

# Get register array value
# if res == 0:
#     for i in range(0, 100):
#         err, value = sdk.GetRegister("board0:/Registers/trgcnt")
#         print(format(value))
# else:
#     print("Error reading register")

# Plot register value
# x, y = [], []
# if res == 0:
#     for i in range(0, 1000):
#         err, value = sdk.GetRegister("board0:/Registers/trgcnt")
#         print("Register value: " + str(value))
#         x.append(i)
#         y.append(value)
#         plt.plot(x, y, color='blue', linewidth=0.5)
#         plt.show(block=False)
#         plt.pause(0.05)
# plt.show()

# Plot with animation
# Create a figure and axis object for the plot
fig, ax = plt.subplots()
x, y = [], []
line, = ax.plot([], [], color='blue', linewidth=0.5)


def update(i):
    global x, y
    if res == 0:
        err, value = sdk.GetRegister("board0:/Registers/trgcnt")
        print("Register value: " + str(value))
        x.append(i)
        y.append(value)
        line.set_data(x, y)
        ax.relim()
        ax.autoscale_view()

    return line,


ani = animation.FuncAnimation(fig, update, interval=10)


# Add buttons
def stop(event):
    ani.event_source.stop()


def start(event):
    ani.event_source.start()


def restart(event):
    ani.event_source.stop()
    x.clear()
    y.clear()
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
