#include "compasswatchlet.h"

using namespace sowatch;

const QLatin1String CompassWatchlet::myId("com.javispedro.sowatch.compass");

CompassWatchlet::CompassWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/compass-" + watch->model() + ".qml"));
}
