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
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewToggleSwitchElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewToggleSwitchElement() override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);
    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewToggleSwitchElement &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> initialOptions() const;
    void setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOptions);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void parseElement(const QJsonObject &json) override;
    void serializeElement(QJsonObject &o) const override;

private:
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
    QList<AutoGenerateInteractionUiViewOption *> mInitialOptions;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t);
