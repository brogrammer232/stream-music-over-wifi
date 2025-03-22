
> This document contains a detailed explanation on how audio data is
sent from the server to the client where it is played.

# Contents
```
1.0.Overview
2.0.Server side in-depth
```

# 1.0.Overview
**Server side:**
The server will do the following:
+ Open the audio file using `libsndfile`.
+ Wait for a signal from the client requesting for audio data.
+ Send a chunk of audio data to the client, not the whole file (1024 frames).
+ Send a signal when all the audio data has been sent.

**Client side:**
The client will do the following:
+ Prepare PortAudio.
+ Request audio data from the server.
+ Receive a chunk of audio data from the server.
+ Store the received audio in a ring buffer.
+ Start audio playback when atleast half of the ring buffer is filled.
+ Refill the buffer when it gets to 50%.


# 2.0.Server side in-depth
#### Receiving the request from the client
+ This will be achieved by the `receive_request()` function.
+ This function will block code execution until a request is received
from the client.
+ The request will be a simple string `"SEND DATA"`.

#### Opening and reading the file
+ The `libsndfile` library will be used to open and read the audio file.
+ **Opening function:** `sf_open()`
+ **Reading function:** `sf_readf_float()`
+ The reading function will be used to read 1024 frames at a time
because this is the amount of frames that will be sent for every request.
