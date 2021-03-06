
#include "EchoSensor.h"
#include "ArduinoSTL.h"
#include <ros.h>
#include "services_and_messages/Echosensors.h"

//define ROS NodeHandler
ros::NodeHandle nh;


//warning message for collision and
//publisher for collision warning message
services_and_messages::Echosensors collision_warn_msg;
ros::Publisher collision_warning_pub("/collision_warning_dir", &collision_warn_msg);



//Ultrasonic sensors
//define all the pins needed for the ultrasonicsensors
#define TRIG_PIN_FRONT 11       //trig-sensor-front
#define TRIG_PIN_LEFT_FRONT 10  //trig-sensor-left-front
#define TRIG_PIN_LEFT_BACK 12   //trig-sensor-left-back
#define TRIG_PIN_RIGHT_FRONT 8  //trig-sensor-right-front
#define TRIG_PIN_RIGHT_BACK 9   //trig-sensor-right-back
#define TRIG_PIN_BACK 13        //trig-sensor-back

#define ECHO_SENSOR_1_PIN 38 //echo-sensor-front
#define ECHO_SENSOR_2_PIN 27 //echo-sensor-left-front
#define ECHO_SENSOR_3_PIN 39 //echo-sensor-left-back
#define ECHO_SENSOR_4_PIN 49 //echo-sensor-right-front
#define ECHO_SENSOR_5_PIN 32 //echo-sensor-right-back
#define ECHO_SENSOR_6_PIN 40 //echo-sensor-back

//implementing different EchoSensor objects
EchoSensor echo_sensor_1 = EchoSensor(TRIG_PIN_FRONT, ECHO_SENSOR_1_PIN);
EchoSensor echo_sensor_2 = EchoSensor(TRIG_PIN_LEFT_FRONT, ECHO_SENSOR_2_PIN);
EchoSensor echo_sensor_3 = EchoSensor(TRIG_PIN_LEFT_BACK, ECHO_SENSOR_3_PIN);
EchoSensor echo_sensor_4 = EchoSensor(TRIG_PIN_RIGHT_FRONT, ECHO_SENSOR_4_PIN);
EchoSensor echo_sensor_5 = EchoSensor(TRIG_PIN_RIGHT_BACK, ECHO_SENSOR_5_PIN);
EchoSensor echo_sensor_6 = EchoSensor(TRIG_PIN_BACK, ECHO_SENSOR_6_PIN);

//put references (pointer to the objets) into a pointer vector for easier use
std::vector<EchoSensor*> echo_all = {&echo_sensor_1, &echo_sensor_2, &echo_sensor_3, &echo_sensor_4, &echo_sensor_5, &echo_sensor_6};


//function for generating the collisono_warn_msg with corresponding direction of the warning
void send_collision_warning(EchoSensor &front, EchoSensor &left_front, EchoSensor &left_back, EchoSensor &right_front, EchoSensor &right_back, EchoSensor &back){

  collision_warn_msg.echo_dir[1] = (left_front.get_echo_dist_warning(350) || left_back.get_echo_dist_warning(350));
  collision_warn_msg.echo_dir[0] = front.get_echo_dist_warning(450);
  collision_warn_msg.echo_dir[3] = back.get_echo_dist_warning(450);
  collision_warn_msg.echo_dir[2] = (right_front.get_echo_dist_warning(350) || right_back.get_echo_dist_warning(350));
  

  //publish the collision_warn_msg to ros
  collision_warning_pub.publish(&collision_warn_msg);
  
}


void setup() {
  Serial.begin(57600);

  while (! Serial) {
    delay(1);
  }

  nh.initNode();
  
  Serial.print("Start programm...");

  //setup all pins for the echosensors
  for (int i = 0; i < echo_all.size(); i++) {
    echo_all[i]->setup_pins();
  }
  delay(10);

  nh.advertise(collision_warning_pub);

}

//variables for testing runtime
/*
unsigned long int lastTime = 0;
unsigned int delta = 0;
unsigned long int sum = 0;
unsigned int m = 0;
std::vector<int> dist = {};
*/

void loop() {
  //m++; //variable for runtime measurement

  //Printing different sensor Messages
  
  for (int i = 0; i < echo_all.size(); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(echo_all[i]->get_distance_in_mm());
    delay(15);
  }
  delay(250);
  send_collision_warning(echo_sensor_1, echo_sensor_2, echo_sensor_3, echo_sensor_4, echo_sensor_5, echo_sensor_6);
  delay(250);
  /*
  
    delta = millis()-lastTime;
    lastTime = millis();
    long int nowTime=lastTime;
    sum += delta;


    if(nowTime > 10000 && nowTime <10040){
    Serial.print("\n");
    Serial.println(m);
    Serial.print("\n");
    Serial.println(sum/m);
    while(1);
  } */

    
    nh.spinOnce();

}
