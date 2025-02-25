/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewactionable.h"

class AutoGenerateInteractionUiViewOverflowElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewOverflowElement();
    ~AutoGenerateInteractionUiViewOverflowElement();
    void parse(const QJsonObject &json) override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
};
