/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT EmoticonCategory
{
public:
    EmoticonCategory();
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &category);

    Q_REQUIRED_RESULT const QString &i18nName() const;

    void setI18nName(const QString &newI18nName);

private:
    QString mName;
    QString mCategory;
    QString mI18nName;
};
Q_DECLARE_METATYPE(EmoticonCategory)
Q_DECLARE_TYPEINFO(EmoticonCategory, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const EmoticonCategory &t);
