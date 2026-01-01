/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewButtonElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
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
    explicit AutoGenerateInteractionUiViewButtonElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewButtonElement() override;

    [[nodiscard]] Style style() const;
    void setStyle(Style newStyle);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewButtonElement &other) const;

    [[nodiscard]] AutoGenerateInteractionUiViewText text() const;
    void setText(const AutoGenerateInteractionUiViewText &newText);

    [[nodiscard]] bool secondary() const;
    void setSecondary(bool newSecondary);

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT AutoGenerateInteractionUiViewButtonElement::Style convertStyleFromString(const QString &str) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString convertStyleToString() const;
    Style mStyle = Style::Unknown;
    AutoGenerateInteractionUiViewText mText;
    QString mValue;
    bool mSecondary = false;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t);
