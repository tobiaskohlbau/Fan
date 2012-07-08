/*
 *
 *  Created on: 07.07.2012
 *	Author: Tobias Kohlbau <tobias.kohlbau@gmail.com>
 *
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>

class Control {
private:
	std::string controlPath;
	std::string controlName;
public:
	Control();
	Control(std::string controlPath, std::string controlName);
	std::string getPath();
	std::string getName();
	void setPath(std::string path);
	void setName(std::string name);
	std::string getContent(std::string path);
	bool writeContent(std::string path, std::string content);
	std::string itos(int i);
	int stoi(std::string s);
	double stod(std::string s);
};

#endif /* CONTROL_H_ */
