#include <QtGui/QFontDatabase>
#include <QtConnectivity/QBluetoothAddress>
#include "metawatchanalog.h"
#include "metawatchdigital.h"
#include "metawatchdigitalsimulator.h"
#include "metawatchscanner.h"
#include "metawatchplugin.h"

using namespace sowatch;
QTM_USE_NAMESPACE

bool MetaWatchPlugin::fontsLoaded = false;

MetaWatchPlugin::MetaWatchPlugin()
{
	if (!fontsLoaded) {
		QFontDatabase::addApplicationFont(SOWATCH_RESOURCES_DIR "/metawatch/fonts/metawatch_16pt_11pxl.ttf");
		QFontDatabase::addApplicationFont(SOWATCH_RESOURCES_DIR "/metawatch/fonts/metawatch_8pt_7pxl_CAPS.ttf");
		QFontDatabase::addApplicationFont(SOWATCH_RESOURCES_DIR "/metawatch/fonts/metawatch_8pt_5pxl_CAPS.ttf");
		// "MetaWatch Large 16pt", "MetaWatch Large caps 8pt", "MetaWatch Small caps 8pt"
		fontsLoaded = true;
	}
}

MetaWatchPlugin::~MetaWatchPlugin()
{

}

QStringList MetaWatchPlugin::drivers()
{
	QStringList d;
	d << "metawatch-analog" << "metawatch-digital" << "metawatch-digital-simulator";
	return d;
}

WatchScanner* MetaWatchPlugin::getScanner(QObject *parent)
{
	return new MetaWatchScanner(parent);
}

QUrl MetaWatchPlugin::getConfigQmlUrl(const QString &driver)
{
	if (driver == "metawatch-digital") {
		return QUrl::fromLocalFile(SOWATCH_QML_DIR "/metawatch-digital-config.qml");
	} else {
		return QUrl();
	}
}

Watch* MetaWatchPlugin::getWatch(const QString& driver, ConfigKey* settings, QObject *parent)
{
	if (driver == "metawatch-digital") {
		return new MetaWatchDigital(settings, parent);
	} else if (driver == "metawatch-analog") {
		return new MetaWatchAnalog(settings, parent);
	} else if (driver == "metawatch-digital-simulator") {
		return new MetaWatchDigitalSimulator(settings, parent);
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(metawatch, MetaWatchPlugin)
