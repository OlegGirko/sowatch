#include "notification.h"

using namespace sowatch;

Notification::Notification(QObject *parent)
	: QObject(parent)
{
}

Notification::~Notification()
{
}

QImage Notification::image() const
{
	return QImage();
}
