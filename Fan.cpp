#include "Fan.h"

std::string Fan::getContent(std::string path) {
	std::string content;
	std::fstream file(path.c_str(), std::ios::in);
	getline(file, content);
	file.close();
	return content;
}

bool Fan::writeContent(std::string path, std::string content) {
	std::fstream file(path.c_str(), std::ios::out);
	file << content;
	file.close();
	return file.good();
}

std::string Fan::itos(int i) {
	std::stringstream iStream;
	iStream << i;
	std::string iString = iStream.str();
	return iString;
}

int Fan::stoi(std::string s) {
	std::stringstream sStream(s);
	int i;
	sStream >> i;
	return i;
}

std::string Fan::checkSpeed(int speed) {
	int minSpeed = this->fanMinSpeed;
	int maxSpeed = this->fanMaxSpeed;
	if (speed < minSpeed) {
		return itos(minSpeed);
	} else if (speed > maxSpeed) {
		return itos(maxSpeed);
	}
	return itos(speed);
}

Fan::Fan() {
	this->fanPath = "";
	this->fanName = "";
	this->fanLabel = "";
	this->fanSpeedCurrent = 0;
	this->fanMinSpeed = 0;
	this->fanMaxSpeed = 0;
}

Fan::Fan(std::string fanPath, std::string fanName) {
	this->fanPath = fanPath;
	this->fanName = fanName;
	this->fanLabel = getContent(this->fanPath + "/" + this->fanName + "_label");
	this->fanSpeedCurrent = stoi(
			getContent(this->fanPath + "/" + this->fanName + "_input"));
	this->fanMinSpeed = 3200;
	this->fanMaxSpeed = 6200;
}

std::string Fan::getPath() {
	return this->fanPath;
}

std::string Fan::getName() {
	return this->fanName;
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
			getContent(this->fanPath + "/" + this->fanName + "_input"));
}

bool Fan::setSpeed(int speed) {
	return writeContent(this->fanPath + "/" + this->fanName + "_output",
			checkSpeed(speed));
}
