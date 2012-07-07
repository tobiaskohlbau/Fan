#include <iostream>
#include "Fan.h"

using namespace std;

int main() {
	Fan fan("/sys/devices/platform/applesmc.768", "fan1");
	fan.refresh();
	cout << fan.getSpeedCurrent() << "\n";

	int speed;
	cin >> speed;
	fan.setSpeed(speed);
}
