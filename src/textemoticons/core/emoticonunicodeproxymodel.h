/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "textemoticonscore_export.h"
#include <QSortFilterProxyModel>
namespace TextEmoticonsCore
{
class TEXTEMOTICONSCORE_EXPORT EmoticonUnicodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeProxyModel(QObject *parent = nullptr);
    ~EmoticonUnicodeProxyModel() override;
    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &newCategories);

    Q_REQUIRED_RESULT QStringList recentEmoticons() const;
    void setRecentEmoticons(const QStringList &newRecentEmoticons);

    Q_REQUIRED_RESULT QString searchIdentifier() const;
    void setSearchIdentifier(const QString &newSearchIdentifier);

protected:
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    void clearSearch();
    QString mCategory;
    QStringList mRecentEmoticons;
    QString mSearchIdentifier;
};
}
