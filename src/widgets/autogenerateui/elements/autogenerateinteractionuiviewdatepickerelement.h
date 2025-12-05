/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
// ./packages/ui-kit/src/blocks/elements/DatePickerElement.ts
class AutoGenerateInteractionUiViewText;
class QDateEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewDatePickerElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewDatePickerElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewDatePickerElement() override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewDatePickerElement &other) const;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] QString initialDate() const;
    void setInitialDate(const QString &newInitialDate);

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    QString mInitialDate;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QDateEdit *mDateTime = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDatePickerElement &t);
