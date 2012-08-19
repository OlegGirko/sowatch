#include "compasswatchlet.h"

using namespace sowatch;

const QLatin1String CompassWatchlet::myId("com.javispedro.sowatch.compass");

CompassWatchlet::CompassWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/compass-" + server->watch()->model() + ".qml"));
}
