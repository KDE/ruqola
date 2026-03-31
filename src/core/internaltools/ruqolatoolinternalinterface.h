/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <TextAutoGenerateText/TextAutoGenerateTextToolInternalInterface>

class LIBRUQOLACORE_EXPORT RuqolaToolInternalInterface : public TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface
{
    Q_OBJECT
public:
    explicit RuqolaToolInternalInterface(QObject *parent = nullptr);
    ~RuqolaToolInternalInterface() override;

    [[nodiscard]] TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *callTool(const QByteArray &toolName) override;
};
