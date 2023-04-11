/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include <QScreen>
#include <QVBoxLayout>
EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : TextEmoticonsWidgets::EmoticonTextEditSelector(parent)
{
    loadEmoticons();
    const QSize popupMenuSize = QSize(400, 250) * screen()->devicePixelRatio();
    setFixedSize(popupMenuSize);
}

EmoticonMenuWidget::~EmoticonMenuWidget() = default;

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    // TODO use custom emoji
}

QSize EmoticonMenuWidget::sizeHint() const
{
    return minimumSize();
}
