#include <QtEndian>

#include "liveviewpaintengine.h"
#include "liveview.h"

using namespace sowatch;
QTM_USE_NAMESPACE

#define PROTOCOL_DEBUG 1

LiveView::LiveView(ConfigKey* settings, QObject* parent) :
	BluetoothWatch(QBluetoothAddress(settings->value("address").toString()), parent),
	_settings(settings->getSubkey(QString(), this)),
    _watchlets(0),
    _24hMode(settings->value("24h-mode", false).toBool()),
    _screenWidth(0), _screenHeight(0),
    _mode(RootMenuMode),
    _paintEngine(0),
    _rootMenuFirstWatchlet(0),
    _sendTimer(new QTimer(this))
{
	_sendTimer->setInterval(DelayBetweenMessages);
	connect(_sendTimer, SIGNAL(timeout()), SLOT(handleSendTimerTick()));

	_buttons << "Select" << "Up" << "Down" << "Left" << "Right";
}

LiveView::~LiveView()
{
	if (_paintEngine) {
		delete _paintEngine;
	}
}

QPaintEngine* LiveView::paintEngine() const
{
	if (!_paintEngine) {
		qDebug() << "Constructing paint engine";
		_paintEngine = new LiveViewPaintEngine;
	}

	return _paintEngine;
}

int LiveView::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return _screenWidth;
	case PdmHeight:
		return _screenHeight;
	case PdmWidthMM:
		return 24;
	case PdmHeightMM:
		return 24;
	case PdmNumColors:
		return 256;
	case PdmDepth:
		return 8;
	case PdmDpiX:
	case PdmPhysicalDpiX:
		return 136;
	case PdmDpiY:
	case PdmPhysicalDpiY:
		return 136;
	}

	return -1;
}

QString LiveView::model() const
{
	return "liveview";
}

QStringList LiveView::buttons() const
{
	return _buttons;
}

bool LiveView::busy() const
{
	return !_connected ||
			_socket->state() != QBluetoothSocket::ConnectedState ||
			_sendingMsgs.size() > 20;
}

void LiveView::setDateTime(const QDateTime& dateTime)
{
	// It seems LiveView _requests_ the current date rather than expecting
	// the phone to be sending it.
	// Wonder what will happen during DST changes?
	// Do nothing here.
	Q_UNUSED(dateTime);
}

void LiveView::queryDateTime()
{
	// LiveView does not support this.
}

QDateTime LiveView::dateTime() const
{
	return QDateTime::currentDateTime();
}

void LiveView::queryBatteryLevel()
{
	// LiveView does not seem to support this.
}

int LiveView::batteryLevel() const
{
	return 100;
}

void LiveView::queryCharging()
{
	// LiveView does not seem to support this.
}

bool LiveView::charging() const
{
	return false;
}

void LiveView::displayIdleScreen()
{
	if (_mode != RootMenuMode) {
		_mode = RootMenuMode;
		refreshMenu();
	}
}

void LiveView::displayNotification(Notification *notification)
{

}

void LiveView::displayApplication()
{
	_mode = ApplicationMode;
	setMenuSize(0); // TODO
}

void LiveView::vibrate(int msecs)
{
	// TODO
}

void LiveView::setWatchletsModel(WatchletsModel *model)
{
	if (_watchlets) {
		disconnect(_watchlets, 0, this, 0);
	}
	_watchlets = model;
	if (_watchlets) {
		connect(_watchlets, SIGNAL(modelChanged()), SLOT(handleWatchletsChanged()));
		handleWatchletsChanged();
	}
}

QImage* LiveView::image()
{
	return &_image;
}

void LiveView::renderImage(int x, int y, const QImage &image)
{
	qDebug() << "Rendering image at" << x << 'x' << y << "of size" << image.size();
	QByteArray data = encodeImage(image.copy(0,0,64,64));
	if (!data.isEmpty()) {
		displayBitmap(x, y, data);
	}
}

void LiveView::clear()
{
	displayClear();
}

void LiveView::setupBluetoothWatch()
{
	_mode = RootMenuMode;

	connect(_socket, SIGNAL(readyRead()), SLOT(handleDataReceived()));
	updateDisplayProperties();
	refreshMenu();
}

void LiveView::desetupBluetoothWatch()
{
	_sendTimer->stop();
	_sendingMsgs.clear();
}

