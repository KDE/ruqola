/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include "parserocketchaturlutils.h"
#include "ruqolaserverconfig.h"

#include <QIcon>
#include <QObject>
class RocketChatAccount;
#if HAVE_ACTIVITY_SUPPORT
class ActivitiesManager;
#endif
class RocketChatAccountModel;
class RocketChatAccountFilterProxyModel;
class LIBRUQOLACORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT ActivitySettings {
        QStringList activities;
        bool enabled = false;
        [[nodiscard]] bool contains(const QString &str) const
        {
            return activities.contains(str);
        }

        void changeActivities(bool added, const QString &currentActivity)
        {
            if (added) {
                if (!activities.contains(currentActivity)) {
                    activities.append(currentActivity);
                }
            } else {
                if (activities.contains(currentActivity)) {
                    activities.removeAll(currentActivity);
                }
            }
            enabled = true;
        }
    };

    struct LIBRUQOLACORE_EXPORT AccountManagerInfo {
        QList<AuthenticationInfo> authenticationInfos;
        ActivitySettings activitiesSettings;
        QString displayName;
        QString accountName;
        QString userName;
        QString serverUrl;
        QString password;
        QString token;
        QString inviteToken;

        QByteArray userId;
        AuthenticationManager::AuthMethodType authMethodType = AuthenticationManager::AuthMethodType::Unknown;
        RuqolaServerConfig::PasswordSettings passwordSettings;
        bool canResetPassword = false;
        bool enabled = true;
        bool canRegisterAccount = false;
        bool accountsManuallyApproveNewUsers = false;
    };

    struct LIBRUQOLACORE_EXPORT AccountDisplayInfo {
        QString name;
        QIcon icon;
    };

    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager() override;

    void removeAccount(const QString &accountName, bool removeLogFiles = false);
    void addAccount(const AccountManagerInfo &info);

    [[nodiscard]] RocketChatAccount *account() const;
    [[nodiscard]] RocketChatAccountModel *rocketChatAccountModel() const;

    [[nodiscard]] RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel() const;

    void addAccount(RocketChatAccount *account);

    void setCurrentAccount(const QString &accountName);
    [[nodiscard]] QString currentAccount() const;

    void modifyAccount(const AccountManagerInfo &info);
    [[nodiscard]] QStringList accountsName() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int accountNumber() const;
    [[nodiscard]] bool showMessage(const ParseRocketChatUrlUtils::ParsingInfo &parseInfo);
    [[nodiscard]] RocketChatAccount *accountFromName(const QString &accountName);

    [[nodiscard]] QList<AccountDisplayInfo> accountDisplayInfoSorted() const;
#if HAVE_ACTIVITY_SUPPORT
    ActivitiesManager *activitiesManager() const;
#endif

    void addInvitedAccount(const AccountManagerInfo &info);
Q_SIGNALS:
    void logoutAccountDone(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void currentAccountChanged();
    void roomNeedAttention();
    void activitiesChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void selectAccount(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void removeDatabaseAccount(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void removeDirectory(const QString &directory);
    LIBRUQOLACORE_NO_EXPORT void removeLogs(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void loadAccount();
    LIBRUQOLACORE_NO_EXPORT void connectToAccount(RocketChatAccount *account);
    LIBRUQOLACORE_NO_EXPORT void slotSwitchToAccountAndRoomName(const QString &accountName, const QByteArray &roomId, const QString &channelType);
    LIBRUQOLACORE_NO_EXPORT void loadExcludeEmoticons();
    LIBRUQOLACORE_NO_EXPORT void changeEnableState(RocketChatAccount *account, bool enabled);
    LIBRUQOLACORE_NO_EXPORT void disconnectAccount(RocketChatAccount *account);
    RocketChatAccount *mCurrentAccount = nullptr;
    RocketChatAccountModel *const mRocketChatAccountModel;
    RocketChatAccountFilterProxyModel *const mRocketChatAccountProxyModel;
#if HAVE_ACTIVITY_SUPPORT
    ActivitiesManager *const mActivitiesManager;
#endif
};
Q_DECLARE_TYPEINFO(AccountManager::AccountManagerInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AccountManager::AccountDisplayInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountManager::AccountManagerInfo &t);
