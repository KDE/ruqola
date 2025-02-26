/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"

class AutoGenerateInteractionUiViewCalloutBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewCalloutBlock();
    ~AutoGenerateInteractionUiViewCalloutBlock();
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) const override;
    void parse(const QJsonObject &json) override;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewCalloutBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewCalloutBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t);
