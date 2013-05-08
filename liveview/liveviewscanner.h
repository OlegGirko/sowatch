#ifndef LIVEVIEWSCANNER_H
#define LIVEVIEWSCANNER_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveViewScanner : public BluetoothWatchScanner
{
	Q_OBJECT
public:
	explicit LiveViewScanner(QObject *parent = 0);

	void handleDiscoveredService(const QBluetoothServiceInfo &info);
};

}

#endif // LIVEVIEWSCANNER_H
