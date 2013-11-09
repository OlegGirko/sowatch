import QtQuick 1.0
import QtMobility.location 1.2
import com.javispedro.sowatch.qmap 1.0

Item {
	MapView {
		id: map
		anchors.fill: parent
		updateEnabled: watch.active
		updateInterval: 5000;
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch(button) {
			case 1:
				map.zoomLevel -= 1;
				break;
			case 2:
				map.zoomLevel += 1;
				break;
			}
		}
	}
}
