import Qt 4.7
import com.javispedro.sowatch.liveview 1.0

LVPage {
	Column {
		anchors.fill: parent
		spacing: 2

		LVLabel {
			text: "Battery: " + batteryLevel + "%"
		}

		Rectangle {
			id: battery
			anchors.horizontalCenter: parent.horizontalCenter
			width: 72
			height: 16

			border.color: "white"
			border.width: 1

			Rectangle {
				width: (batteryLevel / 100) * parent.width
				height: parent.height

				color: "white"
			}
		}

		LVLabel {
			width: parent.width
			text: "Connected to:"
		}

		LVLabel {
			width: parent.width
			text: networkName
			horizontalAlignment: Text.AlignHCenter
			wrapMode: Text.Wrap
		}
	}
}
