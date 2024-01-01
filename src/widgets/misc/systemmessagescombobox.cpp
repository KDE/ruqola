/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagescombobox.h"
#include "model/systemmessagesmodel.h"

SystemMessagesComboBox::SystemMessagesComboBox(QWidget *parent)
    : QComboBox(parent)
    , mSystemMessagesModel(new SystemMessagesModel(this))
{
    setModel(mSystemMessagesModel);
    connect(mSystemMessagesModel, &SystemMessagesModel::dataChanged, this, &SystemMessagesComboBox::settingsChanged);
}

SystemMessagesComboBox::~SystemMessagesComboBox() = default;

void SystemMessagesComboBox::setMessagesSystem(const QStringList &lst)
{
    mSystemMessagesModel->setMessagesSystem(lst);
}

QStringList SystemMessagesComboBox::systemMessagesSelected() const
{
    return mSystemMessagesModel->systemMessagesSelected();
}

#include "moc_systemmessagescombobox.cpp"
