/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ROCKETCHATACCOUNTMODEL_H
#define ROCKETCHATACCOUNTMODEL_H

#include <QAbstractListModel>
#include "libruqola_private_export.h"
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccountModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AccountRoles {
        Name = Qt::UserRole + 1,
        SiteUrl
    };
    Q_ENUM(AccountRoles)

    explicit RocketChatAccountModel(QObject *parent = nullptr);
    ~RocketChatAccountModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void insertAccount(RocketChatAccount *account);
    void removeAccount(const QString &name);
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    void clear();

    void setAccounts(const QVector<RocketChatAccount *> &accounts);

    Q_REQUIRED_RESULT RocketChatAccount *account(int index) const;

    Q_REQUIRED_RESULT RocketChatAccount *account(const QString &accountName) const;
private:
    Q_DISABLE_COPY(RocketChatAccountModel)
    QVector<RocketChatAccount *> mRocketChatAccount;
};

#endif // ROCKETCHATACCOUNTMODEL_H
