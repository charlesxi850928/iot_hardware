// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
#include <ESP8266WiFi.h>
#include <Servo.h>
static WiFiClient espClient;
Servo myServo;
// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

// 设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "gwuiXktKj08"
#define DEVICE_NAME "W8WUf7QNGLuFnrRmYdec"
#define DEVICE_SECRET "53b94842bf3957a3130eb5259f817bbd"
#define REGION_ID "cn-shanghai"

// 设置 wifi 信息
//#define WIFI_SSID "CharlesXi850928"
//#define WIFI_PASSWD "Xf851026*%!)@^"
#define WIFI_SSID "TP-LINK_2412"
#define WIFI_PASSWD "18092706884"
int PWM_PIN = D0;
int LED_PIN = D1;

void setup()
{
    Serial.begin(115200);
   
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id
    AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
    
    // 发送一个数据到云平台，LightLuminance 是在设备产品中定义的物联网模型的 id
    AliyunIoTSDK::send("LightLuminance", 100);
    ledInit();
}
void loop()
{
    AliyunIoTSDK::loop();
//   test();
}

// 初始化 wifi 连接
void wifiInit(const char *ssid, const char *passphrase)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
}
void servoInit(){
  myServo.attach(PWM_PIN, 500, 2500);
  myServo.write(0);
}
void ledInit(){
  pinMode(LED_PIN, OUTPUT);
}
void tetPwmMotor_1(int timePwm){
  int i,t;
  for(t=0;t<50;t++){
    for(i=0;i<5*timePwm;i++){
      digitalWrite(PWM_PIN, HIGH);
    }
    for(i=0;i<5*2000;i++){
      digitalWrite(PWM_PIN, LOW);
    }
  }
}
void tetPwmMotor_1(){
  int pos;
  for (pos = 0; pos <= 180; pos ++) { // 0°到180°
    // in steps of 1 degree
    myServo.write(pos);              // 舵机角度写入
//    delay(15);                       // 等待转动到指定角度
  } 
  delay(2000);//延时2s
  for (pos = 180; pos >= 0; pos --) { // 从180°到0°
    myServo.write(pos);              // 舵机角度写入
//    delay(15);                       // 等待转动到指定角度
  }
  delay(2000);//延时2s 
}
void testLED(int HIGH_OR_LOW){
   digitalWrite(LED_PIN, HIGH_OR_LOW);
}
// 电源属性修改的回调函数
void powerCallback(JsonVariant p)
{
    int PowerSwitch = p["PowerSwitch"];
    if (PowerSwitch == 1)
    {
      Serial.println("Charles Xi Testing 1......");
//      myServo.write(0);
    }
    else{
      Serial.println("Charles Xi Testing 0......");
//      myServo.write(180);
    }
    testLED(PowerSwitch);
//    pwm_motor(PowerSwitch);
//    myServo.write(PowerSwitch);
}
