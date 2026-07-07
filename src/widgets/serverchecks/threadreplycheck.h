/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, replies to it on a thread, and verifies the reply arrives linked to the thread. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadReplyCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    enum class Phase : uint8_t {
        WaitingForParent,
        WaitingForReply,
    };
    Phase mPhase = Phase::WaitingForParent;
    QString mParentText;
    QString mReplyText;
    QByteArray mParentId;
};
