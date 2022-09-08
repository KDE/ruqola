/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sharetextinterface.h"
#include "purposemenuwidget.h"

#include <QMenu>

ShareTextInterface::ShareTextInterface(QObject *parent)
    : PluginTextInterface(parent)
    , mPurposeMenuWidget(new PurposeMenuWidget(this))
{
}

ShareTextInterface::~ShareTextInterface() = default;

void ShareTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    menu->addMenu(mPurposeMenuWidget->menu());
}

void ShareTextInterface::setSelectedText(const QString &str)
{
    mPurposeMenuWidget->setSelectedText(str);
}
