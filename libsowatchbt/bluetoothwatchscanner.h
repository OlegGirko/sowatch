#ifndef BLUETOOTHWATCHSCANNER_H
#define BLUETOOTHWATCHSCANNER_H

#include <sowatch.h>
#include <QBluetoothServiceDiscoveryAgent>

#include "sowatchbt_global.h"

namespace sowatch
{

class SOWATCHBT_EXPORT BluetoothWatchScanner : public WatchScanner
{
	Q_OBJECT

public:
	BluetoothWatchScanner(QObject *parent);
	~BluetoothWatchScanner();

	void start();

protected slots:
	virtual void handleDiscoveredService(const QBluetoothServiceInfo& info) = 0;

private:
	static int _instances;
	static QBluetoothServiceDiscoveryAgent *_agent;
};

}

#endif // BLUETOOTHWATCHSCANNER_H
