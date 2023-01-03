/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RocketChatAccountModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AccountRoles {
        Name = Qt::UserRole + 1,
        SiteUrl,
        UserName,
        Account,
        AccountName,
    };
    Q_ENUM(AccountRoles)

    explicit RocketChatAccountModel(QObject *parent = nullptr);
    ~RocketChatAccountModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void insertAccount(RocketChatAccount *account);
    RocketChatAccount *removeAccount(const QString &name);

    void clear();

    void setAccounts(const QVector<RocketChatAccount *> &accounts);

    Q_REQUIRED_RESULT RocketChatAccount *account(int index) const;

    Q_REQUIRED_RESULT RocketChatAccount *account(const QString &accountName) const;

    Q_REQUIRED_RESULT int accountNumber() const;
    Q_REQUIRED_RESULT QStringList accountsName() const;

    Q_REQUIRED_RESULT RocketChatAccount *accountFromServerUrl(const QString &serverUrl) const;
Q_SIGNALS:
    void accountNumberChanged();

private:
    Q_DISABLE_COPY(RocketChatAccountModel)
    QVector<RocketChatAccount *> mRocketChatAccount;
};
