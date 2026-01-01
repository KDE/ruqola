/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RemoveInviteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RemoveInviteJob(QObject *parent = nullptr);
    ~RemoveInviteJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &identifier);

Q_SIGNALS:
    void removeInviteDone(const QByteArray &identifier);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotRemoveInviteFinished();
    QByteArray mIdentifier;
};
}
