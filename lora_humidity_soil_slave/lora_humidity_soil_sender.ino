#include <LiquidCrystal_PCF8574.h>
#include <SoftwareSerial.h>
SoftwareSerial loraSerial(2, 3);
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int M0 =4 ;   // M0 de set up MODE cho Module Lora
const int M1 =5 ;   // M1 de set up MODE cho Module Lora
void setup() {
  // thiết lập pin cho sensor đo độ ẩm
  pinMode(A0, INPUT);
    pinMode(M0,OUTPUT);
 pinMode(M1, OUTPUT);
 digitalWrite(M0,LOW);
 digitalWrite(M1,HIGH);

  Serial.begin(9600); 
     loraSerial.begin(9600); 
     lcd.begin(16, 2); // initialize the lcd
   Serial.println("lora hudimity sender: ");
}

void loop() {
  // put your main code here, to run repeatedly:
  int mois = analogRead(A0);
//lấy dữ liệu độ ẩm theo %
String humiData= String(map(mois, 0, 1023, 100, 0));

   Serial.print(mois);
   Serial.print(" -");
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    //in dữ liệu độ ẩm lên màn hình lcd
    lcd.print("Humidity: "+humiData+"%");
    lcd.setCursor(0,1);
    String msg="";
  
  if(mois >= 1000) {  
    msg = "Not in the Soil or DISCONNECTED";
  }else{
 Serial.readString();
  //gửi thông tin độ ẩm đất đến lora gateway
  loraSerial.print(humiData);
  if(mois < 1000 && mois >= 600) {   
    msg= "DRY";
  }

  if(mois < 600 && mois >= 370) {
    msg="HUMID"; 
  }

  if(mois < 370) {  
    msg= "Very HUMID";
  }      
  }                                    
    lcd.print(msg);
    delay(10000);
}
