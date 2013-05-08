#ifndef BLUETOOTHWATCHSCANNER_H
#define BLUETOOTHWATCHSCANNER_H

#include <sowatch.h>
#include <QtConnectivity/QBluetoothServiceDiscoveryAgent>

#include "sowatchbt_global.h"

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QBluetoothServiceDiscoveryAgent);
using QTM_PREPEND_NAMESPACE(QBluetoothServiceInfo);
using QTM_PREPEND_NAMESPACE(QBluetoothUuid);

class SOWATCHBT_EXPORT BluetoothWatchScanner : public WatchScanner
{
	Q_OBJECT

public:
	BluetoothWatchScanner(QObject *parent);

	void start();

protected:
	void setUuidFilter(const QBluetoothUuid & uuid);

protected slots:
	virtual void handleDiscoveredService(const QBluetoothServiceInfo& info) = 0;

private:
	QBluetoothServiceDiscoveryAgent *_agent;
};

}

#endif // BLUETOOTHWATCHSCANNER_H
