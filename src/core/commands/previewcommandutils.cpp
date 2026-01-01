/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "previewcommandutils.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
QList<PreviewCommand> PreviewCommandUtils::parsePreviewJson(const QJsonObject &replyObject)
{
    QList<PreviewCommand> commands;
    const QJsonObject previewObj = replyObject["preview"_L1].toObject();
    if (!previewObj.isEmpty()) {
        const QJsonArray items = previewObj["items"_L1].toArray();
        for (const auto &i : items) {
            PreviewCommand command;
            command.parse(i.toObject());
            if (command.isValid()) {
                commands.append(std::move(command));
            }
        }
    }
    return commands;
}
