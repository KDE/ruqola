/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidget.h"

VideoConferenceWidget::VideoConferenceWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

VideoConferenceWidget::~VideoConferenceWidget()
{
}

void VideoConferenceWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
