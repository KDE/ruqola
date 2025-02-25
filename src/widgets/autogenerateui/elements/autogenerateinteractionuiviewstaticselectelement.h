/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewOptionElement;
// see ./packages/ui-kit/src/blocks/elements/StaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewStaticSelectElement();
    ~AutoGenerateInteractionUiViewStaticSelectElement();

    void parse(const QJsonObject &json) override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QString initialValue() const;
    void setInitialValue(const QString &newInitialValue);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOptionElement *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOptionElement *> &newOptions);

private:
    QString mInitialValue;
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QList<AutoGenerateInteractionUiViewOptionElement *> mOptions;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewStaticSelectElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewStaticSelectElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t);
