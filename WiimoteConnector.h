#ifndef WIIMOTE_H
#define WIIMOTE_H

#define MAX_WIIMOTES				1

#include <thread>
#include <string>
#include <wiiuse.h>

class WiimoteConnector
{
public:
	WiimoteConnector();
	~WiimoteConnector();

	void update();
	std::string getCurrentStatus();

private:
	void initializeWiimote();

	wiimote** _wiimotes;
	std::string _myStatus;
};

#endif