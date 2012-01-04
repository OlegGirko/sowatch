#ifndef SOWATCH_TESTDECLARATIVEWATCHLET_H
#define SOWATCH_TESTDECLARATIVEWATCHLET_H

#include <QtSystemInfo/QSystemDeviceInfo>
#include <QtNetwork/QNetworkConfigurationManager>
#include <sowatch.h>

using QTM_PREPEND_NAMESPACE(QSystemDeviceInfo);

namespace sowatch
{

class SysInfoWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit SysInfoWatchlet(WatchServer* server);

private slots:
	void handleActivated();
	void handleDeactivated();
	void updateInformation();

private:
	QSystemDeviceInfo *_devInfo;
	QNetworkConfigurationManager *_netMgr;
};

}

#endif // SOWATCH_TESTDECLARATIVEWATCHLET_H
