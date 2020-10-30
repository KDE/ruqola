/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef LOGINMETHODMODEL_H
#define LOGINMETHODMODEL_H

#include "authenticationinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT LoginMethodModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentLoginMethod READ currentLoginMethod NOTIFY currentLoginMethodChanged)
public:
    enum LoginRoles {
        Name = Qt::UserRole + 1,
        IconName,
        Type,
    };
    Q_ENUM(LoginRoles)
    explicit LoginMethodModel(QObject *parent = nullptr);
    ~LoginMethodModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setAuthenticationInfos(const QVector<AuthenticationInfo> &infos);
    void clear();

    Q_REQUIRED_RESULT AuthenticationManager::OauthType loginType(int index);

    Q_REQUIRED_RESULT int currentLoginMethod() const;
    void setCurrentLoginMethod(int currentLoginMethod);

    Q_REQUIRED_RESULT QVector<AuthenticationInfo> authentications() const;

Q_SIGNALS:
    void currentLoginMethodChanged();

private:
    Q_DISABLE_COPY(LoginMethodModel)
    QVector<AuthenticationInfo> mAuthentications;
    int mCurrentLoginMethod = 0;
};

#endif // LOGINMETHODMODEL_H
