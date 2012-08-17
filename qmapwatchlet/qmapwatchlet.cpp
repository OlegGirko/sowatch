#include "qmapwatchlet.h"

using namespace sowatch;

const QLatin1String QMapWatchlet::myId("com.javispedro.sowatch.qmap");

QMapWatchlet::QMapWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, myId)
{
	setFullUpdateMode(true);
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/" + server->watch()->model() + ".qml"));
}

