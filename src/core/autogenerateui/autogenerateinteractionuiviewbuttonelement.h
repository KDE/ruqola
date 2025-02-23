/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateinteractionuiviewtext.h"
#include "libruqola_private_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewButtonElement
{
    Q_GADGET
public:
    enum class Style : uint8_t {
        Unknown,
        Primary,
        Secondary,
        Danger,
        Warning,
        Success,
    };
    Q_ENUM(Style)
    AutoGenerateInteractionUiViewButtonElement();
    ~AutoGenerateInteractionUiViewButtonElement();

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] Style style() const;
    void setStyle(Style newStyle);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewButtonElement &other) const;

    [[nodiscard]] AutoGenerateInteractionUiViewText text() const;
    void setText(const AutoGenerateInteractionUiViewText &newText);
    void parse(const QJsonObject &json);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT AutoGenerateInteractionUiViewButtonElement::Style convertStyleFromString(const QString &str) const;
    Style mStyle = Style::Unknown;
    AutoGenerateInteractionUiViewText mText;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewButtonElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewButtonElement, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t);
