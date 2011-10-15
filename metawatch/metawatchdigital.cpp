#include "metawatchdigital.h"

using namespace sowatch;

MetaWatchDigital::MetaWatchDigital(const QBluetoothAddress& address, QSettings* settings, QObject *parent) :
	MetaWatch(address, settings, parent),
	_nMails(0), _nCalls(0), _nIms(0), _nSms(0), _nMms(0)
{
	QImage baseImage(screenWidth, screenHeight, QImage::Format_MonoLSB);
	baseImage.setColor(0, QColor(Qt::white).rgb());
	baseImage.setColor(1, QColor(Qt::black).rgb());
	_image[IdleMode] = baseImage;
	_image[ApplicationMode] = baseImage;
	_image[NotificationMode] = baseImage;
}

int MetaWatchDigital::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return screenWidth;
	case PdmHeight:
		return screenHeight;
	case PdmWidthMM:
		return 24;
	case PdmHeightMM:
		return 24;
	case PdmNumColors:
		return 2;
	case PdmDepth:
		return 1;
	case PdmDpiX:
	case PdmPhysicalDpiX:
		return 96;
	case PdmDpiY:
	case PdmPhysicalDpiY:
		return 96;
	}

	return -1;
}

QString MetaWatchDigital::model() const
{
	return "metawatch-digital";
}

void MetaWatchDigital::updateNotificationCount(Notification::Type type, int count)
{
	switch (type) {
	case Notification::MissedCallNotification:
		_nCalls = count;
		break;
	case Notification::EmailNotification:
		_nMails = count;
		break;
	case Notification::ImNotification:
		_nIms = count;
		break;
	case Notification::SmsNotification:
		_nSms = count;
		break;
	case Notification::MmsNotification:
		_nMms = count;
		break;
	default:
		return; // Since this notification won't show up in idle screen, we do not redraw.
		break;
	}

	if (isConnected()) {
		renderIdleCounts();
	}
}

void MetaWatchDigital::displayIdleScreen()
{
	qDebug() << "displaying idle screen";
	MetaWatch::displayIdleScreen();

	// Usually, idle screen is kept updated, so we can flip it right away.
	updateLcdDisplay(IdleMode);
}

void MetaWatchDigital::displayNotification(Notification *n)
{
	qDebug() << "display notification" << n->title() << n->body();

	// Render the notification and display it before invoking haptic feedback
	_currentMode = NotificationMode;
	renderNotification(n);

	MetaWatch::displayNotification(n);
}

void MetaWatchDigital::displayApplication()
{
	qDebug() << "entering application mode";

	MetaWatch::displayApplication();
}

void MetaWatchDigital::update(Mode mode, const QList<QRect> &rects)
{
	if (!_connected) return;
	const QRect clipRect(0, 0, screenWidth, screenHeight);
	QVector<bool> lines(screenHeight, false);

	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(clipRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			lines[i] = true;
		}
	}

	updateLcdLines(mode, _image[mode], lines);
	if (mode == _currentMode) {
		updateLcdDisplay(mode);
	}
}

void MetaWatchDigital::clear(Mode mode, bool black)
{
	if (!_connected) return;
	loadLcdTemplate(mode, black ? 1 : 0);
}

void MetaWatchDigital::renderIdleScreen()
{
	QImage idle_call(QString(":/metawatch/graphics/idle_call.bmp"));
	QImage idle_sms(QString(":/metawatch/graphics/idle_sms.bmp"));
	QImage idle_mail(QString(":/metawatch/graphics/idle_gmail.bmp"));
	QPainter p;

	_paintMode = IdleMode;
	p.begin(this);

	p.fillRect(0, 0, screenWidth, screenHeight, Qt::white);

	p.setPen(QPen(Qt::black, 1.0, Qt::DashLine));
	p.drawLine(1, systemAreaHeight + 2, screenWidth - 2, systemAreaHeight + 2);
	p.drawLine(1, systemAreaHeight * 2 + 4, screenWidth - 2, systemAreaHeight * 2 + 4);

	p.drawImage((32 * 0) + 4, systemAreaHeight * 2 + 7, idle_call);
	p.drawImage((32 * 1) + 4, systemAreaHeight * 2 + 7, idle_sms);
	p.drawImage((32 * 2) + 4, systemAreaHeight * 2 + 7, idle_mail);

	p.end();
	_paintMode = _currentMode;

	renderIdleWeather();
	renderIdleCounts();
}

void MetaWatchDigital::renderIdleWeather()
{
	_paintMode = IdleMode;
	QFont f("MetaWatch Small caps 8pt", 6);
	QImage rain(QString(":/metawatch/graphics/weather_rain.bmp"));
	QPainter p(this);

	p.setFont(f);
	p.drawText(30, systemAreaHeight + 14, "No data!");
	p.drawImage(screenWidth - 26, systemAreaHeight + 6, rain);

	_paintMode = _currentMode;
}

