import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Sheet {
	id: sheet
	anchors.margins: UiConstants.DefaultMargin

	rejectButtonText: qsTr("Cancel")

	title: BusyIndicator {
		anchors.centerIn: parent;
		running: watchScanner.active
	}

	Binding {
		target: watchScanner
		property: "enabled"
		value: sheet.status === DialogStatus.Open
	}

	content: ListView {
		id: listView
		anchors.fill: parent
		anchors.margins: UiConstants.DefaultMargin

		flickableDirection: Flickable.VerticalFlick

		model: watchScanner

		header: Column {
			width: parent.width

			Label {
				text: qsTr("Pick a watch")
				font: UiConstants.HeaderFont
			}
			GroupHeader {
				width: parent.width
				text: qsTr("Nearby watches")
			}
		}

		delegate: ListDelegate {
			onClicked: {
				watches.addFoundWatch(object);
				close();
				accepted();
			}
		}

		ScrollDecorator {
			flickableItem: listView
		}
	}
}
