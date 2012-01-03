#include "qmapwatchlet.h"

using namespace sowatch;

QMapWatchlet::QMapWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.map")
{

	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/" + server->watch()->model() + ".qml"));
}

