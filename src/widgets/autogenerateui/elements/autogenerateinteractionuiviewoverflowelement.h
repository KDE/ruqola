/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
class AutoGenerateInteractionUiViewOption;
// ./packages/ui-kit/src/blocks/elements/OverflowElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewOverflowElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewOverflowElement();
    ~AutoGenerateInteractionUiViewOverflowElement();
    void parse(const QJsonObject &json) override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewOverflowElement &other) const;

private:
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewOverflowElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewOverflowElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOverflowElement &t);
