#include <SoftwareSerial.h>

#define LED1  4  
const int M0 =4 ;   // M0 de set up MODE cho Module Lora
const int M1 =5 ;   // M1 de set up MODE cho Module Lora

// sử dụng 2 pin rx,tx để chuyển dữ liệu
SoftwareSerial loraSerial(2, 3); // TX, RX

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(M0,OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0,LOW);
  digitalWrite(M1,HIGH);

   Serial.begin(9600); 
   loraSerial.begin(9600); 
   Serial.println("lora RECEVICE ");
}

void loop() { 
 if(loraSerial.available()){
  // nhận thông tin trạng thái đèn từ lora gateway
  String statusLight = loraSerial.readString();
    if(statusLight == "on") {
      Serial.println("on");
      //đèn sáng
      digitalWrite(LED1, HIGH); 
    }else if(statusLight == "off"){
      Serial.println("off");
      //đèn tắt
      digitalWrite(LED1, LOW);
   }
 }

}
