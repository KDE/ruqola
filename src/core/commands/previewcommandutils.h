/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "commands/previewcommand.h"
#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QList>
namespace PreviewCommandUtils
{
[[nodiscard]] LIBRUQOLACORE_EXPORT QList<PreviewCommand> parsePreviewJson(const QJsonObject &replyObject);
};
