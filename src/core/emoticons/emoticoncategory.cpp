/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticons/emoticoncategory.h"

EmoticonCategory::EmoticonCategory() = default;

QString EmoticonCategory::name() const
{
    return mName;
}

void EmoticonCategory::setName(const QString &name)
{
    mName = name;
}

QString EmoticonCategory::category() const
{
    return mCategory;
}

void EmoticonCategory::setCategory(const QString &category)
{
    mCategory = category;
}

const QString &EmoticonCategory::i18nName() const
{
    return mI18nName;
}

void EmoticonCategory::setI18nName(const QString &newI18nName)
{
    mI18nName = newI18nName;
}

QDebug operator<<(QDebug d, const EmoticonCategory &t)
{
    d << "Name " << t.name();
    d << "Category " << t.category();
    d << "Category i18n" << t.i18nName();
    return d;
}
