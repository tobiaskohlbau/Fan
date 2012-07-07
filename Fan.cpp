#include "Fan.h"

std::string Fan::checkSpeed(int speed) {
	int minSpeed = getFanMinSpeed();
	int maxSpeed = getFanMaxSpeed();
	if (speed < minSpeed) {
		return itos(minSpeed);
	} else if (speed > maxSpeed) {
		return itos(maxSpeed);
	}
	return itos(speed);
}

Fan::Fan() {
	setPath("");
	setName("");
	this->fanLabel = "";
	this->fanSpeedCurrent = 0;
	this->fanMinSpeed = 0;
	this->fanMaxSpeed = 0;
}

Fan::Fan(std::string fanPath, std::string fanName) {
	setPath(fanPath);
	setName(fanName);
	this->fanLabel = getContent(fanPath + "/" + fanName + "_label");
	this->fanSpeedCurrent = stoi(
			getContent(fanPath + "/" + fanName + "_input"));
	this->fanMinSpeed = 3200;
	this->fanMaxSpeed = 6200;
}

std::string Fan::getLabel() {
	return this->fanLabel;
}

int Fan::getSpeedCurrent() {
	return this->fanSpeedCurrent;
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

void Fan::refresh() {
	this->fanSpeedCurrent = stoi(
			getContent(getPath() + "/" + getName() + "_input"));
}

bool Fan::setSpeed(int speed) {
	return writeContent(getPath() + "/" + getName() + "_output",
			checkSpeed(speed));
}
