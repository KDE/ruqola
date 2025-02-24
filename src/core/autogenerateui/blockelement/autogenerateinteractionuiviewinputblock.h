/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "libruqola_private_export.h"
#include <QDebug>

// see ./packages/ui-kit/src/blocks/layout/InputBlock.ts
class AutoGenerateInteractionUiViewActionable;
class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewInputBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewInputBlock();
    ~AutoGenerateInteractionUiViewInputBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewInputBlock &other) const;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] bool optional() const;
    void setOptional(bool newOptional);

    [[nodiscard]] AutoGenerateInteractionUiViewText label() const;
    void setLabel(const AutoGenerateInteractionUiViewText &newLabel);

private:
    AutoGenerateInteractionUiViewText mLabel;
    AutoGenerateInteractionUiViewActionable *mElement = nullptr;
    bool mOptional = false;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewInputBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewInputBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t);
