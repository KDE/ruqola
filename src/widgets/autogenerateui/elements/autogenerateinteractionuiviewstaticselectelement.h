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
// see ./packages/ui-kit/src/blocks/elements/StaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewStaticSelectElement();
    ~AutoGenerateInteractionUiViewStaticSelectElement() override;

    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QString initialValue() const;
    void setInitialValue(const QString &newInitialValue);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    QString mInitialValue;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewStaticSelectElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewStaticSelectElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t);
