#include "qmapwatchlet.h"

using namespace sowatch;

const QLatin1String QMapWatchlet::myId("com.javispedro.sowatch.qmap");

QMapWatchlet::QMapWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, myId)
{
	setFullUpdateMode(true);
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/map-" + watch->model() + ".qml"));
}
