#include "bluetoothwatch.h"

using namespace sowatch;
QTM_USE_NAMESPACE

const int BluetoothWatch::connectRetryTimes[] = {
	5, 10, 30, 60, 120, 300
};

BluetoothWatch::BluetoothWatch(const QBluetoothAddress& address, QObject *parent)
	: Watch(parent),
      _localDev(new QBluetoothLocalDevice(this)),
      _address(address),
	  _socket(0),
      _connected(false),
      _connectRetries(0),
	  _connectTimer(new QTimer(this)),
	  _connectAlignedTimer(new QSystemAlignedTimer(this))
{
	_connectTimer->setSingleShot(true);
	_connectAlignedTimer->setSingleShot(true);

	connect(_connectTimer, SIGNAL(timeout()), SLOT(handleConnectTimer()));
	connect(_connectAlignedTimer, SIGNAL(timeout()), SLOT(handleConnectTimer()));
	connect(_localDev, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
	        SLOT(handleLocalDevModeChanged(QBluetoothLocalDevice::HostMode)));

	// Check to see if we can connect right away
	if (_localDev->isValid() &&
	        _localDev->hostMode() != QBluetoothLocalDevice::HostPoweredOff) {
		// Do an initial connection attempt after a short delay
		// (To give time for other plugins to initialize, etc.)
		scheduleConnect();
	} else {
		qDebug() << "Not starting MetaWatch connection because BT is off";
	}
}

BluetoothWatch::~BluetoothWatch()
{
	if (_socket) {
		_socket->close();
		delete _socket;
	}
}

bool BluetoothWatch::isConnected() const
{
	return _connected;
}

void BluetoothWatch::scheduleConnect()
{
	if (_connected ||
	        _connectAlignedTimer->isActive() || _connectTimer->isActive()) {
		// Already connected or already scheduled to connect.
		return;
	}

	_connectRetries = 0;
	_connectTimer->start(100);
}

void BluetoothWatch::scheduleRetryConnect()
{
	if (_connected ||
	        _connectAlignedTimer->isActive() || _connectTimer->isActive()) {
		// Already connected or already scheduled to connect.
		return;
	}

	int timeToNextRetry;
	if (_connectRetries >= connectRetryTimesSize) {
		timeToNextRetry = connectRetryTimes[connectRetryTimesSize - 1];
	} else {
		timeToNextRetry = connectRetryTimes[_connectRetries];
		_connectRetries++; // Increase the number of connection attemps
	}

	qDebug() << "Backing off for" << timeToNextRetry << "seconds for next retry";
	_connectAlignedTimer->start(timeToNextRetry / 2, timeToNextRetry * 2);
	if (_connectAlignedTimer->lastError() != QSystemAlignedTimer::NoError) {
		// Hopefully a future version of QSystemAlignedTimer implements this fallback
		// For now, we have to do it ourselves.
		qDebug() << "Note: using plain QTimer for retry";
		_connectTimer->start(timeToNextRetry * 1000);
	}
}

void BluetoothWatch::unscheduleConnect()
{
	_connectAlignedTimer->stop();
	_connectTimer->stop();
}

void BluetoothWatch::connectToWatch()
{
	delete _socket; //Delete socket from previous connect if any.
	_socket = new QBluetoothSocket(QBluetoothSocket::RfcommSocket);

	connect(_socket, SIGNAL(connected()), SLOT(handleSocketConnected()));
	connect(_socket, SIGNAL(disconnected()), SLOT(handleSocketDisconnected()));
	connect(_socket, SIGNAL(readyRead()), SLOT(handleSocketData()));
	connect(_socket, SIGNAL(error(QBluetoothSocket::SocketError)),
			SLOT(handleSocketError(QBluetoothSocket::SocketError)));
	connect(_socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
			SLOT(handleSocketState(QBluetoothSocket::SocketState)));

	_socket->connectToService(_address, 1, QIODevice::ReadWrite | QIODevice::Unbuffered);
}

void BluetoothWatch::handleConnectTimer()
{
	connectToWatch();
}

void BluetoothWatch::handleLocalDevModeChanged(QBluetoothLocalDevice::HostMode state)
{
	qDebug() << "Local bluetooth device mode changed to" << state;
	if (state == QBluetoothLocalDevice::HostPoweredOff) {
		// Host bluetooth was powered down
		// Assume the socket has been disconnected
		handleSocketDisconnected();
		// Cancel any pending connection attempts
		unscheduleConnect();
	} else {
		// Host bluetooth might have been powered up
		if (!_connected) {
			scheduleConnect();
		}
	}
}

void BluetoothWatch::handleSocketConnected()
{
	if (!_connected) {
		qDebug() << "connected";

		_connected = true;
		_connectRetries = 0;

		setupBluetoothWatch();

		emit connected();
	}
}

void BluetoothWatch::handleSocketDisconnected()
{
	// Signal disconnection if necessary
	if (_connected) {
		qDebug() << "disconnected";

		_connected = false;
		desetupBluetoothWatch();

		emit disconnected();
	}

	// Setup reconnection attempt if necessary
	if (_localDev->hostMode() != QBluetoothLocalDevice::HostPoweredOff) {
		scheduleRetryConnect();
	}
}

void BluetoothWatch::handleSocketError(QBluetoothSocket::SocketError error)
{
	qWarning() << "Socket error:" << error;
	// Seems that sometimes a disconnection event may not be generated.
	handleSocketDisconnected();
}

void BluetoothWatch::handleSocketState(QBluetoothSocket::SocketState state)
{
	qDebug() << "socket is in" << state;
}
