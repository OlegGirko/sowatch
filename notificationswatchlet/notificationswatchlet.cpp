#include "notificationswatchlet.h"

using namespace sowatch;

NotificationsWatchlet::NotificationsWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, "com.javispedro.sowatch.notifications")
{
	setSource(QUrl(SOWATCH_QML_DIR "/notificationswatchlet/" + watch->model() + ".qml"));
}