void LiveView::recreateWatchletsMenu()
{
	// Erase all current watchlets frm the menu
	QList<RootMenuItem>::iterator it = _rootMenu.begin();
	it += _rootMenuFirstWatchlet;
	_rootMenu.erase(it, _rootMenu.end());

	if (_watchlets) {
		const int num_watchlets = _watchlets->size();
		for (int i = 0; i < num_watchlets; i++) {
			RootMenuItem item;
			QModelIndex index = _watchlets->index(i);
			item.type = MenuOther;
			item.title = _watchlets->data(index, WatchletsModel::TitleRole).toString();
			item.icon = encodeImage(_watchlets->data(index, WatchletsModel::IconRole).toUrl());
			item.unread = 0;
			item.watchletId = _watchlets->at(i)->id();
			_rootMenu.append(item);
		}
	}
}

void LiveView::refreshMenu()
{
	if (_mode == RootMenuMode) {
		setMenuSize(_rootMenu.size());
	}
}

QByteArray LiveView::encodeImage(const QImage& image) const
{
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);
	if (image.save(&buffer, "PNG", 0)) {
		return buffer.buffer();
	} else {
		qWarning() << "Failed to encode image";
		return QByteArray();
	}
}

QByteArray LiveView::encodeImage(const QUrl& url) const
{
	if (url.encodedPath().endsWith(".png")) {
		// Just load the image
		QFile f(url.toLocalFile());
		if (f.open(QIODevice::ReadOnly)) {
			qDebug() << "Encoding local PNG" << url.toLocalFile();
			return f.readAll();
		} else {
			qWarning() << "Could not read image:" << url.toString();
			return QByteArray();
		}
	} else {
		qDebug() << "Encoding local nonPNG" << url.toLocalFile();
		return encodeImage(QImage(url.toLocalFile()));
	}
}

void LiveView::send(const Message &msg)
{
	Q_ASSERT(_connected);
	_sendingMsgs.enqueue(msg);
	if (!_sendTimer->isActive()) {
		_sendTimer->start();
	}
}

void LiveView::sendResponse(MessageType type, ResponseType response)
{
	send(Message(type, QByteArray(1, static_cast<char>(response))));
}

void LiveView::updateDisplayProperties()
{
	static const char *software_version = "0.0.3";

	send(Message(GetDisplayProperties,
	             QByteArray(software_version, strlen(software_version) + 1)));
}

void LiveView::updateSoftwareVersion()
{
	send(Message(GetSoftwareVersion, QByteArray(1, 0)));
}

void LiveView::displayBitmap(unsigned char x, unsigned char y, const QByteArray &image)
{
	QByteArray data(3, 0);
	data[0] = x;
	data[1] = y;
	data[2] = 1; // TODO Figure out what this is. Maybe format?
	data.append(image);
	qDebug() << "Trying to send" << image.size() << "bytes";
	send(Message(DisplayBitmap, data));
}

void LiveView::displayClear()
{
	send(Message(DisplayClear));
}

void LiveView::setMenuSize(unsigned char size)
{
	qDebug() << "Set menu size to" << size;
	send(Message(SetMenuSize, QByteArray(1, size)));
}

void LiveView::sendMenuItem(unsigned char id, MenuItemType type, unsigned short unread, const QString& text, const QByteArray& image)
{
	QByteArray data(1 + 2 * 3 + 2 + 2 * 3, 0);
	data[0] = type;
	//data[1,2] // Unknown
	data[3] = (unread & 0xFF00U) >> 8;
	data[4] = (unread & 0x00FFU);
	//data[5,6] // Unknown
	data[7] = id + 3;
	//data[8] // Unknown
	//data[9,10] // Unknown
	//data[11,12] // Unknown
	quint16 text_length = text.length();
	data[13] = (text_length & 0xFF00U) >> 8;
	data[14] = (text_length & 0x00FFU);
	data.append(text.toLatin1());
	data.append(image);

	send(Message(MenuItemResponse, data));
}

void LiveView::enableLed(const QColor& color, unsigned short delay, unsigned short time)
{
	QByteArray data;

	quint16 rgb = 0;
	rgb |= ((color.red() & 0xF8) << 8);
	rgb |= ((color.green() & 0xFC) << 3);
	rgb |= ((color.blue() & 0xF8) >> 3);

	data.append((rgb & 0xFF00U) >> 8);
	data.append(rgb & 0x00FFU);
	data.append((delay & 0xFF00U) >> 8);
	data.append(delay & 0x00FFU);
	data.append((time & 0xFF00U) >> 8);
	data.append(time & 0x00FFU);

	send(Message(EnableLed, data));
}

