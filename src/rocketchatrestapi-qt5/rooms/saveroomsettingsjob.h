/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SaveRoomSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT SaveRoomSettingsInfo {
        enum SettingChanged {
            Unknown = 0,
            SystemMessages = 1,
            RoomName = 2,
            RoomTopic = 4,
            RoomAnnouncement = 8,
            RoomDescription = 16,
            RetentionEnabled = 32,
            RetentionExcludePinned = 64,
            RetentionFilesOnly = 128,
            RetentionIgnoreThreads = 256,
            RetentionOverrideGlobal = 512,
            RetentionMaxAge = 1024,
            Favorite = 2048,
        };
        Q_DECLARE_FLAGS(SettingsChanged, SettingChanged)

        QStringList systemMessages;
        QString roomId;
        QString roomName;
        QString roomTopic;
        QString roomAnnouncement;
        QString roomDescription;
        bool retentionEnabled = false;
        bool retentionExcludePinned = false;
        bool retentionFilesOnly = false;
        bool retentionIgnoreThreads = false;
        bool retentionOverrideGlobal = false;
        bool favorite = false;
        int retentionMaxAge = 32;
        //'roomAvatar', 'featured','roomCustomFields', 'roomType', 'readOnly',
        //'reactWhenReadOnly', 'default', 'joinCode', 'tokenpass', 'streamingOptions'
        //'encrypted', 'favorite'
        SettingsChanged mSettingsWillBeChanged = SettingChanged::Unknown;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit SaveRoomSettingsJob(QObject *parent = nullptr);
    ~SaveRoomSettingsJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT SaveRoomSettingsInfo saveRoomSettingsInfo() const;
    void setSaveRoomSettingsInfo(const SaveRoomSettingsInfo &saveRoomSettingsInfo);

Q_SIGNALS:
    void changeNotificationDone();

private:
    Q_DISABLE_COPY(SaveRoomSettingsJob)
    void slotChangeNotificationFinished();

    SaveRoomSettingsInfo mSaveRoomSettingsInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged)
Q_DECLARE_TYPEINFO(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo &t);
