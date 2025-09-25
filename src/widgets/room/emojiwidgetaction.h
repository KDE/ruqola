/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
#include <QWidgetAction>

class QHBoxLayout;
class EmojiWidgetActionWidget;
class LIBRUQOLAWIDGETS_EXPORT EmojiWidgetAction : public QWidgetAction
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_EXPORT EmojiInfo {
        QString emojiStr;
        QString emojiIdentifier;
    };

    explicit EmojiWidgetAction(const QList<EmojiWidgetAction::EmojiInfo> &emojis, QObject *parent = nullptr);
    ~EmojiWidgetAction() override;

Q_SIGNALS:
    void insertEmoji(const QString &str);
    void insertEmojiIdentifier(const QString &identifier);
    void selectEmoji();

private:
    EmojiWidgetActionWidget *const mEmojiWidgetActionWidget;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmojiWidgetActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmojiWidgetActionWidget(const QList<EmojiWidgetAction::EmojiInfo> &emojis, QWidget *parent = nullptr);
    ~EmojiWidgetActionWidget() override;

Q_SIGNALS:
    void insertEmoji(const QString &str);
    void insertEmojiIdentifier(const QString &identifier);
    void selectEmoji();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void addDefaultEmojis(const QList<EmojiWidgetAction::EmojiInfo> &emojis);
    QHBoxLayout *const mMainLayout;
};

Q_DECLARE_TYPEINFO(EmojiWidgetAction::EmojiInfo, Q_RELOCATABLE_TYPE);
