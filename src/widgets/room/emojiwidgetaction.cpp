/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "emojiwidgetaction.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
EmojiWidgetAction::EmojiWidgetAction(QObject *parent)
    : QWidgetAction{parent}
    , mEmojiWidgetActionWidget(new EmojiWidgetActionWidget())
{
    setDefaultWidget(mEmojiWidgetActionWidget);
    connect(mEmojiWidgetActionWidget, &EmojiWidgetActionWidget::addEmoji, this, &EmojiWidgetAction::addEmoji);
    connect(mEmojiWidgetActionWidget, &EmojiWidgetActionWidget::selectEmoji, this, &EmojiWidgetAction::selectEmoji);
}

EmojiWidgetAction::~EmojiWidgetAction()
{
    delete mEmojiWidgetActionWidget;
}

EmojiWidgetActionWidget::EmojiWidgetActionWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(u"toolButton"_s);
    toolButton->setAutoRaise(true);
    toolButton->setIcon(QIcon::fromTheme(u"list-remove"_s));
    mainLayout->addWidget(toolButton);
}

EmojiWidgetActionWidget::~EmojiWidgetActionWidget() = default;

#include "moc_emojiwidgetaction.cpp"
