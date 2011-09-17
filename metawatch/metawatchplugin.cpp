#include "metawatch.h"
#include "metawatchsimulator.h"
#include "metawatchplugin.h"

using namespace sowatch;

MetaWatchPlugin::~MetaWatchPlugin()
{

}

QStringList MetaWatchPlugin::drivers()
{
	QStringList d;
	d << "metawatch-digital";
	d << "metawatch-digital-sim";
	return d;
}

Watch* MetaWatchPlugin::getWatch(const QString& driver, const QString& connId, QObject *parent)
{
	if (driver == "metawatch-digital") {
		return new MetaWatch(QBluetoothAddress(connId), parent);
	} else if (driver == "metawatch-digital-sim") {
		return new MetaWatchSimulator(parent);
	} else {
		return 0;
	}
}
