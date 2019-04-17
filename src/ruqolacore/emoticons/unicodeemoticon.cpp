/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
#include <QRegularExpression>
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

//Code from fairchat
//Reimplement it ?
QString UnicodeEmoticon::escapeUnicodeEmoji(const QString &pString)
{
    qDebug() << pString;
    static const QRegularExpression reg{ QStringLiteral("(\\b[A-Fa-f0-9]{2,6}\\b)") };
    QRegularExpressionMatchIterator iter = reg.globalMatch(pString);

    QString retString;

    if (pString.contains(QLatin1Char('-'))) {
        const QStringList parts = pString.split(QLatin1Char('-'));

        for (const auto &item : parts) {
            int part;
            std::stringstream ss;
            ss << std::hex << item.toStdString();
            ss >> part;

            if (part >= 0x10000 && part <= 0x10FFFF) {
                const int hi = ((part - 0x10000) / 0x400) + 0xD800;
                const int lo = ((part - 0x10000) % 0x400) + 0xDC00;
                retString += QChar(hi);
                retString += QChar(lo);
            } else {
                retString = QChar(part);
            }
        }
    } else {
        int part;
        std::stringstream ss;
        ss << std::hex << pString.toStdString();
        ss >> part;

        if (part >= 0x10000 && part <= 0x10FFFF) {
            const int hi = ((part - 0x10000) / 0x400) + 0xD800;
            const int lo = ((part - 0x10000) % 0x400) + 0xDC00;
            retString += QChar(hi);
            retString += QChar(lo);
        } else {
            retString = QChar(part);
        }
    }

    return retString;
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
    d << "Name : " << t.identifier();
    d << "Unicode: " << t.unicode();
    d << "Category: " << t.category();
    d << "Aliases: " << t.aliases();
    return d;
}
