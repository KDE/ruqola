/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QAbstractListModel>
#include <QObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MultiStaticSelectLineEditModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MultiStaticSelectLineEditRoles {
        Value = Qt::UserRole + 1,
    };
    Q_ENUM(MultiStaticSelectLineEditRoles)

    struct UserCompletionInfo {
        QString text;
        QString value;
        [[nodiscard]] bool isValid() const;
    };
    explicit MultiStaticSelectLineEditModel(QObject *parent = nullptr);
    ~MultiStaticSelectLineEditModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QList<UserCompletionInfo> userCompletionInfos() const;
    void setUserCompletionInfos(const QList<UserCompletionInfo> &newUserCompletionInfos);

private:
    QList<UserCompletionInfo> mUserCompletionInfos;
};
Q_DECLARE_TYPEINFO(MultiStaticSelectLineEditModel::UserCompletionInfo, Q_RELOCATABLE_TYPE);
