#include <QtConnectivity/QBluetoothDeviceInfo>
#include <QtConnectivity/QBluetoothAddress>

#include "metawatchscanner.h"

QTM_USE_NAMESPACE
using namespace sowatch;

MetaWatchScanner::MetaWatchScanner(QObject *parent) :
	WatchScanner(parent),
	_agent(new QBluetoothServiceDiscoveryAgent(this))
{
	_agent->setUuidFilter(QBluetoothUuid::SerialPort);
	connect(_agent, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(_agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
			this, SLOT(handleDiscoveredService(QBluetoothServiceInfo)));
}

void MetaWatchScanner::start()
{
	if (_agent->isActive()) {
		_agent->stop();
	}
	_agent->start();
	qDebug() << "started metawatch bluetooth scan";
	emit started();
}

void MetaWatchScanner::handleDiscoveredService(const QBluetoothServiceInfo &info)
{
	const QBluetoothDeviceInfo dev = info.device();
	QString deviceName = dev.name();
	if (deviceName.startsWith("MetaWatch", Qt::CaseInsensitive)) {
		QVariantMap foundInfo;
		foundInfo["address"] = dev.address().toString();
		foundInfo["name"] = deviceName;
		qDebug() << "metawatch bluetooth scan found:" << deviceName;
		// "MetaWatch Digital" was AU2000 with preSTRATA firmware
		// "MetaWatch SW12" seems to be STRATA
		// "MetaWatch 99" seems to be AU2000 with STRATA firmware
		if (deviceName.contains("Digital", Qt::CaseInsensitive) ||
		        deviceName.contains("SW12") || deviceName.contains("99")) {
			foundInfo["driver"] = QString("metawatch-digital");
			foundInfo["next-watchlet-button"] = QString("A");
			emit watchFound(foundInfo);
		// "MetaWatch Analog" is the only analog watch released so far, preSTRATA fw
		// "MetaWatch WDS111" (seems) analog watch with STRATA fw
		} else if (deviceName.contains("Analog", Qt::CaseInsensitive) ||
		           deviceName.contains("WDS111", Qt::CaseInsensitive)) {
			foundInfo["driver"] = QString("metawatch-analog");
			foundInfo["next-watchlet-button"] = QString("A");
			emit watchFound(foundInfo);
		} else {
			qWarning() << "Unknown MetaWatch device found:" << deviceName;
		}
	}
}
