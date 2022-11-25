# Energy Spectrum

 The energy spectrum is a histogram with the energy on the x-axis and the number of counts on the y-axis. 
 The hisogram is a configurable size memory managed by the IP core in the fpga. The spectrum core increment the bin of the histogram rappresenting the energy of a photon when a photon is detected.
 An energy spectrum has two input:
  - `energy`: a 16 bit value rappresenting the energy of the photon
  - `dv`: a trigger that validate the energy value and increment the bin of the histogram

An energy spectrum has the following parameters:
 - the number of bins
 - the rebin factor
 - the number of bits on the y-axis

Number of bins and number of bits define the memory occupancy of the spectrum indeed they are defined at compile time by SciCompiler.
Rebin factor can be reconifgured at run time by the user.

# Parameters
The following parameters can be configured:

| Parameter         | Acces Mode | Description                                                                              | Default value |
| ----------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| decimator         | R/W        | set x-axis decimation factor                                                             | 0             |

