#include "global.h"
#include "service.h"

using namespace sowatch;

Service::Service(QObject *parent) :
	QObject(parent)
{

}

void Service::terminate()
{
	QCoreApplication::exit(0);
}
