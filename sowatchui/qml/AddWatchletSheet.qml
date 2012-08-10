import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import com.javispedro.sowatch 1.0

Sheet {
	id: sheet
	anchors.margins: UiConstants.DefaultMargin

	property string configKey;

	rejectButtonText: qsTr("Cancel")

	content: ListView {
		id: listView
		anchors.fill: parent
		anchors.margins: UiConstants.DefaultMargin

		flickableDirection: Flickable.VerticalFlick

		model: WatchletsModel {
			id: watchletsModel
			configKey: sheet.configKey
			displayUnadded: true
		}

		header: Column {
			width: parent.width

			GroupHeader {
				width: parent.width
				text: qsTr("Available watchlets")
			}
		}

		delegate: ListDelegate {
			onClicked: {
				watchletsModel.addWatchlet(model.name);
				close();
				accepted();
			}
		}

		ScrollDecorator {
			flickableItem: listView
		}
	}
}
