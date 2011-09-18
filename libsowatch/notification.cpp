#include "notification.h"

using namespace sowatch;

Notification::Notification(Type type, const QDateTime& dateTime, QString title, QString body)
	: _type(type), _dateTime(dateTime), _title(title), _body(body)
{
}

Notification::~Notification()
{

}
