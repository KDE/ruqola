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
        NameRole = Qt::UserRole + 1,
        IconNameRole,
        TypeRole,
    };
    Q_ENUM(LoginRoles)
    explicit LoginMethodModel(QObject *parent = nullptr);
    ~LoginMethodModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void setAuthenticationInfos(const QVector<AuthenticationInfo> &infos);
    void clear();

    [[nodiscard]] AuthenticationManager::AuthMethodType loginType(int index);

    [[nodiscard]] int currentLoginMethod() const;
    void setCurrentLoginMethod(int currentLoginMethod);

    [[nodiscard]] QVector<AuthenticationInfo> authentications() const;

Q_SIGNALS:
    void currentLoginMethodChanged();

private:
    QVector<AuthenticationInfo> mAuthentications;
    int mCurrentLoginMethod = 0;
};
