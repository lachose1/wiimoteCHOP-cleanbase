#include "WiimoteConnector.h"

WiimoteConnector::WiimoteConnector() : _myStatus("Initializing")
{
	initializeWiimote();
}

WiimoteConnector::~WiimoteConnector()
{

}

void WiimoteConnector::update()
{

}

std::string WiimoteConnector::getCurrentStatus()
{
	return _myStatus;
}

void WiimoteConnector::initializeWiimote()
{

}