#include <QtEndian>

#include "liveviewpaintengine.h"
#include "liveview.h"

using namespace sowatch;
QTM_USE_NAMESPACE

#define PROTOCOL_DEBUG 0

const int LiveView::MaxBitmapSize = 64;
QMap<LiveView::MessageType, LiveView::MessageType> LiveView::_ackMap;
QList<LiveView::RootMenuNotificationItem> LiveView::_rootNotificationItems;

LiveView::LiveView(ConfigKey* settings, QObject* parent) :
	BluetoothWatch(QBluetoothAddress(settings->value("address").toString()), parent),
	_settings(settings->getSubkey(QString(), this)),
    _watchlets(0), _notifications(0),
    _24hMode(settings->value("24h-mode", false).toBool()),
    _screenWidth(128), _screenHeight(128),
    _mode(RootMenuMode),
    _paintEngine(0),
    _rootMenuFirstWatchlet(0),
    _waitingForAck(NoMessage)
{
	initializeAckMap();
	_buttons << "Select" << "Up" << "Down" << "Left" << "Right";
	initializeRootNotificationItems();
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
			_sendingMsgs.size() > 1;
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
	qDebug() << "LiveView display idle screen (cur mode=" << _mode << ")";
	if (_mode != RootMenuMode) {
		displayClear();
		_mode = RootMenuMode;
		refreshMenu();
	}
}

void LiveView::displayNotification(Notification *notification)
{
	qDebug() << "LiveView display notification" << notification->title();
	_mode = NotificationMode;
	setScreenMode(ScreenMax);
	setMenuSize(0);
	enableLed(Qt::green, 0, 250);
	vibrate(0, 200);
}

void LiveView::displayApplication()
{
	_mode = ApplicationMode;
	setMenuSize(0); // This clears up the menu.
}

