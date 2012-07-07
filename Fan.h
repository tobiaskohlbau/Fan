#ifndef FAN_H
#define FAN_H

#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>

class Fan {
private:
  std::string fanPath;
  std::string fanName;
  std::string fanLabel;
  int fanSpeedCurrent;
  int fanMinSpeed;
  int fanMaxSpeed;
  std::string getContent(std::string path);
  bool writeContent(std::string path, std::string content);
  std::string itos(int i);
  int stoi(std::string s);
  std::string checkSpeed(int speed);
public:
  Fan();
  Fan(std::string fanPath, std::string fanName);
  std::string getPath();
  std::string getName();
  std::string getLabel();
  int getSpeedCurrent();
  int getFanMinSpeed();
  int getFanMaxSpeed();
  void setFanMinSpeed(int minSpeed);
  void setFanMaxSpeed(int maxSpeed);
  void refresh();
  bool setSpeed(int speed);
};

#endif
