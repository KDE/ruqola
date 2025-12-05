/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
class QCheckBox;
// see ./packages/ui-kit/src/blocks/elements/CheckboxElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewCheckboxElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewCheckboxElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewCheckboxElement() override;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewCheckboxElement &other) const;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> initialOptions() const;
    void setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOption);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
    QList<AutoGenerateInteractionUiViewOption *> mInitialOptions;
    QCheckBox *mCheckBox = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t);
