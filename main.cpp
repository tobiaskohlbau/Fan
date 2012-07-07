#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include "Fan.h"
#include "Sensor.h"
#include "Load.h"

#define LOCK_FILE "/var/lock/fan.lock"
#define LOG_FILE "/var/log/fan.log"

void logToFile(const char *filename, const char *message) {
	FILE *logFile;
	logFile = fopen(filename, "a");
	if (!logFile) {
		return;
	}
	fprintf(logFile, "%s\n", message);
	fclose(logFile);
}

void startDaemon() {
	pid_t pid, sid;

	if (getppid() == 1) {
		return;
	}

	pid = fork();

	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(0);
	}

	if (chdir("/") < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

std::string itos(int i) {
	std::stringstream iStream;
	iStream << i;
	std::string iString = iStream.str();
	return iString;
}

std::string dtos(double d) {
	std::stringstream dStream;
	dStream << d;
	std::string dString = dStream.str();
	return dString;
}

double calcAverage(double first, double second) {
	return (first + second) / 2;
}

int main() {

	startDaemon();

	Fan fan("/sys/devices/platform/applesmc.768", "fan1");
	Sensor coreOne(
			"/sys/devices/platform/applesmc.768/subsystem/devices/coretemp.0",
			"temp2");
	Sensor coreTwo(
			"/sys/devices/platform/applesmc.768/subsystem/devices/coretemp.0",
			"temp3");
	Load load("/proc", "stat");

	fan.writeContent(fan.getPath() + "/" + fan.getName() + "_manual", "1");

	double scaleTo = 0.5;
	double maxTemp = 75;

	int speedFactor = (fan.getFanMaxSpeed() - fan.getFanMinSpeed()) / scaleTo;

	while (1) {
		fan.setSpeed(fan.getFanMinSpeed() + speedFactor * load.getAverage());

		if (((coreOne.getTemp() / 1000) > maxTemp)
				|| ((coreTwo.getTemp() / 1000) > maxTemp)) {
			fan.setSpeed(fan.getFanMaxSpeed());
			logToFile(LOG_FILE, "Temp to high");
		}
		sleep(5);
	}

	return 0;
}
