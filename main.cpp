#include <iostream>
#include "Fan.h"
#include "Sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>

using namespace std;

/* Change this to whatever your daemon is called */
#define DAEMON_NAME "fan"

/* Change this to the user under which to run */
#define RUN_AS_USER "root"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void child_handler(int signum) {
	switch (signum) {
	case SIGALRM:
		exit(EXIT_FAILURE);
		break;
	case SIGUSR1:
		exit(EXIT_SUCCESS);
		break;
	case SIGCHLD:
		exit(EXIT_FAILURE);
		break;
	}
}

static void daemonize(const char *lockfile) {
	pid_t pid, sid, parent;
	int lfp = -1;

	/* already a daemon */
	if (getppid() == 1)
		return;

	/* Create the lock file as the current user */
	if (lockfile && lockfile[0]) {
		lfp = open(lockfile, O_RDWR | O_CREAT, 0640);
		if (lfp < 0) {
			syslog(LOG_ERR, "unable to create lock file %s, code=%d (%s)",
					lockfile, errno, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	/* Drop user if there is one, and we were run as root */
	if (getuid() == 0 || geteuid() == 0) {
		struct passwd *pw = getpwnam(RUN_AS_USER);
		if (pw) {
			syslog(LOG_NOTICE, "setting user to " RUN_AS_USER);
			setuid(pw->pw_uid);
		}
	}

	/* Trap signals that we expect to recieve */
	signal(SIGCHLD, child_handler);
	signal(SIGUSR1, child_handler);
	signal(SIGALRM, child_handler);

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		syslog(LOG_ERR, "unable to fork daemon, code=%d (%s)", errno,
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) {

		/* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
		 for two seconds to elapse (SIGALRM).  pause() should not return. */
		alarm(2);
		pause();

		exit(EXIT_FAILURE);
	}

	/* At this point we are executing as the child process */
	parent = getppid();

	/* Cancel certain signals */
	signal(SIGCHLD, SIG_DFL ); /* A child process dies */
	signal(SIGTSTP, SIG_IGN ); /* Various TTY signals */
	signal(SIGTTOU, SIG_IGN );
	signal(SIGTTIN, SIG_IGN );
	signal(SIGHUP, SIG_IGN ); /* Ignore hangup signal */
	signal(SIGTERM, SIG_DFL ); /* Die on SIGTERM */

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		syslog(LOG_ERR, "unable to create a new session, code %d (%s)", errno,
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory.  This prevents the current
	 directory from being locked; hence not being able to remove it. */
	if ((chdir("/")) < 0) {
		syslog(LOG_ERR, "unable to change directory to %s, code %d (%s)", "/",
				errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Redirect standard files to /dev/null */
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);

	/* Tell the parent process that we are A-okay */
	kill(parent, SIGUSR1);
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
	/* Initialize the logging interface */
	openlog(DAEMON_NAME, LOG_PID, LOG_LOCAL5);
	syslog(LOG_INFO, "starting");

	/* One may wish to process command line arguments here */

	/* Daemonize */
	daemonize("/var/lock/subsys/" DAEMON_NAME);

	/* Now we are a daemon -- do the work for which we were paid */
	Fan fan("/sys/devices/platform/applesmc.768", "fan1");
	Sensor coreOne(
			"/sys/devices/platform/applesmc.768/subsystem/devices/coretemp.0",
			"temp2");
	Sensor coreTwo(
			"/sys/devices/platform/applesmc.768/subsystem/devices/coretemp.0",
			"temp3");

	fan.setSpeed(fan.getPlanedSpeed());

	double oldTemp = ((coreOne.getTemp() + coreTwo.getTemp()) / 2) / 1000;
	double newTemp = oldTemp;
	while (1) {
		syslog(LOG_NOTICE, ("Fanspeed: " + itos(fan.getPlanedSpeed())).c_str());
		syslog(LOG_NOTICE,
				("Cores: "
						+ dtos(
								((coreOne.getTemp() + coreTwo.getTemp()) / 2)
										/ 1000)).c_str());

		newTemp = (coreOne.getTemp() + coreTwo.getTemp()) / 1000;

		syslog(LOG_NOTICE, dtos(oldTemp).c_str());
		syslog(LOG_NOTICE, dtos(newTemp).c_str());

		if (oldTemp - newTemp <= -0.5) {
			if (oldTemp - newTemp <= -5) {
				fan.setPlanedSpeed(fan.getFanMaxSpeed());
			} else {
				fan.setPlanedSpeed(fan.getPlanedSpeed() + 500);
			}
			fan.setSpeed(fan.getPlanedSpeed());
			oldTemp = newTemp;
		} else if (oldTemp - newTemp >= 0.5) {
			fan.setPlanedSpeed(fan.getPlanedSpeed() - 500);
			fan.setSpeed(fan.getPlanedSpeed());
			oldTemp = newTemp;
		}
		sleep(5);
	}

	/* Finish up */
	syslog(LOG_NOTICE, "terminated");
	closelog();
	return 0;
}
