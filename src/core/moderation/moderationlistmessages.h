/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "listmessages.h"

class LIBRUQOLACORE_TESTS_EXPORT ModerationListMessages : public ListMessages
{
public:
    ModerationListMessages();
    ~ModerationListMessages() override;

protected:
    void parseMessagesList(const QJsonObject &messagesObj, const QString &arrayName) override;
};
