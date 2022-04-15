#include <hid.h>
#include <FatFs.h>
#include "ffconf.h"
USBHost Usb;
void setup() {
  // put your setup code here, to run once:
  

  Serial.begin(230400);
  Serial.println("done");
  Serial.println(Usb.getUsbTaskState());
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  Serial.println(Usb.getUsbTaskState());
  Usb.Task();
  Serial.println(Usb.dispatchPkt(12, 0, 0)); 
}
