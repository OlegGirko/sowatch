#include "qmapwatchlet.h"

using namespace sowatch;

QMapWatchlet::QMapWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.qmap")
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/" + server->watch()->model() + ".qml"));
}

