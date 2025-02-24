/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateinteractionuiviewactionable.h"
#include "autogenerateinteractionuiviewoptionelement.h"
#include "libruqola_private_export.h"
#include <QJsonObject>
// see ./packages/ui-kit/src/blocks/elements/RadioButtonElement.ts
class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewRadioButtonElement : public AutoGenerateInteractionUiViewActionable
{
    Q_GADGET
public:
    AutoGenerateInteractionUiViewRadioButtonElement();
    ~AutoGenerateInteractionUiViewRadioButtonElement() override;

    void parse(const QJsonObject &json) override;

private:
    // TODO initialOptions
    QList<AutoGenerateInteractionUiViewOptionElement> mOptions;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewRadioButtonElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewRadioButtonElement, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewRadioButtonElement &t);
