#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include "EEPROM.h"
#include <ArduinoOTA.h>
#include <FastLED.h>
#include <BleKeyboard.h>
#include "OneButton.h"
#define NUM_LEDS 34      //LED 数量
#define DATA_PIN 4       //LED 引脚
#define PIN_INPUT 0      //编码器按钮引脚
#define Pin_A 12         //编码器引脚
#define Pin_B 14         //编码器引脚
#define OFF_TIME 30      //关灯时间
#define VIBRATE_PIN 18  //震动马达
int number_click;
int count = 0;
int old_count=0;
bool flag=0;
 int c;
long unsigned TIME=0;
long unsigned DELAY_TIME=0;
CRGB leds[NUM_LEDS];
bool WIFI_Status = true;  
OneButton button(PIN_INPUT, true);
BleKeyboard bleKeyboard("蓝牙媒体键盘","Esp32",98);
int bright;
int ColorIndex;
int ColorNum=150;
//预设颜色表
const CRGB Colos[] PROGMEM = {
    CRGB(255, 0, 0),  CRGB(255, 10, 0), CRGB(255, 20, 0), CRGB(255, 30, 0), CRGB(255, 40, 0), CRGB(255, 50, 0), CRGB(255, 60, 0), CRGB(255, 70, 0), CRGB(255, 80, 0), CRGB(255, 90, 0),
    CRGB(255, 100, 0),CRGB(255, 110, 0),CRGB(255, 120, 0),CRGB(255, 130, 0),CRGB(255, 140, 0),CRGB(255, 150, 0),CRGB(255, 160, 0),CRGB(255, 170, 0),CRGB(255, 180, 0),CRGB(255, 190, 0),
    CRGB(255, 200, 0),CRGB(255, 210, 0),CRGB(255, 220, 0),CRGB(255, 230, 0),CRGB(255, 240, 0),CRGB(255, 255, 0),CRGB(240, 255, 0),CRGB(230, 255, 0),CRGB(220, 255, 0),CRGB(210, 255, 0),
    CRGB(200, 255, 0),CRGB(190, 255, 0),CRGB(180, 255, 0),CRGB(170, 255, 0),CRGB(160, 255, 0),CRGB(150, 255, 0),CRGB(140, 255, 0),CRGB(130, 255, 0),CRGB(120, 255, 0),CRGB(110, 255, 0),
    CRGB(100, 255, 0),CRGB(90, 255, 0), CRGB(80, 255, 0), CRGB(70, 255, 0), CRGB(60, 255, 0), CRGB(50, 255, 0), CRGB(40, 255, 0), CRGB(30, 255, 0), CRGB(20, 255, 0), CRGB(10, 255, 0), 
    CRGB(0, 255, 0),  CRGB(0, 255, 10), CRGB(0, 255, 20), CRGB(0, 255, 30), CRGB(0, 255, 40), CRGB(0, 255, 50), CRGB(0, 255, 60), CRGB(0, 255, 70), CRGB(0, 255, 80), CRGB(0, 255, 90), 
    CRGB(0, 255, 100),CRGB(0, 255, 110),CRGB(0, 255, 120),CRGB(0, 255, 130),CRGB(0, 255, 140),CRGB(0, 255, 150),CRGB(0, 255, 160),CRGB(0, 255, 170),CRGB(0, 255, 180),CRGB(0, 255, 190),
    CRGB(0, 255, 200),CRGB(0, 255, 210),CRGB(0, 255, 220),CRGB(0, 255, 230),CRGB(0, 255, 240),CRGB(0, 255, 255),CRGB(0, 240, 255),CRGB(0, 230, 255),CRGB(0, 220, 255),CRGB(0, 210, 255),
    CRGB(0, 200, 255),CRGB(0, 190, 255),CRGB(0, 180, 255),CRGB(0, 170, 255),CRGB(0, 160, 255),CRGB(0, 150, 255),CRGB(0, 140, 255),CRGB(0, 130, 255),CRGB(0, 120, 255),CRGB(0, 110, 255),
    CRGB(0, 100, 255),CRGB(0, 90, 255), CRGB(0, 80, 255), CRGB(0, 70, 255), CRGB(0, 60, 255), CRGB(0, 50, 255), CRGB(0, 40, 255), CRGB(0, 30, 255), CRGB(0, 20, 255), CRGB(0, 10, 255), 
    CRGB(0, 0, 255),  CRGB(10, 0, 255), CRGB(20, 0, 255), CRGB(30, 0, 255), CRGB(40, 0, 255), CRGB(50, 0, 255), CRGB(60, 0, 255), CRGB(70, 0, 255), CRGB(80, 0, 255), CRGB(90, 0, 255), 
    CRGB(100, 0, 255),CRGB(110, 0, 255),CRGB(120, 0, 255),CRGB(130, 0, 255),CRGB(140, 0, 255),CRGB(150, 0, 255),CRGB(160, 0, 255),CRGB(170, 0, 255),CRGB(180, 0, 255),CRGB(190, 0, 255),
    CRGB(200, 0, 255),CRGB(210, 0, 255),CRGB(220, 0, 255),CRGB(230, 0, 255),CRGB(240, 0, 255),CRGB(255, 0, 255),CRGB(255, 0, 240),CRGB(255, 0, 230),CRGB(255, 0, 220),CRGB(255, 0, 210),
    CRGB(255, 0, 200),CRGB(255, 0, 190),CRGB(255, 0, 180),CRGB(255, 0, 170),CRGB(255, 0, 160),CRGB(255, 0, 150),CRGB(255, 0, 140),CRGB(255, 0, 130),CRGB(255, 0, 120),CRGB(255, 0, 110),
    CRGB(255, 0, 100),CRGB(255, 0, 90), CRGB(255, 0, 80), CRGB(255, 0, 70), CRGB(255, 0, 60), CRGB(255, 0, 50), CRGB(255, 0, 40), CRGB(255, 0, 30), CRGB(255, 0, 20), CRGB(255, 0, 10),
};


