/*
 *
 *  Created on: 07.07.2012
 *	Author: Tobias Kohlbau <tobias.kohlbau@gmail.com>
 *
 */


#include "Control.h"

std::string Control::getContent(std::string path) {
	std::string content;
	std::fstream file(path.c_str(), std::ios::in);
	getline(file, content);
	file.close();
	return content;
}

bool Control::writeContent(std::string path, std::string content) {
	std::fstream file(path.c_str(), std::ios::out);
	file << content;
	file.close();
	return file.good();
}

std::string Control::itos(int i) {
	std::stringstream iStream;
	iStream << i;
	std::string iString = iStream.str();
	return iString;
}

int Control::stoi(std::string s) {
	std::stringstream sStream(s);
	int i;
	sStream >> i;
	return i;
}

double Control::stod(std::string s) {
	std::stringstream sStream(s);
	double d;
	sStream >> d;
	return d;
}

Control::Control() {
	this->controlPath = "";
	this->controlName = "";
}

Control::Control(std::string controlPath, std::string controlName) {
	this->controlPath = controlPath;
	this->controlName = controlName;
}

std::string Control::getPath() {
	return this->controlPath;
}

std::string Control::getName() {
	return this->controlName;
}

void Control::setPath(std::string path) {
	this->controlPath = path;
}
void Control::setName(std::string name) {
	this->controlName = name;
}
