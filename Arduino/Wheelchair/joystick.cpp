#include "joystick.h"

Joystick::Joystick(short int x_axis = A1, short int y_axis = A2, short int t_axis = A3, short int button = 7)
: x_axis(x_axis), y_axis(y_axis), t_axis(t_axis), button(button)
{}

void Joystick::init(ros::NodeHandle& nh) {
  pinMode(button, INPUT_PULLUP);

  if (! nh.getParam("~threshold", &threshold)) { 
        //default values
        threshold = 75;
  }
}


float Joystick::calculate(int analogue, float range){
  float vel;
  if (abs(analogue-offset)- threshold > 0){
    vel = (analogue-offset)/(512/range);
  }else{
    vel = 0;
  }
  return vel;
}


bool Joystick::movement(){
  if (x_velocity() != 0 || y_velocity() != 0  || t_velocity() != 0 || zero == 0){
    zero = 0;
    if (x_velocity() == 0 || y_velocity() == 0  || t_velocity() == 0){
      zero = 1;
    }
    return true;
  }else{
    return false;
  }
}

bool Joystick::pressed_button(int time){
  if (digitalRead(button) == 1){
    timer = millis();
    return 0;
  }else if (millis()-timer > time){
    return 1;
  }else{
    return 0;
  }

}

bool Joystick::get_button(){
  return digitalRead(button);
}


float Joystick::x_velocity(){
  return calculate(analogRead(x_axis), 0.5);
}


float Joystick::y_velocity(){
  return calculate(analogRead(y_axis), 0.35);
}


float Joystick::t_velocity(){
  return calculate(analogRead(t_axis), 0.35);
}
