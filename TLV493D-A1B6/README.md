# TLV493D Magnetometer DEBUG

## Into

This repository was created to host debug Arduino code for Infineon's TLD493D magnetometer IC. Various mods have been made which I don't want to go into to the original library.

## TLDR

The TLVD493D locks up after some period of time, typically after reporting a frame count error.

The apparent solution is to reinitialize the IC after a frame count error is received by calling the library's begin() method again, which will by default call the resetSensor() function which writes to I2C address 0x00 causing the IC to reset.

## Conclusion

I only encountered one instance of a lockup after running CartesianFast.ino about for about 5 hours. I didn't notice if I2C messages were still flying about and just the console output halted, or if the MCU was totally locked up. I didn't have the frame counter fully instrumented so I don't know if the frame counter might have been reset. Unfortunately I never got another error so there were no additional clues from the debug info or gdb inspection in the next 4-1/2 days. I can only add that there were no memory leaks.

Frame Counter behavior: On reset the frame counter resets to 0 and an acquisition is triggered. When the acquisition is complete, the frame counter is incremented. Hence the sensor object's expected frame count value is initialized to 1.

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
