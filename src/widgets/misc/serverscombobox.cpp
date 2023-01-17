/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverscombobox.h"
#include "accountmanager.h"
#include "ruqola.h"
ServersComboBox::ServersComboBox(QWidget *parent)
    : QComboBox(parent)
    , mModel(Ruqola::self()->accountManager()->rocketChatAccountModel())
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setModel(mModel);
}

ServersComboBox::~ServersComboBox() = default;
