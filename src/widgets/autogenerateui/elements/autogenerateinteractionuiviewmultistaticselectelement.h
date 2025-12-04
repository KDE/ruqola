/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewOption;
class MultiStaticSelectWidget;
// see ./packages/ui-kit/src/blocks/elements/MultiStaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewMultiStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewMultiStaticSelectElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewMultiStaticSelectElement() override;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewMultiStaticSelectElement &other) const;

    [[nodiscard]] QStringList initialValue() const;
    void setInitialValue(const QStringList &newInitialValue);

    [[nodiscard]] int maxSelectItems() const;
    void setMaxSelectItems(int newMaxSelectItems);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> initialOptions() const;
    void setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOptions);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    MultiStaticSelectWidget *mMultiStaticSelectWidget = nullptr;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
    QList<AutoGenerateInteractionUiViewOption *> mInitialOptions;
    QStringList mInitialValue;
    int mMaxSelectItems = -1;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t);
