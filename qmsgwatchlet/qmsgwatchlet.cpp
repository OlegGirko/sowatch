#include "qmsgwatchlet.h"

using namespace sowatch;

QMsgWatchlet::QMsgWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.qmsg")
{

	setSource(QUrl(SOWATCH_QML_DIR "/qmsgwatchlet/" + server->watch()->model() + ".qml"));
}

