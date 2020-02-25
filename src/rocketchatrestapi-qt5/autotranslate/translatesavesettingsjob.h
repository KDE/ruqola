/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef TRANSLATESAVESETTINGSJOB_H
#define TRANSLATESAVESETTINGSJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT TranslateSaveSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateSaveSettingsJob(QObject *parent = nullptr);
    ~TranslateSaveSettingsJob() override;

    enum SettingType {
        AutoTranslateSetting,
        LanguageSetting,
        Underfined
    };

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool autoTranslate() const;
    void setAutoTranslate(bool autoTranslate);

    Q_REQUIRED_RESULT QString language() const;
    void setLanguage(const QString &language);

    Q_REQUIRED_RESULT SettingType type() const;
    void setType(SettingType type);

Q_SIGNALS:
    void translateSavesettingsDone();

private:
    Q_DISABLE_COPY(TranslateSaveSettingsJob)
    void slotTranslateSaveSettingsFinished();
    QString mRoomId;
    QString mLanguage;
    TranslateSaveSettingsJob::SettingType mType = Underfined;
    bool mAutoTranslate = false;
};
}
#endif // TRANSLATESAVESETTINGSJOB_H
