# TLV493D Magnetometer DEBUG

## Into

This repository was created to host debug Arduino code for Infineon's TLD493D magnetometer IC. Various mods have been made which I don't want to go into to the original library.

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

**Cartesian.ino** has been modified. The other examples have not been altered and **may not compile** due to changes in the error code names.
