
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *
import sys
import os
import matplotlib.pyplot as plt
import numpy as np

#import numpy as np
print('test imports true')

sdk = SciSDK()
res = 0
res = sdk.AddNewDevice("192.168.102.139:8888", "DT5771", "RegisterFileDT5771-SmartMCA.json", "board0")
if res:
    print('Script exit due to connection error: ' + str(res))
    exit()
print("we made it to connection")

###
print("started acq")
# general digitizer buffer
ret, ddb = sdk.AllocateBuffer("board0:/boardapi",1024*1024*16)
print("going to read out data now...")



    # Read data
rep, ddb = sdk.ReadData("board0:/boardapi", ddb)


if rep == 0:

    # Preparazione dei dati per il plot
    plt.figure(figsize=(10, 6))  # Imposta la dimensione della figura

    for j in range(ddb.n_channels):
        # Crea un array di indici temporali
        time = np.arange(ddb.n_samples[j])
        # Dati del waveform
        waveform = np.array(ddb.waveform[j][:ddb.n_samples[j]])

        # Plot rapido per il canale corrente
        plt.plot(time, waveform, label=f"CH{j}")

    # Personalizzazione del grafico
    plt.title("Waveforms")
    plt.xlabel("Time (samples)")
    plt.ylabel("Amplitude")
    plt.legend(loc="upper right")
    plt.grid(True)

    # Salva il grafico su file
    #plt.savefig("waveforms_plot.png", dpi=300)
    plt.show()

    sdk.FreeBuffer("board0:/boardapi",ddb)
else:  
    print("Error in reading data: " + str(rep))
    exit()