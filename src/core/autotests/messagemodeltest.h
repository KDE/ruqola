/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageModelTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageModelTest(QObject *parent = nullptr);
    ~MessageModelTest() override = default;

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

