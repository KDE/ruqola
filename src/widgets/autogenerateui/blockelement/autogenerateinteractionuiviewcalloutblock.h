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
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewCalloutBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewCalloutBlock() override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

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
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t);
