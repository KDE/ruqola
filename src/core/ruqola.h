/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "ddpapi/ddpclient.h"
#include "libruqolacore_export.h"
#include "model/messagemodel.h"
#include "model/roommodel.h"
#include <QObject>

class RocketChatAccount;
class AccountManager;
class LIBRUQOLACORE_EXPORT Ruqola : public QObject
{
    Q_OBJECT
public:
    ~Ruqola() override;
    /**
     * @brief Singleton provider
     *
     * @return Returns the singleton object
     */
    static Ruqola *self();

    static void destroy();
    RocketChatAccount *rocketChatAccount() const;

    AccountManager *accountManager() const;

    explicit Ruqola(QObject *parent = nullptr);

    void setCurrentAccount(const QString &accountName);

    void openMessageUrl(const QString &url);

Q_SIGNALS:
    void translatorMenuChanged();

private:
    Q_DISABLE_COPY(Ruqola)
    AccountManager *mAccountManager = nullptr;
};
