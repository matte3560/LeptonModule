import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
    id: slider
	width: 100
	height: 200

    property alias text: label.text
	property alias from: slider.from
	property alias to: slider.to
	property alias value: slider.value

    border.color: "black"
    border.width: 1

	Label {
		id: label
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
	}

	Slider {
		id: slider
		anchors.top: label.bottom
		anchors.left: parent.left
		anchors.right: parent.right
	}

    Label {
		id: number
		anchors.top: label.bottom
		anchors.left: slider.right
		anchors.right: parent.right

		text: slider.value
    }
}
