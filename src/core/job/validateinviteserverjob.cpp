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

    // restApi is a child of this, deleting this deletes it too.
    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::validateInviteTokenDone, this, [this]() {
        Q_EMIT tokenIsValid(mInfo);
        deleteLater();
    });
    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::inviteTokenInvalid, this, [this]() {
        Q_EMIT tokenIsInvalid();
        deleteLater();
    });
    // Http/server errors are reported by failed(), neither of the two signals above is emitted then.
    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::failed, this, [this](const QString &serverErrorStr) {
        qCWarning(RUQOLA_LOG) << "ValidateInviteTokenJob failed:" << serverErrorStr;
        Q_EMIT tokenIsInvalid();
        deleteLater();
    });
    // A network-level error (unknown host, lost connection...) is swallowed by RestApiAbstractJob, which
    // returns early to let the account reconnect: none of the three signals above is emitted then. An
    // invite url pointing at a server which does not exist ends up here, so it must not be silent.
    connect(restApi, &Connection::networkError, this, [this]() {
        qCWarning(RUQOLA_LOG) << "Network error while validating invite token for" << mInfo.serverUrl;
        Q_EMIT tokenIsInvalid();
        deleteLater();
    });

    if (!job->start()) {
        Q_EMIT tokenIsInvalid();
        qCWarning(RUQOLA_LOG) << "Impossible to start ValidateInviteTokenJob";
        deleteLater();
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
