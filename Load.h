/*
 *
 *  Created on: 07.07.2012
 *	Author: Tobias Kohlbau <tobias.kohlbau@gmail.com>
 *
 */

#ifndef LOAD_H_
#define LOAD_H_

#include "Control.h"
#include <unistd.h>

class Load: public Control {
private:
public:
	Load();
	Load(std::string loadPath, std::string loadName);
	double getAverage();
};

#endif /* LOAD_H_ */
