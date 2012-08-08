import QtQuick 1.1
import com.nokia.meego 1.1

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    MainPage {
        id: mainPage
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
			MenuItem {
				text: qsTr("Crap")
				onClicked: {
					console.log("Crap")
				}
			}
        }
    }
}
