/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validateinviteserverjob.h"
#include "connection.h"
#include "invite/validateinvitetokenjob.h"
#include "ruqola_debug.h"

#include <KLocalizedString>
ValidateInviteServerJob::ValidateInviteServerJob(QObject *parent)
    : QObject{parent}
{
}

ValidateInviteServerJob::~ValidateInviteServerJob() = default;

bool ValidateInviteServerJob::canStart() const
{
    return !mInfo.serverUrl.isEmpty() && !mInfo.inviteToken.isEmpty();
}

void ValidateInviteServerJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to check invite server";
        deleteLater();
        return;
    }

    auto job = new RocketChatRestApi::ValidateInviteTokenJob(this);
    job->setToken(mInfo.inviteToken);
    auto restApi = new Connection(this);
    restApi->setServerUrl(mInfo.serverUrl);
    restApi->initializeRestApiJob(job);

    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::validateInviteTokenDone, this, [this, restApi]() {
        restApi->deleteLater();
        Q_EMIT tokenIsValid(mInfo);
        deleteLater();
    });
    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::inviteTokenInvalid, this, [restApi, this]() {
        Q_EMIT tokenIsInvalid();
        restApi->deleteLater();
        deleteLater();
    });

    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start ValidateInviteTokenJob";
    }
}

AccountManager::AccountManagerInfo ValidateInviteServerJob::info() const
{
    return mInfo;
}

void ValidateInviteServerJob::setInfo(const AccountManager::AccountManagerInfo &newInfo)
{
    mInfo = newInfo;
}

#include "moc_validateinviteserverjob.cpp"
