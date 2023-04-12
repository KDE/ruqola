/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textemoticonscore_export.h"
#include <QDebug>
#include <QString>
namespace TextEmoticonsCore
{
/**
 * @brief The UnicodeEmoticon class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEMOTICONSCORE_EXPORT CustomEmoji
{
    Q_GADGET
public:
    CustomEmoji();

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT QString category() const;

private:
    QString mIdentifier;
};
}
Q_DECLARE_METATYPE(TextEmoticonsCore::CustomEmoji)
Q_DECLARE_TYPEINFO(TextEmoticonsCore::CustomEmoji, Q_MOVABLE_TYPE);
TEXTEMOTICONSCORE_EXPORT QDebug operator<<(QDebug d, const TextEmoticonsCore::CustomEmoji &t);
