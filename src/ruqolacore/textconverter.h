/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTCONVERTER_H
#define TEXTCONVERTER_H

#include <QString>
#include <QMap>
#include "libruqola_private_export.h"
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Definition>
class EmojiManager;
class LIBRUQOLACORE_TESTS_EXPORT TextConverter
{
public:
    explicit TextConverter(EmojiManager *emojiManager = nullptr);
    ~TextConverter() = default;

    Q_REQUIRED_RESULT QString convertMessageText(const QString &str, const QMap<QString, QString> &mentions, const QString &userName) const;
private:
    EmojiManager *mEmojiManager = nullptr;
};

#endif // TEXTCONVERTER_H
