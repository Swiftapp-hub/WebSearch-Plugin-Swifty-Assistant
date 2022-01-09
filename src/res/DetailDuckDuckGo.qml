import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {
    width: parent.width
    height: parent.height

    Connections {
        target: swifty

        function onPluginSendedMessageToQml(message, id) {
            if (id === "fr.swifty.websearch") {
                var text = ""

                for (var i = 2; i < message.length; i++) {
                    text += message[i]
                }

                if (message[0] === "h") {
                    header.text = text
                }

                else if (message[0] === "l") {
                    link.text = text
                    link.visible = true
                }

                else if (message[0] === "i") {
                    logo.source = text
                }

                else if (message[0] === "c") {
                    info.text = text
                }

                else if (message[0] === "s") {
                    linkSource.text = text
                }

                else if (message[0] === "u") {
                    linkSource.url = text
                }
            }
        }
    }

    BusyIndicator {
        id: loading
        running: true
        implicitWidth: 40
        implicitHeight: implicitWidth
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Item {
        id: content
        visible: false
        anchors.fill: parent

        RowLayout {
            id: headerLayout
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 10
            anchors.topMargin: 30
            height: 100
            spacing: 10

            ColumnLayout {
                Layout.fillHeight: true
                spacing: 10

                TextEdit {
                    id: header
                    readOnly: true
                    selectByMouse: true
                    text: "Chargement..."
                    color: "white"
                    font.pointSize: 16
                    font.bold: true
                    wrapMode: Label.Wrap
                }

                Button {
                    id: link
                    visible: false
                    text: qsTr("Chargement...")
                    font.pointSize: 8
                    implicitHeight: 21

                    property int radius: 10
                    property int borderWidth: 2

                    property string primaryColor: "#ffc9b3"
                    property string primaryColorlicked: "#95799c"

                    contentItem: Text {
                        text: link.text
                        font: link.font
                        opacity: enabled ? 1.0 : 0.3
                        color: link.down ? link.primaryColorlicked : link.primaryColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 40
                        opacity: enabled ? 1 : 0.3
                        color: "#00ffffff"
                        border.color: link.down ? link.primaryColorlicked : link.primaryColor
                        border.width: link.down ? 1 : link.borderWidth
                        radius: link.radius
                    }

                    onClicked: {
                        anim.running = true

                        swifty.execAction("web_message with_action_btn site "+link.text)
                    }

                    NumberAnimation {
                        id: anim
                        target: link
                        duration: 500
                        from: 0.2
                        to: link.opacity
                        property: "opacity"
                    }
                }

                Button {
                    id: linkSource
                    text: qsTr("Chargement...")
                    font.pointSize: 8
                    implicitHeight: 21

                    property int radius: 10
                    property int borderWidth: 2

                    property string primaryColor: "#ffc9b3"
                    property string primaryColorlicked: "#95799c"

                    property string url

                    contentItem: Text {
                        text: linkSource.text
                        font: linkSource.font
                        opacity: enabled ? 1.0 : 0.3
                        color: linkSource.down ? linkSource.primaryColorlicked : linkSource.primaryColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 40
                        opacity: enabled ? 1 : 0.3
                        color: "#00ffffff"
                        border.color: linkSource.down ? linkSource.primaryColorlicked : linkSource.primaryColor
                        border.width: linkSource.down ? 1 : linkSource.borderWidth
                        radius: linkSource.radius
                    }

                    onClicked: {
                        anim2.running = true

                        swifty.execAction("web_message with_action_btn site "+linkSource.url)
                    }

                    NumberAnimation {
                        id: anim2
                        target: link
                        duration: 500
                        from: 0.2
                        to: link.opacity
                        property: "opacity"
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Image {
                id: logo
                source: "qrc:/Icon/CommonIcon/loader.svg"
                sourceSize.height: 100

                onStatusChanged: {
                    if (status == Image.Ready) {
                        loading.visible = false
                        content.visible = true
                    }
                }
            }
        }

        ColumnLayout {
            anchors.top: headerLayout.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            anchors.topMargin: 30
            spacing: 10

            TextEdit {
                id: info
                readOnly: true
                selectByMouse: true
                textFormat: Text.StyledText
                text: qsTr("Chargement...")
                color: "white"
                font.pointSize: 13
                Layout.fillWidth: true
                Layout.fillHeight: true
                wrapMode: Label.Wrap
            }
        }
    }
}
