#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const char *ssid = "Hihihehe";  //ssid wifi mà mạch sẽ kết nối
const char *password = "Hihihehehe@95";//password wifi
//SoftwareSerialE loraSerial(13, 15, false, 256);
SoftwareSerial loraSerial(13, 15); // TX, RX
//transmit

const int M0 =4 ;   // M0 de set up MODE cho Module Lora
const int M1 =5 ;   // M1 de set up MODE cho Module Lora

String statusOfLight="0";
String turnOff = "off";


void setup() {
    pinMode(M0,OUTPUT);
    pinMode(M1, OUTPUT);
    digitalWrite(M0,LOW);
    digitalWrite(M1,LOW);
 
  Serial.begin(9600);
  loraSerial.begin(9600);
   //kết nối mạch với wifi
         WiFi.mode(WIFI_OFF);     
         delay(1000);
         WiFi.mode(WIFI_STA);
  
        WiFi.begin(ssid, password);     //Kết nối đến wifi route
      // chờ đợi kết nối đến wifi router
      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
  Serial.print("Kết nối thành công đến ");
  Serial.println(ssid);
  
  
  Serial.println("I AM LORA GATEWAY ");
  statusOfLight="0";
}

void loop() {
    sendDataHumiditySoil();
    Serial.println("-------------");
    getStatusOfLED();
    Serial.println("-------------");
       delay(5000);
}

/**
 * gọi api hỏi trạng thái của đèn 
 * Sau khi nhận được thông tin trạng thái của đèn sẽ gửi trạng thái đến lora node (đèn)
 */
void getStatusOfLED(){
        HTTPClient http;    //Sử dụng httpclient để gọi api

        http.begin("http://iots16.azurewebsites.net/api/GetLightStatusCurrent");
        http.setTimeout(30000); // set timeout 30s
    
        int httpCode=http.GET();
        String payload = http.getString();    //get response body
        //chuyển đổi response body dạng string thành json object
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(payload);;;;;;;;;;;;
        boolean isSuccess = root[String("success")];

        //nếu gọi api thành công nhận được httpCode 200 OK
        if(httpCode==200){
          if(isSuccess == 1){
              DynamicJsonBuffer jsonBuf;
              String lightData = root[String("data")][0];
              JsonObject& dataObject = jsonBuf.parseObject(lightData);
              //parse chuỗi json trong response nhận được để lấy trạng thái hiện tại của đèn
              String statusLight = dataObject[String("Status")];
            
              String statusLED="";
              if(statusLight=="1"){
                statusLED="on";
              }else{
                statusLED="off";
              }
              loraSerial.print(statusLED);
              Serial.println("Trạng thái hiện tại của đèn là: "+statusLED);
 
          }else{
              String error = root[String("description")];
              //hiển thị lỗi
              Serial.println(error);
            }    
       }else{
            Serial.println("thất bại");
        }
       http.end();  //Đóng kết nối
  }

  /**
   * Hàm nhận dữ liệu độ ẩm từ lora node và gửi đến server thông qua api
   * 
   * */
void sendDataHumiditySoil(){
    if(loraSerial.available()){
        String mosData = loraSerial.readString();
        HTTPClient http;    //Sử dụng httpclient để gọi api

        String postData= "?deviceid=2&data="+mosData;
        http.begin("http://iots16.azurewebsites.net/api/PostHumidity"+postData);
        http.setTimeout(30000); // set timeout 30s
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //add content-type header
    
        int httpCode = http.POST(postData);   //gửi request method POST kèm request body
    
        String payload = http.getString();    //get response body
       
        //chuyển đổi response body dạng string thành json object
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(payload);
        boolean status = root[String("success")];
        
        //nếu gọi api thành công nhận được httpCode 200 OK
        if(httpCode==200 && status ==1){
           Serial.println("Gửi thông tin độ ẩm đất thành công: độ ẩm là: "+mosData+"%");
        }else{
           Serial.println("Gửi thông tin độ ẩm đất thất bại");
           Serial.println("Độ ẩm đất hiện tại: "+mosData+"%");
        }
        http.end();  //Đóng kết nối
    }
}
