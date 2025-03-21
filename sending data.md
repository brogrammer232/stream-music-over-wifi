
> This file contains the strategy used to send audio data from the
> server to the client.

# Contents
```
1.0.Overview
2.0.Approach
3.0.Key components
	3.1.Client-side components
	3.2.Server-side components
4.0.Synchronization and latency handling
	4.1.Using Mutexes or Atomic operations
	4.2.Buffering threshold
	4.3.Handling dropouts
5.0.TPC vs UDP for streaming
```

# 1.0.Overview
The system consists of the following:
1. A server that reads an audio file using libsndfile and streams audio
frames over WiFi.
2. A client that receives these frames, buffers them, and plays them
using PortAudio.

The challenge is ensuring low-latency playback while avoiding interruptions
due to nework delays.


# 2.0.Approach
1. The server continuously streams audio in chunks.
2. The client has a circular buffer (ring buffer) to store incoming
audio frames.
3. A separate network thread constantly receives new frames and refills
the buffer.
4. The PortAudio callback function reads from this buffer and plays the
audio.


# 3.0.Key components
## 3.1.Client-side components
1. **Circular buffer (ring buffer):**
	+ Acts as an intermediate storage for audio frames before playback.
	+ Helps absorb network latency variations by preloading some audio.
	+ Needs proper synchronization to avoid conflicts between the
	network thread (writing) and the PortAudio callback (reading).

2. **Network thread:**
	+ Runs separately from the PortAudio callback.
	+ Continously requests and receives audio frames from the server.
	+ Refills the circular buffer to keep playback smooth.
	+ Can use blocking sockets (waiting until space is available in
	the buffer) or non-blocking sockets with a polling mechanism.

3. **PortAudio callback:**
	+ Pulls audio data only from the circular buffer.
	+ If the buffer is too low, it can play silence or repeat the last
	frame to avoid dropouts.
	+ Runs in a real-time low-latency thread, so it must be lightweight.


## 3.2.Server-side components
1. **Audio file reader (libsndfile):**
	+ Read frames from an audio file (e.g.,WAV, FLAC).
	+ Converts the audio to the desired format if necessary (e.g., mono/stereo,
	sample rate conversion).

2. **Streaming module:**
	+ Splits audio into fixed-size frames (e.g., 512, 1024, or 2048
	frames per packet).
	+ Sends frames over WiFi using TCP or UDP.


# 4.0.Synchronization and latency handling
Proper synchronization is needed for the two client threads (one for
playback and one for networking).

## 4.1.Using Mutexes or Atomic operations
1. **Mutexes:**
	+ The network thread locks the buffer while writing new data.
	+ The PortAudio callback locks the buffer while reading.
	+ This ensures no data corruption.

2. **Atomic operations:**
	+ If latency is a concern, a lock-freee atomic circular buffer is
	better than using mutexes.


## 4.2.Buffering threshold
+ The client should wait until a minimum buffer fill level is reached
before starting playback.
+ Example: If the circular buffer can hold 500ms of audio, start playback
only when at least 250ms of audio is available.
+ This helps prevent underruns (buffer running out of data).


## 4.3.Handling dropouts
+ If the buffer runs low, play silence or repeat the last received
frame (basic packet loss concealment).
+ If using UDP, consider adding a simple retransmission request for
lost packets.


# 5.0.TPC vs UDP for streaming
## TCP
+ Ensures all packets arrive in order.
+ Can introduce buffering delays due to retransmissions.
+ Best for lossless audio streaming where minor latency is acceptable.

## UDP
+ Ideal for real-time audio streaming where low latency is critical.
+ If a packet is lost, no retransmission (but we can implement basic
error correction).

## Hybrid approach
+ Use UDP with Forward Error correction (FEC) to recover lost packets
without increasing latency.