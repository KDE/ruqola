/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewButtonElement : public AutoGenerateInteractionUiViewActionable
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
    ~AutoGenerateInteractionUiViewButtonElement() override;

    [[nodiscard]] Style style() const;
    void setStyle(Style newStyle);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewButtonElement &other) const;

    [[nodiscard]] AutoGenerateInteractionUiViewText text() const;
    void setText(const AutoGenerateInteractionUiViewText &newText);
    void parse(const QJsonObject &json) override;

    [[nodiscard]] bool secondary() const;
    void setSecondary(bool newSecondary);

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

protected:
    void serializeElement(QJsonObject &o) const override;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT AutoGenerateInteractionUiViewButtonElement::Style convertStyleFromString(const QString &str) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString convertStyleToString() const;
    Style mStyle = Style::Unknown;
    AutoGenerateInteractionUiViewText mText;
    bool mSecondary = false;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewButtonElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewButtonElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t);
