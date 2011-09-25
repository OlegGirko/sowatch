#include "sysinfowatchlet.h"

using namespace sowatch;
QTM_USE_NAMESPACE

SysInfoWatchlet::SysInfoWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.sysinfo"),
	_devInfo(new QSystemDeviceInfo(this)),
	_netMgr(new QNetworkConfigurationManager(this))
{
	rootContext()->setContextProperty("batteryLevel", 0);
	rootContext()->setContextProperty("networkName", "");
	setSource(QUrl("qrc:/sysinfowatchlet/" + server->watch()->model() + ".qml"));
	connect(this, SIGNAL(activated()), SLOT(handleActivated()));
}

void SysInfoWatchlet::handleActivated()
{
	QList<QNetworkConfiguration> cfgs = _netMgr->allConfigurations(QNetworkConfiguration::Active);
	rootContext()->setContextProperty("batteryLevel", _devInfo->batteryLevel());
	if (cfgs.size() > 0) {
		rootContext()->setContextProperty("networkName", cfgs[0].name());
	} else {
		rootContext()->setContextProperty("networkName", "");
	}
}
