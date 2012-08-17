import QtQuick 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	Neko {
		id: neko
		running: watch.active

		targetX: goal.x
		targetY: goal.y
	}

	Rectangle {
		id: goal
		width: 2
		height: 2
		color: "black"

		Behavior on x { SmoothedAnimation { velocity: 80; }}
		Behavior on y { SmoothedAnimation { velocity: 80; }}
	}

	function goToRandomPosition() {
		goal.x = 16 + Math.floor(Math.random() * (width - 32));
		goal.y = 16 + Math.floor(Math.random() * (height - 32));
	}

	function goToSleep() {
		neko.state = "SLEEPING";
	}

	Connections {
		target: watch
		onActiveChanged: {
			if (watch.active) {
				goToSleep();
			}
		}

		onButtonPressed : {
			switch (button) {
			case 1:
			case 2:
				goToRandomPosition();
				break;
			}
		}
	}
}
