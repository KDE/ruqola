/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "commands/previewcommand.h"
#include "libruqolacore_export.h"
#include <QList>
class QJsonObject;

namespace PreviewCommandUtils
{
[[nodiscard]] LIBRUQOLACORE_EXPORT QList<PreviewCommand> parsePreviewJson(const QJsonObject &replyObject);
};
