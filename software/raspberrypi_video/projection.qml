import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Item {
	id: root
	
	Rectangle {
		anchors.fill: parent
		anchors.margins: 10

		Label {
			anchors { left: parent.left; right: parent.right; top: parent.top; }
			text: "Projection parameters"
			font.pixelSize: 20
		}


		Slider {
			from: -90.0
			to: 90.0
			value: 0.0
		}
	}
}
