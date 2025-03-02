/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class AutoGenerateInteractionUiViewText;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewTimePickerElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewTimePickerElement();
    ~AutoGenerateInteractionUiViewTimePickerElement() override;

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewTimePickerElement &other) const;

    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) override;
    [[nodiscard]] QString initialTime() const;
    void setInitialTime(const QString &newInitialDate);

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

protected:
    void parseElement(const QJsonObject &json) override;
    void serializeElement(QJsonObject &o) const override;

private:
    QString mInitialTime;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewTimePickerElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewTimePickerElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewTimePickerElement &t);
