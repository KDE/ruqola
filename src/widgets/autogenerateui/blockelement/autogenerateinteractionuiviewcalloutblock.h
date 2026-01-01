/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

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

    void setErrorMessages(const QMap<QString, QString> &map) override;
    void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) override;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const override;

private:
    AutoGenerateInteractionUiViewText *mText = nullptr;
    AutoGenerateInteractionUiViewActionable *mAccessory = nullptr;
    // TODO variant
    // TODO title
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t);
