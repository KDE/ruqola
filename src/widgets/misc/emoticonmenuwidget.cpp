/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include "customemojiiconmanager.h"
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
        // TODO use custom emoji
    }
}

QSize EmoticonMenuWidget::sizeHint() const
{
    return minimumSize();
}
