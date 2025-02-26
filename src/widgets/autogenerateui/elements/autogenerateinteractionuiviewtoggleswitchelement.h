/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
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

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption> &newOptions);
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewToggleSwitchElement &other) const;

private:
    // TODO initialOptions
    QList<AutoGenerateInteractionUiViewOption> mOptions;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewToggleSwitchElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewToggleSwitchElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t);
