/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateinteractionuiviewelement.h"
#include "libruqolawidgets_private_export.h"
#include <QMetaType>

class QJsonObject;

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewText : public AutoGenerateInteractionUiViewElement
{
    Q_GADGET
public:
    enum class TextType : uint8_t {
        Unknown,
        PlainText,
        Markdown,
    };
    Q_ENUM(TextType)
    // PlainText/Mardown?
    AutoGenerateInteractionUiViewText();
    ~AutoGenerateInteractionUiViewText() override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewText &other) const;
    [[nodiscard]] bool emoji() const;
    void setEmoji(bool newEmoji);

    [[nodiscard]] TextType type() const;
    void setType(TextType newType);
    [[nodiscard]] QString generateText(bool forcePlainText = false) const;
    [[nodiscard]] QJsonObject serialize() const override;

    [[nodiscard]] bool isEmpty() const;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT AutoGenerateInteractionUiViewText::TextType convertTypeFromString(const QString &str) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString convertTextTypeToString() const;
    TextType mType = TextType::Unknown;
    QString mText;
    bool mEmoji = false;
};
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewText, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewText &t);
