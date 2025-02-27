/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>
// packages/ui-kit/src/blocks/layout/SectionBlock.ts:14
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewActionable;
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

    [[nodiscard]] AutoGenerateInteractionUiViewActionable *accessory() const;
    void setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory);
    [[nodiscard]] QJsonObject serialize() const override;

private:
    AutoGenerateInteractionUiViewText *mText = nullptr;
    AutoGenerateInteractionUiViewActionable *mAccessory = nullptr;
    /*
        fields?: readonly TextObject[];
    */
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewSectionBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewSectionBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t);
