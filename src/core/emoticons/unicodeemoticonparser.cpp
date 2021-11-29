/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "unicodeemoticonparser.h"
#include <QJsonArray>
#include <QJsonObject>

#include <algorithm>

UnicodeEmoticonParser::UnicodeEmoticonParser() = default;

UnicodeEmoticonParser::~UnicodeEmoticonParser() = default;

QVector<UnicodeEmoticon> UnicodeEmoticonParser::parse(const QJsonObject &o) const
{
    auto aliases = [](const QJsonArray &alternates, const QJsonArray &ascii) {
        QStringList ret;
        ret.reserve(alternates.size() + ascii.size());
        auto convert = [&](const QJsonArray &array) {
            auto toString = [](const QJsonValue &value) {
                return value.toString();
            };
            std::transform(array.begin(), array.end(), std::back_inserter(ret), toString);
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
