#ifndef METAWATCHSCANNER_H
#define METAWATCHSCANNER_H

#include <sowatch.h>
#include <QtConnectivity/QBluetoothServiceDiscoveryAgent>

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QBluetoothServiceDiscoveryAgent);
using QTM_PREPEND_NAMESPACE(QBluetoothServiceInfo);

class MetaWatchScanner : public WatchScanner
{
	Q_OBJECT
public:
	explicit MetaWatchScanner(QObject *parent = 0);

	void start();

private:
	QBluetoothServiceDiscoveryAgent *_agent;

private slots:
	void handleDiscoveredService(const QBluetoothServiceInfo& info);
};

}

#endif // METAWATCHSCANNER_H
