/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessagesModelTest : public QObject
{
    Q_OBJECT
public:
    explicit MessagesModelTest(QObject *parent = nullptr);
    ~MessagesModelTest() override = default;

private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValue();
    void shouldAddMessage();
    void shouldRemoveMessage();
    void shouldRemoveNotExistingMessage();
    void shouldDetectDateChange();
    void shouldAddMessages();
    void shouldUpdateFirstMessage();
    void shouldAllowEditing();
    void shouldFindPrevNextMessage();
};
