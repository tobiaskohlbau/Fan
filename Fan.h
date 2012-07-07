#ifndef FAN_H
#define FAN_H

#include "Control.h"

class Fan : public Control {
private:
	std::string fanLabel;
	int fanSpeedCurrent;
	int fanMinSpeed;
	int fanMaxSpeed;
	std::string checkSpeed(int speed);
public:
	Fan();
	Fan(std::string fanPath, std::string fanName);
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
