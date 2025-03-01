/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
// see ./packages/ui-kit/src/blocks/elements/CheckboxElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewCheckboxElement : public AutoGenerateInteractionUiViewActionable
{
    Q_GADGET
public:
    AutoGenerateInteractionUiViewCheckboxElement();
    ~AutoGenerateInteractionUiViewCheckboxElement() override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewCheckboxElement &other) const;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

protected:
    void serializeElement(QJsonObject &o) const override;

private:
    // TODO initialOptions
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewCheckboxElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewCheckboxElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t);
