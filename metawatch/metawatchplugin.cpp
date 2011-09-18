#include <QtConnectivity/QBluetoothAddress>
#include "metawatch.h"
#include "metawatchsimulator.h"
#include "metawatchplugin.h"

using namespace sowatch;
QTM_USE_NAMESPACE

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

Watch* MetaWatchPlugin::getWatch(const QString& driver, QSettings& settings, QObject *parent)
{
	if (driver == "metawatch-digital") {
		QBluetoothAddress address(settings.value("address").toString());
		return new MetaWatch(address, parent);
	} else if (driver == "metawatch-digital-sim") {
		//return new MetaWatchSimulator(parent);
		return 0;
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(metawatch, MetaWatchPlugin)