//初始化
void setup() {
  pinMode(18,OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(100);
  bright=EEPROM.read(10);
  delay(3);
  ColorIndex=EEPROM.read(20) ;
  delay(3);
  pinMode(4, OUTPUT);
  pinMode(Pin_A, INPUT_PULLUP);
  pinMode(Pin_B, INPUT_PULLUP);
  attachInterrupt(Pin_A, ClockChanged, CHANGE);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
   
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(100);
  //AutoConfig();//不需要WIFI可注释掉
  // ArduinoOTA.begin();//不需要OTA注释掉，包括LOOP里的ArduinoOTA.handle();
  button.attachClick(oneclick);//关联单击事件
  button.attachDoubleClick(doubleclick);//关联双击事件
  button.attachDuringLongPress(longPressStop);//关联长按结束事件
  button.attachMultiClick(attachMultiClick);//多次点击事件
}

void loop() {
  //ArduinoOTA.handle();
  //检测按键
  button.tick();

  OFF_M();
  xuanniu();
  EVERY_N_SECONDS(1){ 
    //Serial.print(TIME);
    TIME++;
    if(TIME>10){
     // Serial.print("LED_OFF");
      for(int i=0;i<NUM_LEDS;i++){
        leds[i]=CRGB::Black;
        };
        FastLED.show();
        TIME=0;
      }
    }
  switch(number_click){
    case 3:{ threes_click();break;}
    case 4:{ four_click();break;}  
  } 
} 


//中断处理程序,(注意中断里不能有打印语句,会出现看门狗超时)
void ClockChanged(){ 
   
  bool a = digitalRead(Pin_A);//读取CLK引脚的电平
  bool b = digitalRead(Pin_B);//读取DT引脚的电平
   static bool last_a;
   static unsigned long last_time;
   
   if(last_a!=a&&millis()-last_time>4){
    last_a=a;
    if(++c==2){
      c=0;
      old_count=count;
      count+=(a!=b?1:-1); 
      }
    last_time=millis();
  }
}

void xuanniu(){
  if(old_count!=count){
   
    TIME=0;
    FastLED.clear();
   
    old_count>count?(bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN)):(bleKeyboard.write(KEY_MEDIA_VOLUME_UP));
    LEDSHOW();
    delay(2);
    
    digitalWrite(18,HIGH);
    DELAY_TIME=millis();
    old_count=count;
  }
}

//显示LED
void LEDSHOW(){
   static unsigned long old_time;
   
   if(count>=0){
      leds[count%NUM_LEDS]=Colos[ColorIndex];
      FastLED.setBrightness(bright);
      FastLED.show();
  }
  else{
     leds[33-(abs(count)%NUM_LEDS)]=Colos[ColorIndex];
     FastLED.setBrightness(bright);
     FastLED.show();
  
  }     
}
  


//按三次修改亮度
void threes_click(){
    digitalWrite(18,HIGH);
    delay(100);
    Serial.println("开始拉高18脚");
    digitalWrite(18,LOW);

  
  flag=0;
  count=bright;
 
  while(1){
     OFF_M();
     button.tick();
     if(count>=0){ 
         if(count>250){
            count=250;
             }
      bright=count;   
      // myHSVcolor.h=H;
      // myHSVcolor.v=bright;
      fill_solid(leds,NUM_LEDS, Colos[ColorIndex]); 
      FastLED.setBrightness(bright);
      FastLED.show();
       }
    else{count=0;}
     if(flag==1){
      number_click=0;
      
      for(int i=0;i<NUM_LEDS;i++){
        leds[i]=CRGB::Black;
        FastLED.show();
      }
      EEPROM.write(10, bright); 
      EEPROM.commit(); 
      break;
    }
  }
}