void LiveView::handleMessage(const Message &msg)
{
	send(Message(Ack, QByteArray(1, msg.type)));
	switch (msg.type) {
	case DeviceStatusChange:
		handleDeviceStatusChange(msg);
		break;
	case DisplayClearResponse:
	case DisplayBitmapResponse:
		// Nothing to do
		break;
	case MenuItemRequest:
		handleMenuItemRequest(msg);
		break;
	case NotificationRequest:
		handleNotificationRequest(msg);
		break;
	case Navigation:
		handleNavigation(msg);
		break;
	case MenuItemsRequest:
		handleMenuItemsRequest(msg);
		break;
	case DateTimeRequest:
		handleDateTimeRequest(msg);
		break;
	case EnableLedResponse:
		// Nothing to do
		break;
	case GetDisplayPropertiesResponse:
		handleDisplayProperties(msg);
		break;
	case GetSoftwareVersionResponse:
		handleSoftwareVersion(msg);
		break;
	default:
		qWarning() << "Received unknown LiveView message" << msg.type;
	}
}

void LiveView::handleDeviceStatusChange(const Message &msg)
{
	if (msg.data.size() == 1) {
		DeviceStatus status = static_cast<DeviceStatus>(msg.data.at(0));
		qDebug() << "liveview device status change" << status;
	}
	sendResponse(DeviceStatusChangeResponse, ResponseOk);
}

void LiveView::handleMenuItemRequest(const Message &msg)
{
	Q_UNUSED(msg);
	// TODO
	qWarning() << "TODO" << Q_FUNC_INFO;
}

void LiveView::handleNotificationRequest(const Message &msg)
{
	// TODO
	sendResponse(NotificationResponse, ResponseError); // TODO Crashes the watch
}

void LiveView::handleNavigation(const Message &msg)
{
	if (msg.data.size() < 5) {
		// Packet too small
		sendResponse(NavigationResponse, ResponseError);
		return;
	}

	int menu_id = static_cast<unsigned char>(msg.data[4]);
	int item_id = static_cast<unsigned char>(msg.data[3]);
	int event = static_cast<unsigned char>(msg.data[2]);
	qDebug() << "navigation" << event << item_id << menu_id;

	switch (event) {
	case UpPress:
		emit buttonPressed(1);
		break;
	case DownPress:
		emit buttonPressed(2);
		break;
	case LeftPress:
		emit buttonPressed(3);
		break;
	case RightPress:
		emit buttonPressed(4);
		break;
	case SelectPress:
		emit buttonPressed(0);
		break;
	case SelectLongPress:
		if (_mode == ApplicationMode) {
			sendResponse(NavigationResponse, ResponseCancel);
			emit closeWatchledRequested();
			return;
		}
		break;
	case SelectMenu:
		if (item_id <= _rootMenu.size()) {
			sendResponse(NavigationResponse, ResponseError);

			switch(_rootMenu[item_id].type) {
			case MenuNotificationList:
				Q_ASSERT(false); // TODO
				break;
			case MenuOther:
				emit watchletRequested(_rootMenu[item_id].watchletId);
				break;
			}

		} else {
			sendResponse(NavigationResponse, ResponseError);
		}

		return;
	}

	// Fallback case
	sendResponse(NavigationResponse, ResponseOk);
}

void LiveView::handleMenuItemsRequest(const Message &msg)
{
	qDebug() << "Sending menu items";
	if (_mode == RootMenuMode) {
		for (int i = 0; i < _rootMenu.size(); i++) {
			qDebug() << "Sending one menu item";
			sendMenuItem(i, _rootMenu[i].type, _rootMenu[i].unread,
			                _rootMenu[i].title, _rootMenu[i].icon);
		}
	}
}

void LiveView::handleDateTimeRequest(const Message &msg)
{
	QByteArray data(5, 0);

	Q_UNUSED(msg);

	QDateTime time = QDateTime::currentDateTime();
	time.setTimeSpec(Qt::UTC);
	quint32 timestamp = time.toTime_t();
	data[0] = (timestamp & 0xFF000000U) >> 24;
	data[1] = (timestamp & 0x00FF0000U) >> 16;
	data[2] = (timestamp & 0x0000FF00U) >>  8;
	data[3] = (timestamp & 0x000000FFU);
	data[4] = _24hMode ? 1 : 0;

	send(Message(DateTimeResponse, data));
}

