#include "notificationswatchlet.h"

using namespace sowatch;

NotificationsWatchlet::NotificationsWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.notifications")
{
	setSource(QUrl("qrc:/notificationswatchlet/" + server->watch()->model() + ".qml"));
}

