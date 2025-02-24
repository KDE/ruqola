/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewDividerBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewDividerBlock();
    ~AutoGenerateInteractionUiViewDividerBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) const override;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewDividerBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewDividerBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t);
