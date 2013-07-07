#include "liveviewnotificationwatchlet.h"

using namespace sowatch;

const QLatin1String LiveViewNotificationWatchlet::myId("com.javispedro.sowatch.liveview.notification");

LiveViewNotificationWatchlet::LiveViewNotificationWatchlet(Watch *watch) :
    DeclarativeWatchlet(watch, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/liveviewwatchlets/" + watch->model() + "-notification.qml"));
}
