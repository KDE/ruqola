/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "emojiwidgetaction.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
EmojiWidgetAction::EmojiWidgetAction(const QList<EmojiWidgetAction::EmojiInfo> &emojis, QObject *parent)
    : QWidgetAction{parent}
    , mEmojiWidgetActionWidget(new EmojiWidgetActionWidget(emojis))
{
    setDefaultWidget(mEmojiWidgetActionWidget);
    connect(mEmojiWidgetActionWidget, &EmojiWidgetActionWidget::insertEmoji, this, &EmojiWidgetAction::insertEmoji);
    connect(mEmojiWidgetActionWidget, &EmojiWidgetActionWidget::insertEmojiIdentifier, this, &EmojiWidgetAction::insertEmojiIdentifier);
    connect(mEmojiWidgetActionWidget, &EmojiWidgetActionWidget::selectEmoji, this, &EmojiWidgetAction::selectEmoji);
}

EmojiWidgetAction::~EmojiWidgetAction()
{
    delete mEmojiWidgetActionWidget;
}

EmojiWidgetActionWidget::EmojiWidgetActionWidget(const QList<EmojiWidgetAction::EmojiInfo> &emojis, QWidget *parent)
    : QWidget(parent)
    , mMainLayout(new QHBoxLayout(this))
{
    mMainLayout->setObjectName(u"mainLayout"_s);
    mMainLayout->setContentsMargins({});
    // TODO replace by ktextaddons/emoji font
    constexpr int defaultFontSize{14};
    QFont f;
    f.setPointSize(defaultFontSize);
#ifdef Q_OS_WIN
    f.setFamily(u"Segoe UI Emoji"_s);
#else
    f.setFamily(u"NotoColorEmoji"_s);
#endif
    setFont(f);
    addDefaultEmojis(emojis);
}

EmojiWidgetActionWidget::~EmojiWidgetActionWidget() = default;

void EmojiWidgetActionWidget::addDefaultEmojis(const QList<EmojiWidgetAction::EmojiInfo> &emojis)
{
    for (const auto &emoji : emojis) {
        auto toolButton = new QToolButton(this);
        toolButton->setAutoRaise(true);
        toolButton->setText(emoji.emojiStr);
        mMainLayout->addWidget(toolButton);
        connect(toolButton, &QToolButton::clicked, this, [this, emoji]() {
            Q_EMIT insertEmoji(emoji.emojiStr);
            Q_EMIT insertEmojiIdentifier(emoji.emojiIdentifier);
        });
    }
    auto selectMoreEmojiButton = new QToolButton(this);
    selectMoreEmojiButton->setObjectName(u"selectMoreEmojiButton"_s);
    selectMoreEmojiButton->setIcon(QIcon::fromTheme(u"overflow-menu"_s));
    selectMoreEmojiButton->setAutoRaise(true);
    mMainLayout->addWidget(selectMoreEmojiButton);
    connect(selectMoreEmojiButton, &QToolButton::clicked, this, &EmojiWidgetActionWidget::selectEmoji);

    mMainLayout->addStretch(1);
}

#include "moc_emojiwidgetaction.cpp"
