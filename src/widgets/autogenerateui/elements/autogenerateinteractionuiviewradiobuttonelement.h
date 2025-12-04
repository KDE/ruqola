/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
// see ./packages/ui-kit/src/blocks/elements/RadioButtonElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewRadioButtonElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewRadioButtonElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewRadioButtonElement() override;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewRadioButtonElement &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] AutoGenerateInteractionUiViewOption *initialOption() const;
    void setInitialOption(AutoGenerateInteractionUiViewOption *newInitialOption);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
    AutoGenerateInteractionUiViewOption *mInitialOption = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewRadioButtonElement &t);
