/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QStandardItemModel>

class TextSelectionTest : public QObject
{
    Q_OBJECT
public:
    explicit TextSelectionTest(QObject *parent = nullptr);
    ~TextSelectionTest() override = default;

private Q_SLOTS:
    void initTestCase();
    void testChangingSelection();
    void testSingleLineReverseSelection();
    void testSelectWordUnderCursor();
    void shouldHaveDefaultValues();

private:
    QStandardItemModel model;
};
