/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewOption;
class QComboBox;
class ActionElementWidget;
class QJsonObject;

// see ./packages/ui-kit/src/blocks/elements/StaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewStaticSelectElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewStaticSelectElement() override;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QString initialValue() const;
    void setInitialValue(const QString &newInitialValue);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    QString mInitialValue;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
    QComboBox *mComboBox = nullptr;
    // TODO optionGroups
    // TODO initialOption
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t);
