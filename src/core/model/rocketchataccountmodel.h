/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void insertAccount(RocketChatAccount *account);
    RocketChatAccount *removeAccount(const QString &name);

    void clear();

    void setAccounts(const QList<RocketChatAccount *> &accounts);

    [[nodiscard]] RocketChatAccount *account(int index) const;

    [[nodiscard]] RocketChatAccount *account(const QString &accountName) const;

    [[nodiscard]] int accountNumber() const;
    [[nodiscard]] QStringList accountsName() const;

    [[nodiscard]] RocketChatAccount *accountFromServerUrl(const QString &serverUrl) const;
Q_SIGNALS:
    void accountNumberChanged();

private:
    QList<RocketChatAccount *> mRocketChatAccount;
};
