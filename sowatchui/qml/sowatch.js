var service = null;

function getService() {
	var component = Qt.createComponent("ServiceLoader.qml");
	if (component.status == Component.Ready) {
		var loader = component.createObject(null);
		service = loader.serviceObject;
	}
}

function checkService() {
	if (service === null) {
		getService();
	}
}

function start() {
	getService();
}

function stop() {
	checkService();
	service.terminate();
}

function restart() {
	stop();
	start();
}
