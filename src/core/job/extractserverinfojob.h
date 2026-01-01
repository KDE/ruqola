/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "authenticationinfo.h"
#include "libruqolacore_export.h"
#include "ruqolaserverconfig.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT ExtractServerInfoJob : public QObject
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT ServerInfo {
        QString url;
        QList<AuthenticationInfo> authenticationInfos;
        RuqolaServerConfig::PasswordSettings passwordSettings;
        bool canResetPassword = false;
        bool canRegisterAccount = false;
        bool accountsManuallyApproveNewUsers = false;
    };

    explicit ExtractServerInfoJob(QObject *parent = nullptr);
    ~ExtractServerInfoJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &newServerUrl);

Q_SIGNALS:
    void serverInfoFound(const ExtractServerInfoJob::ServerInfo &info);
    void errorConnection(const QString &);

private:
    QString mServerUrl;
};
Q_DECLARE_TYPEINFO(ExtractServerInfoJob::ServerInfo, Q_RELOCATABLE_TYPE);
