/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QSortFilterProxyModel>
class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonRecentUsedFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonRecentUsedFilterProxyModel(QObject *parent = nullptr);
    ~EmoticonRecentUsedFilterProxyModel() override;

    Q_REQUIRED_RESULT QStringList usedIdentifier() const;
    void setUsedIdentifier(const QStringList &usedIdentifier);

    void addIdentifier(const QString &identifier);

    void loadRecentUsed();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    void writeRecentUsed();
    QStringList mUsedIdentifier;
};
