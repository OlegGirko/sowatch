#include "notificationswatchlet.h"

using namespace sowatch;

NotificationsWatchlet::NotificationsWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.notifications")
{
	setSource(QUrl(SOWATCH_QML_DIR "/notificationswatchlet/" + server->watch()->model() + ".qml"));
}

