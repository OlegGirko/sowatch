#include <QtConnectivity/QBluetoothDeviceInfo>
#include <QtConnectivity/QBluetoothAddress>

#include "bluetoothwatchscanner.h"

QTM_USE_NAMESPACE
using namespace sowatch;

BluetoothWatchScanner::BluetoothWatchScanner(QObject *parent) :
	WatchScanner(parent),
	_agent(new QBluetoothServiceDiscoveryAgent(this))
{
	connect(_agent, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(_agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
			this, SLOT(handleDiscoveredService(QBluetoothServiceInfo)));
}

void BluetoothWatchScanner::start()
{
	if (_agent->isActive()) {
		_agent->stop();
	}
	_agent->start();
	qDebug() << "started bluetooth scan";
	emit started();
}

void BluetoothWatchScanner::setUuidFilter(const QBluetoothUuid & uuid)
{
	_agent->setUuidFilter(uuid);
}
