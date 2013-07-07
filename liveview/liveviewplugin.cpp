#include <QtConnectivity/QBluetoothAddress>

#include "liveview.h"
#include "liveviewscanner.h"
#include "liveviewplugin.h"

using namespace sowatch;
QTM_USE_NAMESPACE

LiveViewPlugin::LiveViewPlugin()
{

}

LiveViewPlugin::~LiveViewPlugin()
{

}

QStringList LiveViewPlugin::drivers()
{
	QStringList d;
	d << "liveview";
	return d;
}

WatchScanner* LiveViewPlugin::getScanner(QObject *parent)
{
	return new LiveViewScanner(parent);
}

QUrl LiveViewPlugin::getConfigQmlUrl(const QString &driver)
{
	if (driver == "liveview") {
		return QUrl::fromLocalFile(SOWATCH_QML_DIR "/liveview-config.qml");
	} else {
		return QUrl();
	}
}

Watch* LiveViewPlugin::getWatch(const QString& driver, ConfigKey* settings, QObject *parent)
{
	if (driver == "liveview") {
		return new LiveView(settings, parent);
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(liveview, LiveViewPlugin)
