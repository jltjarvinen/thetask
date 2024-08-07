
## Build instructions

```
mkdir build
cd build
cmake ..
cmake --build .
```

Binaries:
- client1/client1
- client2/client2

## Architecture

The design is written so that heap is minimally allocated after startup,
only what the threads reserve.

### clientlib

Common library for client1 and client2.

#### Client

Templated library to be generic with the port numbers. Opens TCP sockets, starts
threads to listen those sockets and periodically prints out JSON formatted
report.

#### Control

Clients configuration provides possibility to hook into data reception. Control
opens UDP socket to operate the server based on the data passed with the hook
function.

### client1

Starts the Client with configuration given in Task1.

### client2

Starts the Client with configuration given in Task2. Uses Control to operate
the out1 frequency and amplitude.


## Task 1

What are the frequencies, amplitues and shapes you see on the server outputs?

### out1

Amplitude values range from approximately -5.0 to 5.0.

Shape seem to be a sinusoidal wave.

Frequency is 0.5 Hz as the cycle take about 20 timestamps (2000 ms).

### out2

Amplitude values range from -5.0 to 5.0.

Shape seems to be a triangle wave.

Frequency is 0.25 Hz as the cycle take about 40 timestamps (4000 ms).

### out3

Amplitude values range from approximately 0.0 to 5.0.

Shape isn't really a definite waveform but appears more like a pulse signal with high (5.0) and low (0.0) values.

Frequency isn't applicable as the signal doesn't seem to be periodic.
