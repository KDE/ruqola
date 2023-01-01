/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT UnicodeEmoticon
{
    Q_GADGET
public:
    UnicodeEmoticon();

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString unicode() const;
    Q_REQUIRED_RESULT QString unicodeDisplay() const;
    void setUnicode(const QString &unicode);

    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &category);

    Q_REQUIRED_RESULT QStringList aliases() const;
    void setAliases(const QStringList &aliases);

    Q_REQUIRED_RESULT bool hasEmoji(const QString &identifier) const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT int order() const;
    void setOrder(int order);

    Q_REQUIRED_RESULT QString key() const;
    void setKey(const QString &key);

private:
    Q_REQUIRED_RESULT QString escapeUnicodeEmoji(const QString &pString);
    QStringList mAliases;
    QString mIdentifier;
    QString mUnicode;
    QString mCategory;
    QString mKey;
    mutable QString mCachedHtml;
    int mOrder = -1;
};
Q_DECLARE_METATYPE(UnicodeEmoticon)
Q_DECLARE_TYPEINFO(UnicodeEmoticon, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const UnicodeEmoticon &t);
