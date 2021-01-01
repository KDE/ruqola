/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#ifndef CHANNELJOINJOB_H
#define CHANNELJOINJOB_H

#include "librestapi_private_export.h"
#include "channelbasejob.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelJoinJob : public ChannelBaseJob
{
    Q_OBJECT
public:
    explicit ChannelJoinJob(QObject *parent = nullptr);
    ~ChannelJoinJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString joinCode() const;
    void setJoinCode(const QString &joinCode);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void setChannelJoinDone(const ChannelBaseJob::ChannelInfo &channelInfo);
    void missingChannelPassword(const ChannelBaseJob::ChannelInfo &channelInfo);
    void openArchivedRoom(const ChannelBaseJob::ChannelInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelJoinJob)
    void slotChannelJoinFinished();
    QString mJoinCode;
};
}
#endif // CHANNELJOINJOB_H
