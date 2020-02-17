/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "emoticons/unicodeemoticon.h"
#include <sstream>
#include <QString>

UnicodeEmoticon::UnicodeEmoticon()
{
}

bool UnicodeEmoticon::isValid() const
{
    return !mIdentifier.isEmpty() && !mUnicode.isEmpty();
}

QString UnicodeEmoticon::identifier() const
{
    return mIdentifier;
}

void UnicodeEmoticon::setIdentifier(const QString &name)
{
    mIdentifier = name;
}

QString UnicodeEmoticon::unicode() const
{
    return mUnicode;
}

QString UnicodeEmoticon::unicodeDisplay() const
{
    if (!mUnicode.isEmpty()) {
        if (mCachedHtml.isEmpty()) {
            mCachedHtml = QStringLiteral("<font face=\"NotoColorEmoji\" size=\"20\">%1</font>").arg(mUnicode);
        }
    }
    return mCachedHtml;
}

QString UnicodeEmoticon::escapeUnicodeEmoji(const QString &pString)
{
    QString retString;

    const QStringList parts = pString.split(QLatin1Char('-'));
    for (const auto &item : parts) {
        bool ok;
        const int part = item.toInt(&ok, 16);
        Q_ASSERT(ok);
        if (QChar::requiresSurrogates(part)) {
            retString += QChar::highSurrogate(part);
            retString += QChar::lowSurrogate(part);
        } else {
            retString = QChar(part);
        }
    }

    return retString;
}

QString UnicodeEmoticon::key() const
{
    return mKey;
}

void UnicodeEmoticon::setKey(const QString &key)
{
    mKey = key;
}

int UnicodeEmoticon::order() const
{
    return mOrder;
}

void UnicodeEmoticon::setOrder(int order)
{
    mOrder = order;
}

void UnicodeEmoticon::setUnicode(const QString &unicode)
{
    mUnicode = escapeUnicodeEmoji(unicode);
}

QString UnicodeEmoticon::category() const
{
    return mCategory;
}

void UnicodeEmoticon::setCategory(const QString &category)
{
    mCategory = category;
}

QStringList UnicodeEmoticon::aliases() const
{
    return mAliases;
}

void UnicodeEmoticon::setAliases(const QStringList &aliases)
{
    mAliases = aliases;
}

bool UnicodeEmoticon::hasEmoji(const QString &identifier) const
{
    return (mIdentifier == identifier) || mAliases.contains(identifier);
}

QDebug operator <<(QDebug d, const UnicodeEmoticon &t)
{
    d << "Identifier : " << t.identifier();
    d << "Unicode: " << t.unicode();
    d << "Category: " << t.category();
    d << "Aliases: " << t.aliases();
    d << "Order: " << t.order();
    d << "Key:" << t.key();
    return d;
}
