# TLV493D Magnetometer DEBUG

## Into

This repository was created to host debug Arduino code for Infineon's TLD493D magnetometer IC. Various mods have been made which I don't want to go into to the original library.

## TLDR

The TLVD493D locks up after some period of time, typically after reporting a frame count error.

The apparent solution is to reinitialize the IC after a frame count error is received by calling the library's begin() method again, which will by default call the resetSensor() function which writes to I2C address 0x00 causing the IC to reset.

## Conclusion

I only encountered one instance of a lockup after running CartesianFast.ino for about about 5 hours. I didn't notice if I2C messages were still flying about and just the console output halted, or if the MCU was locked up. I didn't have the frame counter fully instrumented so I don't know if the frame counter might have been reset. Unfortunately I never got another error so there were no additional clues from the debug info or gdb inspection in the next 4-1/2 days. I can only add that there were no memory leaks.

### Other Thoughts

#### Frame Counter Behavior

On reset the frame counter resets to 0 and an acquisition is triggered. When the acquisition is complete, the frame counter is incremented. Hence the sensor object's expected frame count value is initialized to 1.

#### Interrupt Access Modes

Access modes FASTMODE, LOWPOWERMODE, and ULTRALOWPOWERMODE are intended to be interrupt driven. The IC's timer will trigger an acquisition. Upon completion, the  SCL line is strobed low. The would trigger the MCU's interrupt service routine to  read the sensor data.

I don't think these modes are used in any of the examples. First off, because access mode MASTERCONTROLLEDMODE is typically fast enough where a new acquisition is triggered after each read. Your processing cycle needs to be fast enough that the data doesn't get stale, but not so fast that the acquisition isn't complete (unlikely) which would induce frame and channel errors. Second, you'd have to turn the SCL interrupt off while you do the I2C read. Kind of clunky.

#### Non-fast Examples

The non-fast example sketches go in and out of power down mode. It seemed like a lot of unnecessary I2C buss traffic and  config changes. Fortunately a corrupted message *should* be ignored if parity checks are used. Maybe it reduces power and gives you current sensor data as opposed to slow reads with MASTERCONTROLLEDMODE. Note that updateData() inserts some delay() calls that might also appear  in client code. Anyway, I'll revisit this topic if I need infrequent updates.

------

## Problem

Output from this IC halts after some number of hours, apparently after a FRAME_ERROR.

## MODS

### Fault Codes

I added and renamed fault codes in Tlv493d.h. The ACQ code replaces the reused FRAME error in the Channel test in Tlvd493d.cpp,

```c++
TLV493D_FRAME_COUNT_ERROR	=	2,
TLV493D_ACQUISITION_INCOMPLETE_ERROR
```

## Public Variables

I added public variable to Tlv493d.h available to example code.  

````C++
  // exposed - no get methods yet
  uint32_t frame_count_errors;
  uint32_t acquisition_errors;
  uint8_t  last_channel;
  uint8_t  last_frame_number;
  uint8_t  mExpectedFrameCountWas;
````

### Tests in Tlv496d::updateData()

I modified the code for the Channel and Frame Counter tests.

### Client Code

**CartesianFast.ino** has been modified. The other examples have not been altered and **may not compile** due to changes in the error code names.
