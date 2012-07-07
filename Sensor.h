/*
 * Sensor.h
 *
 *  Created on: 07.07.2012
 *      Author: tobias
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "Control.h"

class Sensor: public Control {
private:
	std::string sensorLabel;
	double sensorTemp;
public:
	Sensor();
	Sensor(std::string sensorPath, std::string sensorName);
	std::string getLabel();
	double getTemp();
};

#endif /* SENSOR_H_ */
