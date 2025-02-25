/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
// see ./packages/ui-kit/src/blocks/elements/MultiStaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewMultiStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewMultiStaticSelectElement();
    ~AutoGenerateInteractionUiViewMultiStaticSelectElement();

    void parse(const QJsonObject &json) override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewMultiStaticSelectElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewMultiStaticSelectElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t);
