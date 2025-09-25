/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class EmojiWidgetActionTest : public QObject
{
    Q_OBJECT
public:
    explicit EmojiWidgetActionTest(QObject *parent = nullptr);
    ~EmojiWidgetActionTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValuesEmojiWidgetActionWidget();
};