void MetaWatchDigital::renderIdleCounts()
{
	_paintMode = IdleMode;
	QFont f("MetaWatch Large caps 8pt");
	QString s;
	QPainter p(this);
	QTextOption opt(Qt::AlignCenter);
	const int y = systemAreaHeight * 2 + 26;
	const int w = 24;
	const int h = screenHeight - (y + 1);
	const int mails = _nMails;
	const int calls = _nCalls;
	const int sms = _nSms + _nIms;

	qDebug() << "unread counts" << calls << sms << mails;

	f.setPixelSize(8); // Seems to be the only way to get the desired size.

	p.setFont(f);
	p.fillRect(QRect(0, y, screenWidth, h), Qt::white);
	p.drawText(QRect((32 * 0) + 4, y, w, h), s.sprintf("%d", calls), opt);
	p.drawText(QRect((32 * 1) + 4, y, w, h), s.sprintf("%d", sms), opt);
	p.drawText(QRect((32 * 2) + 4, y, w, h), s.sprintf("%d", mails), opt);

	_paintMode = _currentMode;
}

void MetaWatchDigital::renderNotification(Notification *n)
{
	_paintMode = NotificationMode;
	QPainter p;
	QFont sf("MetaWatch Small caps 8pt");
	QFont lf("MetaWatch Large 16pt");
	QFont mf("MetaWatch Large 16pt");
	QImage icon = iconForNotification(n);

	sf.setPixelSize(8);
	mf.setPixelSize(14);
	lf.setPixelSize(16);

	const int iconW = icon.width(), iconH = icon.height();
	const int margin = 4;
	const int x = margin;
	const int iconY = margin;
	const int titleY = margin*2 + iconH;
	const int dateX = x + iconW + margin;
	int textFlags;
	QString text;

	p.begin(this);

	p.fillRect(0, 0, screenWidth, screenHeight, Qt::white);
	p.drawImage(x, iconY, icon);

	p.setPen(Qt::black);

	p.setFont(sf);
	textFlags = Qt::AlignRight | Qt::AlignVCenter | Qt::TextWordWrap;
	text = n->displayTime();
	QRect dateRect(dateX, iconY, (screenWidth - dateX) - margin, iconH);
	p.drawText(dateRect, textFlags, text);

	p.setFont(lf);
	textFlags = Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
	text = n->title();

	QRect titleMaxRect(x, titleY, screenWidth - x*2, screenHeight - titleY);
	QRect titleRect = p.boundingRect(titleMaxRect, textFlags, text);
	if (titleRect.width() > titleMaxRect.width()) {
		textFlags = Qt::AlignLeft | Qt::AlignTop | Qt::TextWrapAnywhere;
		titleRect = p.boundingRect(titleMaxRect, textFlags, text);
	}

	p.drawText(titleMaxRect, textFlags, text);

	p.setFont(mf);
	textFlags = Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
	text = n->body();

	int bodyY = titleRect.y() + titleRect.height();
	if (bodyY >= screenHeight) return;

	QRect bodyMaxRect(x, bodyY, titleMaxRect.width(), screenHeight - bodyY);
	QRect bodyRect = p.boundingRect(bodyMaxRect, textFlags, text);
	if (bodyRect.width() > bodyMaxRect.width()) {
		textFlags = Qt::AlignLeft | Qt::AlignTop | Qt::TextWrapAnywhere;
	}

	p.drawText(bodyMaxRect, textFlags, text);

	p.end();
	_paintMode = _currentMode;
}

QImage MetaWatchDigital::iconForNotification(const Notification *n)
{
	switch (n->type()) {
	case Notification::CallNotification:
	case Notification::MissedCallNotification:
		return QImage(QString(":/metawatch/graphics/phone.bmp"));
		break;
	case Notification::SmsNotification:
	case Notification::MmsNotification:
	case Notification::ImNotification:
		return QImage(QString(":/metawatch/graphics/message.bmp"));
		break;
	case Notification::EmailNotification:
		return QImage(QString(":/metawatch/graphics/email.bmp"));
		break;
	case Notification::CalendarNotification:
		return QImage(QString(":/metawatch/graphics/timer.bmp"));
		break;
	default:
		return QImage();
	}
}

void MetaWatchDigital::handleWatchConnected()
{
	// Configure to show watch-rendered clock in idle screen
	configureLcdIdleSystemArea(false);

	// Render the idle screen assuming previous contents were lost
	renderIdleScreen();
}
