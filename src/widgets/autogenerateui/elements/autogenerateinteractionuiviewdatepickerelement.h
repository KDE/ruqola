/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class AutoGenerateInteractionUiViewText;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewDatePickerElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewDatePickerElement();
    ~AutoGenerateInteractionUiViewDatePickerElement();

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewDatePickerElement &other) const;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] QString initialDate() const;
    void setInitialDate(const QString &newInitialDate);

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QJsonObject serialize() const override;

private:
    QString mInitialDate;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewDatePickerElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewDatePickerElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDatePickerElement &t);
