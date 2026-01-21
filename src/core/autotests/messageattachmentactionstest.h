/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class MessageAttachmentActionsTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageAttachmentActionsTest(QObject *parent = nullptr);
    ~MessageAttachmentActionsTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldParseAlignment();
};
