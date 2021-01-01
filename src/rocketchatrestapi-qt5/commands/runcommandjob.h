/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef RUNCOMMANDJOB_H
#define RUNCOMMANDJOB_H

#include "restapiabstractjob.h"
#include "librocketchatrestapi-qt5_export.h"
#include <QDebug>
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_EXPORT RunCommandJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RunCommandInfo {
        QString commandName;
        QString roomId;
        QString threadMessageId;
        QString triggerId;
        QString params;
        Q_REQUIRED_RESULT bool isValid() const;
    };
    explicit RunCommandJob(QObject *parent = nullptr);
    ~RunCommandJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT RunCommandInfo runCommandInfo() const;
    void setRunCommandInfo(const RunCommandInfo &runCommandInfo);

    static RunCommandInfo parseString(const QString &str, const QString &roomId, const QString &tmid);
Q_SIGNALS:
    void runCommandDone();

private:
    Q_DISABLE_COPY(RunCommandJob)
    void slotRunCommand();
    RunCommandInfo mRunCommandInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::RunCommandJob::RunCommandInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::RunCommandJob::RunCommandInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::RunCommandJob::RunCommandInfo &t);

#endif // RunCommandJob_H
