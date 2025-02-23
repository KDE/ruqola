/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqola_private_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewText
{
public:
    enum class TextType : uint8_t {
        PlainText,
        Markdown,
    };
    // PlainText/Mardown?
    AutoGenerateInteractionUiViewText();
    ~AutoGenerateInteractionUiViewText();
    void parse(const QJsonObject &json);

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewText &other) const;
    [[nodiscard]] bool emoji() const;
    void setEmoji(bool newEmoji);

private:
    QString mText;
    bool mEmoji = false;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewText)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewText, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewText &t);
