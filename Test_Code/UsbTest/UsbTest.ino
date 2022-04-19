#include <hid.h>
<<<<<<< Updated upstream
#include <FatFs.h>
#include "ffconf.h"


int code = 0;   
int lastCode = 0;
int i = 0; 

uint32_t* nbytes;                                  
uint8_t data[512];

=======
//#include <FatFs.h>
//#include "ffconf.h"
>>>>>>> Stashed changes
USBHost Usb;

void setup() {
  // put your setup code here, to run once:
  
  
  Serial.begin(230400);
  //Serial.println("done");
  //Serial.println(Usb.getUsbTaskState());
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Serial.println(Usb.getUsbTaskState());
  
  //checkTask();
  Usb.Task();
<<<<<<< Updated upstream
  doStuff();
  
  
  //Serial.println(Usb.dispatchPkt(12, 0, 0)); 
}

void checkTask(){
  code = Usb.getUsbTaskState();
  if (code!= lastCode){
   //messagePrinter();
  }
  lastCode = code;
}



void messagePrinter(){
  switch (code){
    case 0x10:Serial.println("USB_STATE_DETACHED");break;
    case 0x11:Serial.println("USB_DETACHED_SUBSTATE_INITIALIZE");break;
    case 0x12:Serial.println("USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE");break;
    case 0x13:Serial.println("USB_DETACHED_SUBSTATE_ILLEGAL");break;
    case 0x20:Serial.println("USB_ATTACHED_SUBSTATE_SETTLE");break;
    case 0x30:Serial.println("USB_ATTACHED_SUBSTATE_RESET_DEVICE");break;
    case 0x40:Serial.println("USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE");break;
    case 0x50:Serial.println("USB_ATTACHED_SUBSTATE_WAIT_SOF");break;
    case 0x60:Serial.println("USB_ATTACHED_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE");break;
    case 0x70:Serial.println("USB_STATE_ADDRESSING");break;
    case 0x80:Serial.println("USB_STATE_CONFIGURING");break;
    case 0x90:Serial.println("USB_STATE_RUNNING");break;
    case 0xa0:Serial.println("USB_STATE_ERROR");break;
    case 0xf0:Serial.println("USB_STATE_MASK");break;

    // device classes
    case 0x00:Serial.println("USB_CLASS_USE_CLASS_INFO");break;
    case 0x01:Serial.println("USB_CLASS_AUDIO");break;
    case 0x02:Serial.println("USB_CLASS_COM_AND_CDC_CTRL");break;
    case 0x03:Serial.println("USB_CLASS_HID");break;
    case 0x05:Serial.println("USB_CLASS_PHYSICAL");break;
    case 0x06:Serial.println("USB_CLASS_IMAGE");break;
    case 0x07:Serial.println("USB_CLASS_PRINTER");break;
    case 0x08:Serial.println("USB_CLASS_MASS_STORAGE");break;
    case 0x09:Serial.println("USB_CLASS_HUB");break;
    case 0x0a:Serial.println("USB_CLASS_CDC_DATA");break;
    case 0x0b:Serial.println("USB_CLASS_SMART_CARD");break;
    case 0x0d:Serial.println("USB_CLASS_CONTENT_SECURITY");break;
    case 0x0e:Serial.println("USB_CLASS_VIDEO");break;
    case 0x0f:Serial.println("USB_CLASS_PERSONAL_HEALTH");break;
    case 0xdc:Serial.println("USB_CLASS_DIAGNOSTIC_DEVICE");break;
    case 0xe0:Serial.println("USB_CLASS_WIRELESS_CTRL");break;
    case 0xef:Serial.println("USB_CLASS_MISC");break;
    case 0xfe:Serial.println("USB_CLASS_APP_SPECIFIC");break;
    //case 0xff:Serial.println("USB_CLASS_VENDOR_SPECIFIC");break;

    // error codes
    case 0xd1:Serial.println("USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED");break;
    case 0xd2:Serial.println("USB_DEV_CONFIG_ERROR_DEVICE_INIT_INCOMPLETE");break;
    case 0xd3:Serial.println("USB_ERROR_UNABLE_TO_REGISTER_DEVICE_CLASS");break;
    case 0xd4:Serial.println("USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL");break;
    case 0xd5:Serial.println("USB_ERROR_HUB_ADDRESS_OVERFLOW");break;
    case 0xd6:Serial.println("USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL");break;
    case 0xd7:Serial.println("USB_ERROR_EPINFO_IS_NULL");break;
    case 0xd8:Serial.println("USB_ERROR_INVALID_ARGUMENT");break;
    case 0xd9:Serial.println("USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE");break;
    case 0xda:Serial.println("USB_ERROR_INVALID_MAX_PKT_SIZE");break;
    case 0xdb:Serial.println("USB_ERROR_EP_NOT_FOUND_IN_TBL");break;
    case 0xff:Serial.println("USB_ERROR_TRANSFER_TIMEOUT");break;
    
  }
}
          
void doStuff(){
  code = Usb.getUsbTaskState();
  switch (code){
    case 0x90:{
      Usb.inTransfer(0, 0x02, nbytes, data);
      Serial.println(data[0]);
      i++;
      if (i < 512) i = 0;
    }
  }
=======
  //Serial.println(Usb.dispatchPkt(12, 0, 0)); 
>>>>>>> Stashed changes
}
