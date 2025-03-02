/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewActionable;
// ./src/blocks/layout/CalloutBlock.ts
class AutoGenerateInteractionUiViewCalloutBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewCalloutBlock();
    ~AutoGenerateInteractionUiViewCalloutBlock() override;
    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) const override;

    [[nodiscard]] AutoGenerateInteractionUiViewText *text() const;
    void setText(AutoGenerateInteractionUiViewText *newText);

    [[nodiscard]] AutoGenerateInteractionUiViewActionable *accessory() const;
    void setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory);

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewText *mText = nullptr;
    AutoGenerateInteractionUiViewActionable *mAccessory = nullptr;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewCalloutBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewCalloutBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t);
