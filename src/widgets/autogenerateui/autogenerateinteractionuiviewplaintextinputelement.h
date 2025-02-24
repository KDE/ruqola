/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewactionable.h"

// see ./packages/ui-kit/src/blocks/elements/PlainTextInputElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewPlainTextInputElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewPlainTextInputElement();
    ~AutoGenerateInteractionUiViewPlainTextInputElement() override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] QString placeHolder() const;
    void setPlaceHolder(const QString &newPlaceHolder);

    [[nodiscard]] QString initialValue() const;
    void setInitialValue(const QString &newInitialValue);

    [[nodiscard]] bool multiLine() const;
    void setMultiLine(bool newMultiLine);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewPlainTextInputElement &other) const;

    [[nodiscard]] int minLength() const;
    void setMinLength(int newMinLength);

    [[nodiscard]] int maxLength() const;
    void setMaxLength(int newMaxLength);

private:
    QString mPlaceHolder;
    QString mInitialValue;
    bool mMultiLine = false;
    int mMinLength = -1;
    int mMaxLength = -1;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewPlainTextInputElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewPlainTextInputElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewPlainTextInputElement &t);