void LiveView::vibrate(int msecs)
{
	vibrate(0, msecs);
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

void LiveView::setNotificationsModel(NotificationsModel *model)
{
	if (_notifications) {
		disconnect(_notifications, 0, this, 0);
	}
	_notifications = model;
	if (_notifications) {
		connect(_notifications, SIGNAL(modelChanged()), SLOT(handleNotificationsChanged()));
		handleNotificationsChanged();
	}
}

QImage* LiveView::image()
{
	return &_image;
}

void LiveView::renderImage(int x, int y, const QImage &image)
{
	const QSize image_size = image.size();
	qDebug() << "Rendering image at" << x << 'x' << y << "of size" << image.size();
	QByteArray data;
	if (image.size().isEmpty()) {
		return; // Empty image
	}

	if (image_size.width() > MaxBitmapSize || image_size.height() > MaxBitmapSize) {
		const QRect new_size(0, 0,
		                     qMin(MaxBitmapSize, image_size.width()),
		                     qMin(MaxBitmapSize, image_size.height()));
		data = encodeImage(image.copy(new_size));
	} else {
		data = encodeImage(image);
	}

	Q_ASSERT(!data.isEmpty());

	displayBitmap(x, y, data);
}

void LiveView::clear()
{
	displayClear();
}

void LiveView::initializeAckMap()
{
	if (_ackMap.empty()) {
		_ackMap[GetDisplayProperties] = GetDisplayPropertiesResponse;
		_ackMap[DeviceStatusChange] = DeviceStatusChangeResponse;
		_ackMap[DisplayBitmap] = DisplayBitmapResponse;
		_ackMap[DisplayClear] = DisplayClearResponse;
		//_ackMap[SetMenuSize] = SetMenuSizeResponse; // fw does not send this, for some reason.
		_ackMap[EnableLed] = EnableLedResponse;
		_ackMap[Vibrate] = VibrateResponse;
		_ackMap[SetScreenMode] = SetScreenModeResponse;
	}
}

LiveView::MessageType LiveView::ackForMessage(MessageType type)
{
	QMap<MessageType, MessageType>::const_iterator i = _ackMap.constFind(type);
	if (i != _ackMap.constEnd()) {
		return i.value();
	}
	return NoMessage;
}

void LiveView::initializeRootNotificationItems()
{
	if (_rootNotificationItems.empty()) {
		RootMenuNotificationItem item;
		item.icon = encodeImage(QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/liveview/graphics/menu_missed_calls.png"));
		item.title = tr("Missed calls");
		item.notificationTypes.append(Notification::MissedCallNotification);
		_rootNotificationItems.append(item);

		item.icon = encodeImage(QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/liveview/graphics/menu_notifications.png"));
		item.title = tr("Events");
		// All other notifications
		item.notificationTypes.append(Notification::OtherNotification);
		item.notificationTypes.append(Notification::SmsNotification);
		item.notificationTypes.append(Notification::MmsNotification);
		item.notificationTypes.append(Notification::ImNotification);
		item.notificationTypes.append(Notification::EmailNotification);
		item.notificationTypes.append(Notification::CalendarNotification);
		_rootNotificationItems.append(item);
	}
}

void LiveView::setupBluetoothWatch()
{
	_mode = RootMenuMode;
	_waitingForAck = NoMessage;

	connect(_socket, SIGNAL(readyRead()), SLOT(handleDataReceived()));
	updateDisplayProperties();
	refreshMenu();
}

void LiveView::desetupBluetoothWatch()
{
	_sendingMsgs.clear();
}

void LiveView::recreateNotificationsMenu()
{
	// Erase all current notifications from the menu
	QList<RootMenuItem>::iterator it = _rootMenu.begin();
	it += _rootMenuFirstWatchlet;
	_rootMenu.erase(_rootMenu.begin(), it);

	_rootMenuFirstWatchlet = 0;

	if (_notifications) {
		foreach (const RootMenuNotificationItem& nitem, _rootNotificationItems) {
			int count = 0;
			foreach (Notification::Type type, nitem.notificationTypes) {
				count += _notifications->countByType(type);
			}
			if (count > 0) {
				RootMenuItem item;
				item.type = MenuNotificationList;
				item.title = nitem.title;
				item.icon = nitem.icon;
				item.unread = count;
				item.notificationItem = &nitem;
				_rootMenu.insert(_rootMenuFirstWatchlet, item);
				_rootMenuFirstWatchlet++;
			}
		}
	}
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
			if (item.title.isEmpty()) {
				qWarning() << "Watchlet" << _watchlets->at(i)->id() << "without title";
			}
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

QByteArray LiveView::encodeImage(const QImage& image)
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

QByteArray LiveView::encodeImage(const QUrl& url)
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
	_sendingMsgs.enqueue(msg);
	if (_connected && _waitingForAck == NoMessage) {
		sendMessageFromQueue();
	} else {
#if PROTOCOL_DEBUG
		qDebug() << "Enqueing message while waiting for ack" << _waitingForAck;
#endif
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

void LiveView::sendNotification(unsigned short id, unsigned short unread, unsigned short count, const QString &date, const QString &header, const QString &body, const QByteArray &image)
{
	QByteArray data;
	QDataStream ds(&data, QIODevice::WriteOnly);

	ds << quint8(0); // Unknown
	ds << quint16(count);
	ds << quint16(unread);
	ds << quint16(id);
	ds << quint8(0) << quint8(0); // Unknown

	QByteArray str = date.toLatin1();
	ds << quint16(str.length());
	ds.writeRawData(str.constData(), str.length());

	str = header.toLatin1();
	ds << quint16(str.length());
	ds.writeRawData(str.constData(), str.length());

	str = body.toLatin1();
	ds << quint16(str.length());
	ds.writeRawData(str.constData(), str.length());

	ds << quint8(0) << quint8(0) << quint8(0); // Unknown
	ds << quint16(image.length());
	ds.writeRawData(image.constData(), image.size());

	send(Message(NotificationResponse, data));
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

void LiveView::vibrate(unsigned short delay, unsigned short time)
{
	QByteArray data;

	data.append((delay & 0xFF00U) >> 8);
	data.append(delay & 0x00FFU);
	data.append((time & 0xFF00U) >> 8);
	data.append(time & 0x00FFU);

	send(Message(Vibrate, data));
}

void LiveView::setScreenMode(ScreenBrigthness mode)
{
	qDebug() << "Set screenmode to" << mode;
	send(Message(SetScreenMode, QByteArray(1, mode)));
}

void LiveView::handleMessage(const Message &msg)
{
	send(Message(Ack, QByteArray(1, msg.type)));
	if (msg.type == _waitingForAck) {
#if PROTOCOL_DEBUG
		qDebug() << "Got ack to" << _waitingForAck;
#endif
		_waitingForAck = NoMessage;
		sendMessageFromQueue();
	}
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
	case VibrateResponse:
		// Nothing to do
		break;
	case GetDisplayPropertiesResponse:
		handleDisplayProperties(msg);
		break;
	case SetScreenModeResponse:
		// Nothing to do
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
		switch (status) {
		case DeviceOff:
			if (_mode == NotificationMode) {
				emit idling();
			}
			break;
		case DeviceOn:
			if (_notifications && _notifications->size() > 0) {
				enableLed(Qt::green, 0, 250);
			}
			break;
		case DeviceMenu:
			qDebug() << "Device in menu";
			break;
		}
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
	if (msg.data.size() < 4) {
		// Packet too small
		sendResponse(NotificationResponse, ResponseError);
		return;
	}

	int menu_id = static_cast<unsigned char>(msg.data[0]);
	NotificationAction action = static_cast<NotificationAction>(msg.data[1]);
	unsigned int max_size = static_cast<unsigned char>(msg.data[2]) << 8;
	max_size |= static_cast<unsigned char>(msg.data[3]);
	qDebug() << "notification rq" << action << menu_id << "(" << max_size << ")";

	if (menu_id >= _rootMenu.size()) {
		sendNotification(0, 0, 0, "", "", "", QByteArray());
		return;
	}

	const RootMenuNotificationItem *notification_item = _rootMenu[menu_id].notificationItem;
	if (!notification_item) {
		sendNotification(0, 0, 0, "", "", "", QByteArray());
		return;
	}

	const QList<Notification::Type> &notification_types = notification_item->notificationTypes;
	const int num_notifications = _notifications->size(notification_types);

	if (_mode != NotificationListMode) {
		_mode = NotificationListMode;
		_curNotificationIndex = 0;
	}

	switch (action) {
	case NotificationShowFirst:
		_curNotificationIndex = 0;
		break;
	case NotificationShowLast:
		_curNotificationIndex = num_notifications - 1;
		break;
	case NotificationShowPrev:
		if (_curNotificationIndex > 0) {
			_curNotificationIndex--;
		} else {
			_curNotificationIndex = 0;
		}
		break;
	case NotificationShowNext:
		if (_curNotificationIndex < num_notifications - 1) {
			_curNotificationIndex++;
		} else {
			_curNotificationIndex = num_notifications - 1;
		}
		break;
	case NotificationShowCurrent:
		if (_curNotificationIndex >= num_notifications - 1 ||
		        _curNotificationIndex < 0) {
			_curNotificationIndex = 0;
		}
		break;
	}

	const Notification *notification = _notifications->at(notification_types,
	                                                      _curNotificationIndex);
	if (!notification) {
		sendNotification(_curNotificationIndex, num_notifications, num_notifications,
		                 "", "", "", QByteArray());
		return;
	}

	sendNotification(_curNotificationIndex, num_notifications, num_notifications,
	                 notification->dateTime().toString(Qt::SystemLocaleShortDate),
	                 notification->title(),
	                 notification->body(),
	                 QByteArray());
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
		} else if (_mode == NotificationMode) {
			sendResponse(NavigationResponse, ResponseCancel);
			emit nextWatchletRequested();
			return;
		} else if (_mode == NotificationListMode) {
			sendResponse(NavigationResponse, ResponseCancel);
			_mode = RootMenuMode;
			return;
		}
		break;
	case SelectMenu:
		if (item_id <= _rootMenu.size()) {
			sendResponse(NavigationResponse, ResponseError);

			switch(_rootMenu[item_id].type) {
			case MenuNotificationList:
				// This should not happen!
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
	Q_UNUSED(msg);
	qDebug() << "Sending menu items";
	if (_mode == NotificationListMode) {
		_mode = RootMenuMode; // Switch to the root menu
	}
	if (_mode == RootMenuMode) {
		for (int i = 0; i < _rootMenu.size(); i++) {
			qDebug() << "Sending one menu item" << _rootMenu[i].title;
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
	         << QString::fromLocal8Bit(msg.data.constData(), msg.data.size());
}


void LiveView::sendMessageFromQueue()
{
	static const int HEADER_SIZE = 6;
	Q_ASSERT(_waitingForAck == NoMessage);

	// If there are packets to be sent...
	while (!_sendingMsgs.empty()) {
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
		qDebug() << "sending" << msg.type << packet.mid(6, 24).toHex();
#endif

		_socket->write(packet);

		_waitingForAck = ackForMessage(msg.type);
		if (_waitingForAck != NoMessage) {
			break; // Wait for that ack before sending more messages.
		}
	}
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

#if PROTOCOL_DEBUG
			qDebug() << "got header (type=" << _receivingMsg.type <<
			            "size=" << data_size << ")";
#endif
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

void LiveView::handleWatchletsChanged()
{
	recreateWatchletsMenu();
	if (_connected) {
		refreshMenu();
	}
}

void LiveView::handleNotificationsChanged()
{
	recreateNotificationsMenu();
	if (_connected) {
		refreshMenu();
	}
}
