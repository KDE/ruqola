/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Changes the channel topic and verifies the new topic arrives on the room, then restores it. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT TopicChangeCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void sendTopic(const QString &topic);
    QString mOriginalTopic;
    QString mNewTopic;
};
