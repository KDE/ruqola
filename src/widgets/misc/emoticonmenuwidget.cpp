/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include "config-ruqola.h"
#include "customemojiiconmanager.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <QVBoxLayout>
#include <TextEmoticonsCore/EmojiModel>
#include <TextEmoticonsCore/EmojiModelManager>
EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : TextEmoticonsWidgets::EmoticonTextEditSelector(parent)
{
    setCustomEmojiSupport(true);
    setAttribute(Qt::WA_DeleteOnClose);
    loadEmoticons();
    const QSize popupMenuSize = QSize(400, 250);
    setFixedSize(popupMenuSize);
    TextEmoticonsCore::EmojiModelManager::self()->emojiModel()->setCustomEmojiIconManager(Ruqola::self()->customEmojiIconManager());
    loadExcludeEmoticons();
}

EmoticonMenuWidget::~EmoticonMenuWidget() = default;

void EmoticonMenuWidget::loadExcludeEmoticons()
{
#if HAVE_TEXTADDONS_TEXTEMOTICON_EXCLUDEEMOTICON_SUPPORT
    const QStringList lst = QStringList({QStringLiteral(":saluting_face:")});
    TextEmoticonsCore::EmojiModelManager::self()->setExcludeEmoticons(lst);
#endif
}

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    Ruqola::self()->customEmojiIconManager()->setCurrentRocketChatAccount(currentRocketChatAccount);
    if (currentRocketChatAccount) {
        TextEmoticonsCore::EmojiModelManager::self()->emojiModel()->setCustomEmojiList(currentRocketChatAccount->customEmojies());
    }
}

QSize EmoticonMenuWidget::sizeHint() const
{
    return minimumSize();
}

#include "moc_emoticonmenuwidget.cpp"
