#include "metawatchnotificationwatchlet.h"

using namespace sowatch;

const QLatin1String MetaWatchNotificationWatchlet::myId("com.javispedro.sowatch.metawatch.notification");

MetaWatchNotificationWatchlet::MetaWatchNotificationWatchlet(Watch *watch) :
    DeclarativeWatchlet(watch, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/metawatchwatchlets/" + watch->model() + "-notification.qml"));
}
