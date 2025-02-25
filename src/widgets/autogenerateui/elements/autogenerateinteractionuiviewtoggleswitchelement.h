/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoptionelement.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

// see packages/ui-kit/src/blocks/elements/ToggleSwitchElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewToggleSwitchElement : public AutoGenerateInteractionUiViewActionable
{
    Q_GADGET
public:
    AutoGenerateInteractionUiViewToggleSwitchElement();
    ~AutoGenerateInteractionUiViewToggleSwitchElement() override;

    void parse(const QJsonObject &json) override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOptionElement> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOptionElement> &newOptions);
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

private:
    // TODO initialOptions
    QList<AutoGenerateInteractionUiViewOptionElement> mOptions;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewToggleSwitchElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewToggleSwitchElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t);
