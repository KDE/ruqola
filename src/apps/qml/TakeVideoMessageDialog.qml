/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtMultimedia 5.8
import org.kde.kirigami 2.7 as Kirigami
import KDE.Ruqola.RocketChatAccount 1.0
QQC2.Dialog {
    id: takeVideoMessageDialog


    property QtObject rcAccount

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: 300
    height: 500

    modal: true
    focus: true
    property bool isNotCameraAvailable: camera.availability === Camera.Unavailable || camera.availability === Camera.ResourceMissing
    standardButtons: isNotCameraAvailable == true ? QQC2.Dialog.Close : QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    Camera {
        id: camera
        videoRecorder.audioEncodingMode: CameraRecorder.ConstantBitrateEncoding;
        videoRecorder.audioBitRate: 128000
        videoRecorder.mediaContainer: "mp4"
        videoRecorder.outputLocation: rcAccount.recordingVideoPath
    }

    Kirigami.InlineMessage {
        visible: isNotCameraAvailable
        anchors.centerIn: parent
        width: 300
        height: 60
        text: i18n("There is no camera available.")
    }
    Kirigami.InlineMessage {
        visible: camera.availability === Camera.Busy
        anchors.centerIn: parent
        width: 300
        height: 60
        text: i18n("Your camera is busy.\nTry to close other applications using the camera.")
    }
    VideoOutput {
        id: camareLiveOutput
        source: camera
        anchors.fill: parent
        autoOrientation: true
        focus: visible
        visible: !isNotCameraAvailable && (camera.cameraStatus === Camera.ActiveStatus)
        enabled: !isNotCameraAvailable && (camera.cameraStatus === Camera.ActiveStatus)
    }
    QQC2.Button {
        text: i18n("Video");
        visible: !isNotCameraAvailable
        onPressed: {
            if (camera.cameraStatus === camera.StartingStatus)
                camera.stop()
            else
                camera.start()
        }
    }

    Component.onCompleted: {
        camera.stop()
    }
    //TODO add info when we can't have camera ok
    onClosed: {
        camera.stop()
    }
}
