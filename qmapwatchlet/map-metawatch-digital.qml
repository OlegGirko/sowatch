import QtQuick 1.0
import QtMobility.location 1.2
import com.javispedro.sowatch.metawatch 1.0
import com.javispedro.sowatch.qmap 1.0

MWPage {
	MWTitle {
		id: title
		z: 1
	}

	MapView {
		id: map
		anchors.fill: parent
		updateEnabled: watch.active
		updateInterval: 5000;
		decolor: true
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
