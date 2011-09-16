#include <QtCore/QDebug>
#include <QtGui/QPainter>

#include "metawatchsimulator.h"

#define SIMULATE_DAMAGES 1

using namespace sowatch;

MetaWatchSimulator::MetaWatchSimulator(QObject *parent) :
	WatchSimulator(QImage(96, 96, QImage::Format_Mono), parent),
	_screen(96, 96),
	_form(new MetaWatchSimulatorForm)
{
	_form->showNormal();
	connect(_form, SIGNAL(destroyed()), SIGNAL(disconnected()));
	connect(_form, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
	connect(_form, SIGNAL(buttonReleased(int)), SIGNAL(buttonReleased(int)));
}

MetaWatchSimulator::~MetaWatchSimulator()
{
	delete _form;
}

QString MetaWatchSimulator::model() const
{
	return "metawatch-digital";
}

bool MetaWatchSimulator::isConnected() const
{
	return true;
}

bool MetaWatchSimulator::busy() const
{
	return false;
}

void MetaWatchSimulator::update(const QList<QRect> &rects)
{
#if SIMULATE_DAMAGES
	const QRect imageRect = _image.rect();
	QPainter p;
	QVector<bool> rows(96, false);
	unsigned total = 0;

	p.begin(&_screen);
	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(imageRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			rows[i] = true;
		}
		total += r.width() * r.height();

		p.drawImage(r, _image, r);
	}
	p.end();

	_form->refreshScreen(_screen);

	qDebug() << "updated " << total << " pixels " << rows.count(true) << " lines";
#else
	_form->refreshScreen(QPixmap::fromImage(_image));
#endif
}

void MetaWatchSimulator::vibrate(bool on)
{

}
