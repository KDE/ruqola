/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"

class AutoGenerateInteractionUiViewText;
class QPlainTextEdit;
// see ./packages/ui-kit/src/blocks/elements/PlainTextInputElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewUserSelectElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewUserSelectElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewUserSelectElement() override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewUserSelectElement &other) const;

protected:
    void parseElement(const QJsonObject &json) override;
    void serializeElement(QJsonObject &o) const override;

private:
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QPlainTextEdit *mPlainTextEdit = nullptr;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewUserSelectElement)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewUserSelectElement &t);
