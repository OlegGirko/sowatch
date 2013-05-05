import QtQuick 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	// Remember that firmware draws top 30 lines

	Connections {
		target: watch
		onActiveChanged: {
			console.log("watchface is now " + (watch.active ? "active" : "inactive"))
		}
	}

	MWLabel {
		anchors.centerIn: parent
		text: "This is a test"
	}
}
