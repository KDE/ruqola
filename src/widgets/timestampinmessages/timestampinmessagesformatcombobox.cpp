/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesformatcombobox.h"

TimeStampInMessagesFormatComboBox::TimeStampInMessagesFormatComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
}

TimeStampInMessagesFormatComboBox::~TimeStampInMessagesFormatComboBox() = default;

void TimeStampInMessagesFormatComboBox::initialize()
{
    // TODO
}

#include "moc_timestampinmessagesformatcombobox.cpp"
