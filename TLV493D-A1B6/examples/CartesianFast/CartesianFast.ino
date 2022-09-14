#include <Tlv493d.h>
#include <util/free_mem.h>

// Tlv493d Opject
Tlv493d Tlv493dMagnetic3DSensor = Tlv493d();

uint32_t line_counter;
int      free_mem, free_mem_min;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  //For the Evalkit "TLV493D-A1B6 MS2GO" uncommend following 3 lines:
  //pinMode(LED2, OUTPUT);	//Sensor-VDD as output
  //digitalWrite(LED2, HIGH);	//Power on the sensor
  //delay(50);
  
  Tlv493dMagnetic3DSensor.begin();
  Tlv493dMagnetic3DSensor.setAccessMode(Tlv493dMagnetic3DSensor.MASTERCONTROLLEDMODE);
  Tlv493dMagnetic3DSensor.disableTemp();
  free_mem_min = freeMemory();
}

void loop() {
  if (line_counter++ % 256 == 0) {
    free_mem = freeMemory();
    if (free_mem < free_mem_min) {
      Serial.print("Free memory:");
      Serial.print(free_mem);
      Serial.print(" ***********************************");
      free_mem_min = free_mem;
      Serial.println();
      delay(2e3);
    }
  }
  delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
  Tlv493d_Error read_status = Tlv493dMagnetic3DSensor.updateData();

  Serial.print(Tlv493dMagnetic3DSensor.getX());
  Serial.print(" ; ");
  Serial.print(Tlv493dMagnetic3DSensor.getY());
  Serial.print(" ; ");
  Serial.print(Tlv493dMagnetic3DSensor.getZ());
  switch (read_status) {
    case TLV493D_NO_ERROR:
      Serial.print(" (");
      Serial.print(Tlv493dMagnetic3DSensor.acquisition_errors);
      Serial.print('/');
      Serial.print(Tlv493dMagnetic3DSensor.frame_count_errors);
      Serial.println(')');
      break;
    case TLV493D_BUS_ERROR:
      Serial.println(" - BUS error");
      break;
    case TLV493D_FRAME_COUNT_ERROR:
      Serial.print(" - FRAME COUNT error (");
      Serial.print(Tlv493dMagnetic3DSensor.frame_count_errors);
      Serial.print(") ");
      Serial.print(Tlv493dMagnetic3DSensor.last_frame_number);
      Serial.print(" : ");
      Serial.println(Tlv493dMagnetic3DSensor.mExpectedFrameCountWas);
      break;
    case TLV493D_ACQUISITION_INCOMPLETE_ERROR:
      Serial.print(" - ACQUISITION INCOMPLETE error (");
      Serial.print(Tlv493dMagnetic3DSensor.acquisition_errors);
      Serial.print(") CH=");
      Serial.print(Tlv493dMagnetic3DSensor.last_channel);
      Serial.println();
      break;
    default:
      Serial.println(" - uncoded error status");
      break;
  }
}
