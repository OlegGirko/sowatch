#include <QtCore/QDebug>

#include "qorgwatchlet.h"

using namespace sowatch;

QOrgWatchlet::QOrgWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.qorg")
{
	setSource(QUrl(SOWATCH_QML_DIR "/qorgwatchlet/" + server->watch()->model() + ".qml"));
}
