/*
 *
 *  Created on: 07.07.2012
 *	Author: Tobias Kohlbau <tobias.kohlbau@gmail.com>
 *
 */

#include "Sensor.h"

Sensor::Sensor() {
	this->sensorTemp = 0;
}

Sensor::Sensor(std::string sensorPath, std::string sensorName) {
	setPath(sensorPath);
	setName(sensorName);
	this->sensorTemp = getTemp();
}

std::string Sensor::getLabel() {
	return getContent(getPath() + "/" + getName() + "_label");
}

double Sensor::getTemp() {
	return stod(getContent(getPath() + "/" + getName() + "_input"));
}
