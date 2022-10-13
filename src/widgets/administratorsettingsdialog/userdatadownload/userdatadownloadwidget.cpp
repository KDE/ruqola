/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

UserDataDownloadWidget::UserDataDownloadWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

UserDataDownloadWidget::~UserDataDownloadWidget() = default;

void UserDataDownloadWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
