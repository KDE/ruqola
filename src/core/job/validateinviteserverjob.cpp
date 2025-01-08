/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

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
        qDebug() << " Token is valid !!!!";
        // TODO it's valid !
        // TODO create account
#if 0
        const QString newAccountName = Utils::createUniqueAccountName(accountsName(), info.accountName);
        auto account = new RocketChatAccount();
        account->setAccountName(newAccountName);
        account->setServerUrl(info.serverUrl);
        account->setAccountEnabled(info.enabled);
        account->setActivities(info.activitiesSettings.activities);
        account->setActivityEnabled(info.activitiesSettings.enabled);
        if (info.authMethodType == AuthenticationManager::AuthMethodType::Password) {
            account->setUserName(info.userName);
            account->setPassword(info.password);
        } else if (info.authMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
            account->setAuthToken(info.token);
            account->setUserId(info.userId);
        } else {
            // TODO for other authMethodType ?
            // google used ?
            // Fb ?
            // Gitlab ?
            // GitHub ?
        }
        account->setAuthMethodType(info.authMethodType);
        if (info.enabled) {
            connectToAccount(account);
        }
        addAccount(account);
#endif
        deleteLater();
    });
    connect(job, &RocketChatRestApi::ValidateInviteTokenJob::inviteTokenInvalid, this, [restApi, this]() {
        // TODO show info ?
        qDebug() << " Token is invalid !!!!";
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
