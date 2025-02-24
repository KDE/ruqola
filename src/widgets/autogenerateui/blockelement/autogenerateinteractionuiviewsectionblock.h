/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>
class AutoGenerateInteractionUiViewText;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewSectionBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewSectionBlock();
    ~AutoGenerateInteractionUiViewSectionBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) const override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *text() const;
    void setText(AutoGenerateInteractionUiViewText *newText);

    void parse(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewText *mText = nullptr;
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewSectionBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewSectionBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_NO_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t);