void LiveView::handleDisplayProperties(const Message &msg)
{
	if (msg.data.size() < 2) {
		qWarning() << "Invalid DPR response";
		return;
	}

	_screenWidth = static_cast<unsigned char>(msg.data[0]);
	_screenHeight = static_cast<unsigned char>(msg.data[1]);

	qDebug() << "LiveView display properties:" << _screenWidth << 'x' << _screenHeight;

	// Recreate the display image
	_image = QImage(_screenWidth, _screenHeight, QImage::Format_RGB16);

	// For some reason firmware expects us to send this message right
	// after display properties
	// Otherwise the watch hangs up the connection
	updateSoftwareVersion();
	Q_UNUSED(msg);
}

void LiveView::handleSoftwareVersion(const Message &msg)
{
	qDebug() << "LiveView software version is"
	         << QString::fromAscii(msg.data.constData(), msg.data.size());
}

void LiveView::handleDataReceived()
{
#pragma pack(push)
#pragma pack(1)
	static const int HEADER_SIZE = 6;
	union header_t {
		char c[HEADER_SIZE];
		struct header_fields_t {
			quint8 msg_type;
			quint8 header_len;
			quint32 data_len;
		} h;
	} header;
#pragma pack(pop)

	Q_ASSERT(sizeof(header) == HEADER_SIZE);

	do {
		qint64 dataRead;

		qDebug() << "received" << _socket->bytesAvailable() << "bytes";

		if (_receivingMsg.type == NoMessage) {
			/* Still not received even the packet type */
			/* Receive the full header. */
			if (_socket->bytesAvailable() < HEADER_SIZE) {
				/* Still not enough data available. */
				return; /* Wait for more, if non blocking. */
			}

			dataRead = _socket->read(header.c, HEADER_SIZE);
			if (dataRead < HEADER_SIZE) {
				qWarning() << "Short read";
				return;
			}

			_receivingMsg.type = static_cast<MessageType>(header.h.msg_type);
			if (header.h.header_len != HEADER_SIZE - 2) {
				qWarning() << "Unexpected header length:" << header.h.header_len;
			}

			unsigned long data_size = qFromBigEndian(header.h.data_len);
			if (data_size > 1048576) {
				// If input packet is > 1 MiB, consider a protocol error.
				qWarning() << "Too large data size: " << data_size;
				data_size = 0;
			}
			_receivingMsg.data.resize(data_size);

			qDebug() << "got header (type=" << _receivingMsg.type <<
			            "size=" << data_size << ")";
		}

		/* We have the header; now, try to get the complete packet. */
		if (_socket->bytesAvailable() < _receivingMsg.data.size()) {
			return; /* Wait for more. */
		}

		dataRead = _socket->read(_receivingMsg.data.data(), _receivingMsg.data.size());
		if (dataRead < _receivingMsg.data.size()) {
			qWarning() << "Short read";
			return;
		}

#if PROTOCOL_DEBUG
		qDebug() << "received" << _receivingMsg.type << _receivingMsg.data.toHex();
#endif
		handleMessage(_receivingMsg);

		// Prepare for the next packet
		_receivingMsg.data.clear();
		_receivingMsg.type = NoMessage;
	} while (_socket->bytesAvailable() > 0);
}

void LiveView::handleSendTimerTick()
{
	static const int HEADER_SIZE = 6;

	// If there are packets to be sent...
	if (!_sendingMsgs.empty()) {
		// Send a message to the watch
		Message msg = _sendingMsgs.dequeue();
		const quint32 data_size = msg.data.size();
		QByteArray packet;

		Q_ASSERT(_connected && _socket);

		packet.resize(HEADER_SIZE + data_size);
		packet[0] = msg.type;
		packet[1] = HEADER_SIZE - 2;
		packet[2] = (data_size & 0xFF000000U) >> 24;
		packet[3] = (data_size & 0x00FF0000U) >> 16;
		packet[4] = (data_size & 0x0000FF00U) >>  8;
		packet[5] = (data_size & 0x000000FFU);
		packet.replace(HEADER_SIZE, data_size, msg.data);

#if PROTOCOL_DEBUG
		if (packet.size() < 20) {
			qDebug() << "sending" << packet.toHex();
		} else {
			qDebug() << "sending" << packet.left(20).toHex() << "...";
		}
#endif

		_socket->write(packet);
	}
	// If we just finished sending all packets...
	if (_sendingMsgs.empty()) {
		// Stop the send timer to save battery
		_sendTimer->stop();
	}
}

void LiveView::handleWatchletsChanged()
{
	recreateWatchletsMenu();
	if (_connected) {
		refreshMenu();
	}
}
