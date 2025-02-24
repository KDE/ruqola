/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/blockelement/autogenerateinteractionuiviewblockbase.h"
#include "libruqola_private_export.h"
#include <QJsonArray>
#include <QList>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewBlocks
{
public:
    AutoGenerateInteractionUiViewBlocks();
    ~AutoGenerateInteractionUiViewBlocks();
    void parse(const QJsonArray &array);
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewBlocks &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase> blockElements() const;
    void setBlockElements(const QList<AutoGenerateInteractionUiViewBlockBase> &newBlockElements);

private:
    QList<AutoGenerateInteractionUiViewBlockBase> mBlockElements;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewBlocks)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewBlocks, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t);
