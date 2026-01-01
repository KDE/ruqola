/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
// packages/ui-kit/src/blocks/elements/TimePickerElement.ts
class AutoGenerateInteractionUiViewText;
class QTimeEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewTimePickerElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewTimePickerElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewTimePickerElement() override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewTimePickerElement &other) const;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] QString initialTime() const;
    void setInitialTime(const QString &newInitialDate);

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

protected:
    void parseElement(const QJsonObject &json) override;
    void serializeElement(QJsonObject &o) const override;

private:
    QString mInitialTime;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QTimeEdit *mTimeEdit = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewTimePickerElement &t);
