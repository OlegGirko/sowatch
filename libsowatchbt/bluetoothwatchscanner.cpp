#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>

#include "bluetoothwatchscanner.h"

using namespace sowatch;


int BluetoothWatchScanner::_instances = 0;
QBluetoothServiceDiscoveryAgent *BluetoothWatchScanner::_agent = 0;

BluetoothWatchScanner::BluetoothWatchScanner(QObject *parent) :
	WatchScanner(parent)
{
	if (_instances == 0) {
		Q_ASSERT(!_agent);
		_agent = new QBluetoothServiceDiscoveryAgent;
	}
	_instances++;
	Q_ASSERT(_agent);
}

BluetoothWatchScanner::~BluetoothWatchScanner()
{
	_instances--;
	if (_instances == 0) {
		delete _agent;
		_agent = 0;
	}
}

void BluetoothWatchScanner::start()
{
	Q_ASSERT(_agent);
	connect(_agent, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(_agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
			this, SLOT(handleDiscoveredService(QBluetoothServiceInfo)));

	if (!_agent->isActive()) {
		qDebug() << "started bluetooth scan";
		_agent->start();
	}

	emit started();
}
