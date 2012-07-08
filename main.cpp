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

#define DAEMON_LOG "/var/log/fan.log"

#define FANMINSPEED 3500
//#define FANMAXSPEED 6200
#define LOACSCALEFROM 0.35
#define LOADSCALETO 0.45
#define SENSORSCALEFROM 0.55
#define SENSORSCALETO 0.60
#define AVERAGECOUNT 3
#define SLEEPTIME 2

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

std::string dtos(double d) {
	std::stringstream dStream;
	dStream << d;
	std::string dString = dStream.str();
	return dString;
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

#if FANMINSPEED
	fan.setFanMinSpeed(FANMINSPEED);
#endif
#if FANMAXSPEED
	fan.setFanMaxSpeed(FANMAXSPEED);
#endif

	int loadFactor = (fan.getFanMaxSpeed() - fan.getFanMinSpeed())
			/ (LOADSCALETO - LOACSCALEFROM);
	int sensorFacotor = (fan.getFanMaxSpeed() - fan.getFanMinSpeed())
			/ (SENSORSCALETO - SENSORSCALEFROM);

	while (1) {
		double loadAverage = 0;
		double sensorAverage = 0;
		for (int i = 0; i < AVERAGECOUNT; i++) {
			loadAverage += load.getAverage();
			sensorAverage += (coreOne.getTemp() + coreTwo.getTemp());
			sleep(SLEEPTIME);
		}

		loadAverage = loadAverage / AVERAGECOUNT;
		sensorAverage = sensorAverage / ((2 * AVERAGECOUNT) * 100000);

		logToFile(DAEMON_LOG, dtos(loadAverage).c_str());
		logToFile(DAEMON_LOG, dtos(sensorAverage).c_str());

		if (loadAverage > LOACSCALEFROM) {
			fan.setSpeed(
					fan.getFanMinSpeed()
							+ loadFactor * (loadAverage - LOACSCALEFROM));
		} else {
			fan.setSpeed(
					fan.getFanMinSpeed()
							+ sensorFacotor
									* (sensorAverage - SENSORSCALEFROM));
		}
	}
	return 0;
}
