#include <QtDebug>

#include "sysinfowatchlet.h"

using namespace sowatch;
QTM_USE_NAMESPACE

SysInfoWatchlet::SysInfoWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, "com.javispedro.sowatch.sysinfo"),
	_devInfo(new QSystemDeviceInfo(this)),
	_netMgr(new QNetworkConfigurationManager(this))
{
	context()->setContextProperty("batteryLevel", 0);
	context()->setContextProperty("networkName", "");
	setSource(QUrl(SOWATCH_QML_DIR "/sysinfowatchlet/" + watch->model() + ".qml"));
	connect(this, SIGNAL(activated()), SLOT(handleActivated()));
	connect(this, SIGNAL(deactivated()), SLOT(handleDeactivated()));
}

void SysInfoWatchlet::handleActivated()
{
	updateInformation();
	connect(_devInfo, SIGNAL(batteryLevelChanged(int)), this, SLOT(updateInformation()));
	connect(_netMgr, SIGNAL(onlineStateChanged(bool)), this, SLOT(updateInformation()));
}

void SysInfoWatchlet::handleDeactivated()
{
	disconnect(_devInfo, SIGNAL(batteryLevelChanged(int)), this, SLOT(updateInformation()));
	disconnect(_netMgr, SIGNAL(onlineStateChanged(bool)), this, SLOT(updateInformation()));
}

void SysInfoWatchlet::updateInformation()
{
	QList<QNetworkConfiguration> cfgs = _netMgr->allConfigurations(QNetworkConfiguration::Active);
	int batteryLevel = _devInfo->batteryLevel();
	qDebug() << "Updating system information (batteryLevel =" << batteryLevel << "%)";
	context()->setContextProperty("batteryLevel", batteryLevel);
	if (cfgs.size() > 0) {
		context()->setContextProperty("networkName", cfgs[0].name());
	} else {
		context()->setContextProperty("networkName", "-");
	}
}
