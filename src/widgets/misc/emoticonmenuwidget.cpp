/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include "customemojiiconmanager.h"
#include "rocketchataccount.h"
#include <QScreen>
#include <QVBoxLayout>
#include <TextEmoticonsCore/EmojiModel>
#include <TextEmoticonsCore/EmojiModelManager>
EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : TextEmoticonsWidgets::EmoticonTextEditSelector(parent)
    , mCustomEmojiIconManager(new CustomEmojiIconManager(this))
{
    setCustomEmojiSupport(true);
    loadEmoticons();
    const QSize popupMenuSize = QSize(400, 250) * screen()->devicePixelRatio();
    setFixedSize(popupMenuSize);
    TextEmoticonsCore::EmojiModelManager::self()->emojiModel()->setCustomEmojiIconManager(mCustomEmojiIconManager);
}

EmoticonMenuWidget::~EmoticonMenuWidget() = default;

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    if (currentRocketChatAccount) {
        mCustomEmojiIconManager->setCurrentRocketChatAccount(currentRocketChatAccount);
        TextEmoticonsCore::EmojiModelManager::self()->emojiModel()->setCustomEmojiList(currentRocketChatAccount->customEmojies());
    }
}

QSize EmoticonMenuWidget::sizeHint() const
{
    return minimumSize();
}
