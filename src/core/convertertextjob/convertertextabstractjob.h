/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT ConverterTextAbstractJob : public QObject
{
    Q_OBJECT
public:
    explicit ConverterTextAbstractJob(RocketChatAccount *account, QObject *parent = nullptr);
    ~ConverterTextAbstractJob() override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    virtual void start() = 0;

    [[nodiscard]] RocketChatAccount *rocketChatAcount() const;

protected:
    QString mMessageId;
    RocketChatAccount *const mRocketChatAcount;
};
