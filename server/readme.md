# Server Side (Phone Side)
This is the part of the code that will be running on the phone.

This will do the following:
+ Read the music file.
+ Send the contents over to the client (laptop) over Wi-Fi.
+ Close when all the data is sent.

## Usage
This code will be cross-compiled on a Linux machine and then transferred to the phone for execution.

### **Cross-Compiling for Android**
1. **Install the cross-compiler** (on Debian-based systems like Ubuntu/Kali):
   ```bash
   sudo apt install gcc-aarch64-linux-gnu
   ```
   This is for 64-bit ARM devices. For 32-bit devices, install:
   ```bash
   sudo apt install gcc-arm-linux-gnueabi
   ```

2. **Compile the code for Android**:
   ```bash
   aarch64-linux-gnu-gcc main.c -o server_android
   ```
   For 32-bit ARM:
   ```bash
   arm-linux-gnueabi-gcc main.c -o server_android
   ```

3. **Transfer the binary to the phone**:
   ```bash
   scp server_android user@android_ip:/data/local/tmp/
   ```
   Alternatively, use ADB:
   ```bash
   adb push server_android /data/local/tmp/
   ```

4. **Run the program on the phone**:
   ```bash
   chmod +x /data/local/tmp/server_android
   ./data/local/tmp/server_android
   ```
   If there are missing library errors, install `libc6-arm64-cross` for shared libraries.

## Planned Features
+ Ability to pause the audio.
+ Change the song being played.

