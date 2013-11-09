#include "notification.h"

using namespace sowatch;

Notification::Notification(QObject *parent)
	: QObject(parent)
{
}

Notification::~Notification()
{
}

Notification::Priority Notification::priority() const
{
	return Normal;
}

QString Notification::displayTime() const
{
	QDateTime dt = dateTime();
	int secsDiff = dt.secsTo(QDateTime::currentDateTime());
	if (secsDiff < 20) {
		return "";
	} else if (secsDiff < 60) {
		return tr("%n second(s) ago", "", secsDiff);
	} else if (secsDiff < 60*60) {
		int n = secsDiff / 60;
		return tr("%n minute(s) ago", "", n);
	} else if (secsDiff < 60*60*24) {
		int n = secsDiff / 3600;
		return tr("%n hour(s) ago", "", n);
	} else {
		return dt.toString(Qt::SystemLocaleShortDate);
	}
}

QImage Notification::image() const
{
	return QImage();
}
