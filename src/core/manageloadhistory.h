/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QObject>
class RocketChatAccount;
class MessageModel;
class LIBRUQOLACORE_TESTS_EXPORT ManageLoadHistory : public QObject
{
    Q_OBJECT
public:
    explicit ManageLoadHistory(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageLoadHistory() override;

    void loadHistory(MessageModel *roomModel, const QString &roomID, bool initial, qint64 timeStamp);

private:
    RocketChatAccount *const mAccount;
};
