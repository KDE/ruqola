/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ruqolatoolinternalinterface.h"

RuqolaToolInternalInterface::RuqolaToolInternalInterface(QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface{parent}
{
}

RuqolaToolInternalInterface::~RuqolaToolInternalInterface() = default;

TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *RuqolaToolInternalInterface::callTool(const QByteArray &toolName)
{
    Q_UNUSED(toolName)
    return {};
}
