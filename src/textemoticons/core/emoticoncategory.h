/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textemoticonscore_export.h"
#include <QDebug>
#include <QString>
namespace TextEmoticonsCore
{
/**
 * @brief The EmoticonCategory class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEMOTICONSCORE_EXPORT EmoticonCategory
{
public:
    EmoticonCategory();
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &category);

    Q_REQUIRED_RESULT const QString &i18nName() const;

    void setI18nName(const QString &newI18nName);

    Q_REQUIRED_RESULT bool operator<(const EmoticonCategory &other) const;

    Q_REQUIRED_RESULT int order() const;
    void setOrder(int newOrder);

private:
    QString mName;
    QString mCategory;
    QString mI18nName;
    int mOrder = 20;
};
}
Q_DECLARE_METATYPE(TextEmoticonsCore::EmoticonCategory)
Q_DECLARE_TYPEINFO(TextEmoticonsCore::EmoticonCategory, Q_MOVABLE_TYPE);
TEXTEMOTICONSCORE_EXPORT QDebug operator<<(QDebug d, const TextEmoticonsCore::EmoticonCategory &t);
