#include "U8glib.h"//引入OLED库
#include <ESP8266.h>//调用库
#include <avr/sleep.h>
#include <avr/power.h>
#include <IRremote.h>//引入红外解码库
int RECV_PIN = 10;   //红外线接收器OUTPUT端接在pin 10
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif
/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define EspSerial mySerial
#define UARTSPEED  9600
#endif
#define SSID        F("")//热点名称
#define PASSWORD    F("")//热点密码
#define HOST_NAME   F("api.heclouds.com")
#define HOST_PORT   (80)
int nowtime=1;//通过时间前后判断弹幕先后顺序
unsigned long currentMillis=0;
//unsigned long previousMillis=0；
static const byte  GETDATA[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/561746562/datapoints?datastream_id=value&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:SXop0l4xynA9GrZFJNKPB0Vu3=g=\r\nConnection: close\r\n\r\n"
};
ESP8266 wifi(&EspSerial);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
String s[4];//用于存放读取数据的字符串数组
void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}//绘图准备
void u8g_box_frame(uint8_t a,String s[]) {
  u8g.drawStr( 2.8*a, 10, s[0].c_str());
  u8g.drawStr( 2.5*a+6, 25, s[1].c_str());
  u8g.drawStr( 2*a-10, 40, s[3].c_str());
  u8g.drawStr( 2.5*a, 65, s[4].c_str());
}//字符串运动
uint8_t draw_state = 0;
void draw(String s[]) {
  u8g_prepare();
  switch (draw_state >> 300) {
    case 0: u8g_box_frame(draw_state & 700,s); break;
  }
}//绘制弹幕

void setup(void) {
  Serial.begin(115200);//设置串口
  //执行一次set up先读取一次数据
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
  delay(100);
  irrecv.enableIRIn(); // 启动红外解码
  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));
  u8g.setColorIndex(1);         
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));
  } else {
    Serial.print(F("create tcp err\r\n"));
  }

  //char *hello = "GET /testwifi/index.html HTTP/1.0\r\nHost: www.adafruit.com\r\nConnection: on\r\n\r\n";
  //wifi.send((const uint8_t*)hello, strlen(hello));  //直接发送

  wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存

  uint8_t buffer[512] = {0};//定义存放读取数据的数组
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0) {
    Serial.print(F("Received:["));
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print(F("]\r\n"));
  }//串口监视器打印所读到内容
 String a;
  int i,j;
  char temp[100]={0};
  for(i=0;;i++)
  {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')//判断读取位置
      {j=i+8;
      break;}   
  }
  for(;buffer[j]!='"';j++)
  {
     a+=((char)buffer[j]);
  }
  s[0]=a;//读到第一条弹幕内容
  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
}
void loop(void) {
  currentMillis = millis();//读取系统计时器
  long interval = 50000;           // 闪烁的时间间隔（毫秒）
 Serial.println(currentMillis);
 //两种控制弹幕更新的方法，方法2更加稳定，这里使用方法2
  //if(currentMillis - previousMillis > interval) //方法1：如果清醒时间大于了设置的时间
  if(irrecv.decode(&results)) //方法2：如果收到了接受新内容指令
  {
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
      Serial.print(F("create tcp ok\r\n"));
      } 
      else {
      Serial.print(F("create tcp err\r\n"));
      }
      wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存
      uint8_t buffer[512] = {0};
      uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
      if (len > 0) {
        Serial.print(F("Received:["));
      for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
      }
      Serial.print(F("]\r\n"));
      }
        String a;
        int i,j;
        char temp[100]={0};
        for(i=0;;i++)
        {
          if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
          {j=i+8;
            break;}   
          }
          for(;buffer[j]!='"';j++)
          {
              a+=((char)buffer[j]);
            }
            s[nowtime]=a;
            Serial.print(s[nowtime]);//串口打印弹幕字符
            if (wifi.releaseTCP()) {
                Serial.print(F("release tcp ok\r\n"));
                } 
                else {
                    Serial.print(F("release tcp err\r\n"));
                     }
    //millis()=millis()-30000; //清零清醒时间
    // previousMillis+=60000;
     //currentMillis = 0;
      Serial.println(results.value, HEX);//// 输出红外线解码结果（十六进制）
    irrecv.resume(); //  接收下一个值
   }
//打印最新的一条弹幕
  u8g.firstPage();
  do {
    draw(s);
  } while ( u8g.nextPage() );

  // increase the state
  draw_state++;
if ( draw_state >= 7 * 8 )
    draw_state = 0;
    delay(1);
    nowtime++;
  // rebuild the picture after some delay
}

