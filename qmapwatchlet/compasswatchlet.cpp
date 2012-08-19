#include "tripwatchlet.h"

using namespace sowatch;

const QLatin1String TripWatchlet::myId("com.javispedro.sowatch.trip");

TripWatchlet::TripWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmapwatchlet/trip-" + server->watch()->model() + ".qml"));
}
