#include "qmapwatchlet.h"

using namespace sowatch;

QMapWatchlet::QMapWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.map")
{

	setSource(QUrl("qrc:/qmapwatchlet/" + server->watch()->model() + ".qml"));
}

