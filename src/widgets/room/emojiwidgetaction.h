/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
#include <QWidgetAction>

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmojiWidgetActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmojiWidgetActionWidget(QWidget *parent = nullptr);
    ~EmojiWidgetActionWidget() override;

Q_SIGNALS:
    void insertEmoji(const QString &str);
    void insertEmojiIdentifier(const QString &identifier);
    void selectEmoji();
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmojiWidgetAction : public QWidgetAction
{
    Q_OBJECT
public:
    struct EmojiInfo {
        QString emojiStr;
        QString emojiIdentifier;
    };

    explicit EmojiWidgetAction(QObject *parent = nullptr);
    ~EmojiWidgetAction() override;

    void addDefaultEmojis(const QList<EmojiInfo> &emojis);

Q_SIGNALS:
    void insertEmoji(const QString &str);
    void insertEmojiIdentifier(const QString &identifier);
    void selectEmoji();

private:
    EmojiWidgetActionWidget *const mEmojiWidgetActionWidget;
};
Q_DECLARE_TYPEINFO(EmojiWidgetAction::EmojiInfo, Q_RELOCATABLE_TYPE);