//按四次修改颜色
void four_click(){
    digitalWrite(18,HIGH);
    delay(100);
    Serial.println("开始拉高18脚");
    digitalWrite(18,LOW);
   flag=0;
   count=ColorIndex;
  while(1){
    OFF_M();
    button.tick();

    int index=0;
    if(count>=0){
      index=count%ColorNum;
    }
    else{
      index=(ColorNum-1)-(abs(count)%ColorNum);
    }     

    ColorIndex=index;
     if(count>=0){ 
      //    if(count>250){
      //       count=250;
      //        }
      // H=count;
      // // myHSVcolor.h=H;
      // // myHSVcolor.v=bright;
      // // myRGBcolor=HSV2RGB(myHSVcolor);
      // myRGBcolor.g=H;
      // Serial.printf("count：%d。\r\n",count);
      // Serial.printf("sizeof(Colos)：%d。\r\n",sizeof(Colos));
      // Serial.printf("ColorIndex：%d。\r\n",ColorIndex);
      fill_solid(leds,NUM_LEDS, Colos[ColorIndex]); 
      FastLED.setBrightness(bright);
      FastLED.show();
      
      }
    else{count=0;}
     if(flag==1){
      for(int i=0;i<NUM_LEDS;i++){
        leds[i]=CRGB::Black;
         FastLED.show();
        }
      
      EEPROM.write(20, ColorIndex); 
      EEPROM.commit();
      number_click=0;
      break;
    }
  }
}

//检测是否关闭LED
void OFF_M(){
if((millis()-DELAY_TIME>OFF_TIME)&&digitalRead(18)){
   digitalWrite(18,LOW);
    //Serial.println("OFF");
  }
}

//多次按下事件
void attachMultiClick(){
 Serial.printf("getNumberClicks-总共按了：%d次。\r\n",button.getNumberClicks());
  switch(button.getNumberClicks()){
      case 3:{Serial.printf("switch语句判断出打印3次。\r\n");number_click=3;break;}
      case 4:{Serial.printf("switch语句判断出打印4次。\r\n");number_click=4;break;}
      case 5:{Serial.printf("switch语句判断出打印5次。\r\n");break;}
      case 6:{Serial.printf("switch语句判断出打印6次。\r\n");break;}
      default:{Serial.printf("switch语句判断出打印其它次数:[%d]。\r\n",button.getNumberClicks());break;
    }
  }
}
void longPressStop(){
   flag=1;
   Serial.println("长按结束");
}

//双击按钮、下一首曲目
void doubleclick(){
  Serial.println("双击");
  bleKeyboard.write(KEY_MEDIA_NEXT_TRACK );
}

//单击按钮、播放或暂停
void oneclick(){
  Serial.println("单击");
  bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
}






void smartConfig(){    //配网
  WiFi.mode(WIFI_STA);//设置STA模式
  Serial.println("\r\nWait for Smartconfig...");//打印log信息
  WiFi.beginSmartConfig();//开始SmartConfig，等待手机端发出用户名和密码
  while(1)
  {
    Serial.println(".");
    blink_led0(1000,1);
    delay(500);
   
    if(WiFi.smartConfigDone())//配网成功，接收到SSID和密码
    {
      Serial.println("配网成功");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;      
    }
  }  
}
void AutoConfig(){
  WiFi.begin();
  Serial.println("\r\n正在连接WIFI...");

    while(WiFi.status()!=WL_CONNECTED)//判断是否连接WIFI成功
    { 
      
      int count;
    
      if(WIFI_Status)
      {
          Serial.print(".");
         blink_led0(1000,0);
         delay(100);
         
          count++;
          if(count>=300)//等待0.5分钟连接失败，配网
          {
              WIFI_Status = false;
              
              Serial.println("WiFi连接失败，请用手机进行配网"); 
          }        
        }
        else
        {
          smartConfig();  //微信智能配网
        }
    }
    
    Serial.println("连接成功");  
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
    leds[0]=CRGB::White; 
    FastLED.show();
}
void blink_led0(int a,int b){
  static bool flag;
  EVERY_N_MILLISECONDS(a){
     if(b==0){   
      if(flag==0){
      leds[0]=CRGB::Red; 
      FastLED.show();
      flag=1;
      }
      if(flag==1){
      leds[0]=CRGB::Black; 
      FastLED.show();
      flag=0;
      }
     }
     if(b==1){
      static bool flag;
      if(flag==0){
      leds[0]=CRGB::Yellow; 
      FastLED.show();
      flag=1;
      }
      if(flag==1){
      leds[0]=CRGB::Black; 
      FastLED.show();
      flag=0;
      }
     }
     };
     }


 











     
