/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "commonmessagesmodel.h"
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT ModerationMessagesModel : public CommonMessagesModel
{
    Q_OBJECT
public:
    explicit ModerationMessagesModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~ModerationMessagesModel() override;

protected:
    [[nodiscard]] QVector<Message> extractMessages(const QJsonObject &obj) override;
};
