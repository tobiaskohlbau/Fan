#ifndef FAN_H
#define FAN_H

#include "Control.h"

class Fan : public Control {
private:
	std::string fanLabel;
	int fanMinSpeed;
	int fanMaxSpeed;
	int planedSpeed;
	int checkSpeed(int speed);
public:
	Fan();
	Fan(std::string fanPath, std::string fanName);
	std::string getLabel();
	int getSpeed();
	int getFanMinSpeed();
	int getFanMaxSpeed();
	void setFanMinSpeed(int minSpeed);
	void setFanMaxSpeed(int maxSpeed);
	bool setSpeed(int speed);
	int getPlannedSpeed();
};

#endif
