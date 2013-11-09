#ifndef METAWATCHSCANNER_H
#define METAWATCHSCANNER_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class MetaWatchScanner : public BluetoothWatchScanner
{
	Q_OBJECT
public:
	explicit MetaWatchScanner(QObject *parent = 0);

private slots:
	void handleDiscoveredService(const QBluetoothServiceInfo& info);
};

}

#endif // METAWATCHSCANNER_H
