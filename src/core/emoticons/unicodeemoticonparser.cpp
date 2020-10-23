/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "unicodeemoticonparser.h"
#include <QJsonObject>
#include <QJsonArray>

#include <algorithm>

UnicodeEmoticonParser::UnicodeEmoticonParser()
{
}

UnicodeEmoticonParser::~UnicodeEmoticonParser()
{
}

QVector<UnicodeEmoticon> UnicodeEmoticonParser::parse(const QJsonObject &o) const
{
    auto aliases = [](const QJsonArray &alternates, const QJsonArray &ascii)
    {
        QStringList ret;
        ret.reserve(alternates.size() + ascii.size());
        auto convert = [&](const QJsonArray &array) {
            auto toString = [](const QJsonValue &value) { return value.toString(); };
            std::transform(array.begin(), array.end(),
                           std::back_inserter(ret), toString);
        };
        convert(alternates);
        convert(ascii);
        return ret;
    };

    QVector<UnicodeEmoticon> lstEmoticons;
    const QStringList keys = o.keys();
    for (const QString &key : keys) {
        UnicodeEmoticon emoticon;
        const QJsonObject emojiObj = o[key].toObject();
        emoticon.setKey(key);
        const QString unicodeStr = emojiObj[QStringLiteral("code_points")].toObject()[QStringLiteral("fully_qualified")].toString();
        Q_ASSERT(!unicodeStr.isEmpty());
        emoticon.setUnicode(unicodeStr);
        const QString category = emojiObj[QStringLiteral("category")].toString();
        emoticon.setCategory(category);
        emoticon.setIdentifier(emojiObj[QStringLiteral("shortname")].toString());
        emoticon.setOrder(emojiObj[QStringLiteral("order")].toInt());
        const auto shortnameAlternates = emojiObj[QStringLiteral("shortname_alternates")].toArray();
        const auto ascii = emojiObj[QStringLiteral("ascii")].toArray();
        emoticon.setAliases(aliases(shortnameAlternates, ascii));
        if (emoticon.isValid()) {
            lstEmoticons.append(std::move(emoticon));
        }
    }
    auto compareOrder = [](const UnicodeEmoticon &left, const UnicodeEmoticon &right) {
                            return left.order() < right.order();
                        };
    std::sort(lstEmoticons.begin(), lstEmoticons.end(), compareOrder);
    return lstEmoticons;
}
