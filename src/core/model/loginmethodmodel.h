/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT LoginMethodModel : public QAbstractListModel
{
    Q_OBJECT
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
