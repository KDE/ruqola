/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QAbstractListModel>
class QObject;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MultiStaticSelectLineEditModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MultiStaticSelectLineEditRoles {
        Value = Qt::UserRole + 1,
        Text,
    };
    Q_ENUM(MultiStaticSelectLineEditRoles)

    struct SelectItemCompletionInfo {
        QString text;
        QString value;
        [[nodiscard]] bool isValid() const;
    };
    explicit MultiStaticSelectLineEditModel(QObject *parent = nullptr);
    ~MultiStaticSelectLineEditModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QList<SelectItemCompletionInfo> userCompletionInfos() const;
    void setUserCompletionInfos(const QList<SelectItemCompletionInfo> &newUserCompletionInfos);

    [[nodiscard]] MultiStaticSelectLineEditModel::SelectItemCompletionInfo itemCompletionInfo(const QString &value) const;

private:
    QList<SelectItemCompletionInfo> mUserCompletionInfos;
};
Q_DECLARE_TYPEINFO(MultiStaticSelectLineEditModel::SelectItemCompletionInfo, Q_RELOCATABLE_TYPE);
