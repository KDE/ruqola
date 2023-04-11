/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include <QVBoxLayout>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : TextEmoticonsWidgets::EmoticonTextEditSelector(parent)
{
    // TODO don't load all the time
    loadEmoticons();
}

EmoticonMenuWidget::~EmoticonMenuWidget() = default;
