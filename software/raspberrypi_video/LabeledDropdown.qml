import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
    id: root
	height: 40
	anchors { left: parent.left; right: parent.right }

    property alias text: label.text
	property alias model: dropdown.model
	property alias textRole: dropdown.textRole
	property alias currentIndex: dropdown.currentIndex

	Label {
		id: label
		width: 50
		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter
		anchors.margins: 5
	}

	ComboBox {
		id: dropdown
		anchors.left: label.right
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
		anchors.margins: 5
	}
}
