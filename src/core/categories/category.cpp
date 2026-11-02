/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "category.h"
#include <QDebug>

QT_IMPL_METATYPE_EXTERN_TAGGED(Category, Ruqola_Category)
using namespace Qt::Literals::StringLiterals;

Category::Category() = default;

QByteArray Category::identifier() const
{
    return mIdentifier;
}

void Category::setIdentifier(const QByteArray &newIdentifier)
{
    mIdentifier = newIdentifier;
}

QString Category::name() const
{
    return mName;
}

void Category::setName(const QString &newName)
{
    mName = newName;
}

bool Category::defaultCategory() const
{
    return mDefaultCategory;
}

void Category::setDefaultCategory(bool newDefaultCategory)
{
    mDefaultCategory = newDefaultCategory;
}

bool Category::showUnreads() const
{
    return mShowUnreads;
}

void Category::setShowUnreads(bool newShowUnreads)
{
    mShowUnreads = newShowUnreads;
}

bool Category::isValid() const
{
    return !mIdentifier.isEmpty();
}

QDebug operator<<(QDebug d, const Category &t)
{
    d.space() << "identifier:" << t.identifier();
    d.space() << "showUnreads:" << t.showUnreads();
    d.space() << "defaultCategory:" << t.defaultCategory();
    d.space() << "name:" << t.name();
    return d;
}
