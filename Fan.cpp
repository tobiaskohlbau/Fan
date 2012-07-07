#include "Fan.h"

int Fan::checkSpeed(int speed) {
	int minSpeed = getFanMinSpeed();
	int maxSpeed = getFanMaxSpeed();
	if (speed < minSpeed) {
		return minSpeed;
	} else if (speed > maxSpeed) {
		return maxSpeed;
	}
	return speed;
}

Fan::Fan() {
	setPath("");
	setName("");
	this->fanLabel = "";
	this->fanMinSpeed = 0;
	this->fanMaxSpeed = 0;
	this->planedSpeed = 0;
}

Fan::Fan(std::string fanPath, std::string fanName) {
	setPath(fanPath);
	setName(fanName);
	this->fanLabel = getContent(fanPath + "/" + fanName + "_label");
	this->fanMinSpeed = 3200;
	this->fanMaxSpeed = 6200;
	this->planedSpeed = this->fanMinSpeed;
}

std::string Fan::getLabel() {
	return this->fanLabel;
}

int Fan::getSpeed() {
	return stoi(getContent(getPath() + "/" + getName() + "_input"));
}

int Fan::getFanMinSpeed() {
	return this->fanMinSpeed;
}

int Fan::getFanMaxSpeed() {
	return this->fanMaxSpeed;
}

void Fan::setFanMinSpeed(int minSpeed) {
	this->fanMinSpeed = minSpeed;
}

void Fan::setFanMaxSpeed(int maxSpeed) {
	this->fanMaxSpeed = maxSpeed;
}

bool Fan::setSpeed(int speed) {
	return writeContent(getPath() + "/" + getName() + "_output",
			itos(checkSpeed(speed)));
}

int Fan::getPlannedSpeed() {
	return stoi(getContent(getPath() + "/" + getName() + "_output"));
}
