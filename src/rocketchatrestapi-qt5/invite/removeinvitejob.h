/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RemoveInviteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RemoveInviteJob(QObject *parent = nullptr);
    ~RemoveInviteJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

Q_SIGNALS:
    void removeInviteDone(const QString &identifier);

private:
    Q_DISABLE_COPY(RemoveInviteJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void slotRemoveInviteFinished();
    QString mIdentifier;
};
}
