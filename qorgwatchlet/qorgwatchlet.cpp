#include <QtCore/QDebug>

#include "qorgwatchlet.h"

using namespace sowatch;

QOrgWatchlet::QOrgWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, "com.javispedro.sowatch.qorg")
{
	setSource(QUrl(SOWATCH_QML_DIR "/qorgwatchlet/" + watch->model() + ".qml"));
}
