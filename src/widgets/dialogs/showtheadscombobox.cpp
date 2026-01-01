/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showtheadscombobox.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

ShowTheadsComboBox::ShowTheadsComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("All"), ShowTheadsComboBox::ThreadType::All);
    addItem(i18n("Following"), ShowTheadsComboBox::ThreadType::Following);
    addItem(i18n("Unread"), ShowTheadsComboBox::ThreadType::Unread);
}

ShowTheadsComboBox::~ShowTheadsComboBox() = default;

ShowTheadsComboBox::ThreadType ShowTheadsComboBox::currentThreadType() const
{
    return currentData().value<ShowTheadsComboBox::ThreadType>();
}

#include "moc_showtheadscombobox.cpp"
