/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentcombobox.h"
#include <KLocalizedString>
ShowAttachmentComboBox::ShowAttachmentComboBox(QWidget *parent)
    : QComboBox(parent)
{
    fillCombobox();
}

ShowAttachmentComboBox::~ShowAttachmentComboBox() = default;

void ShowAttachmentComboBox::fillCombobox()
{
    addItem(i18n("All"), QString());
    addItem(i18n("Video"), QStringLiteral("video"));
    addItem(i18n("Image"), QStringLiteral("image"));
    addItem(i18n("Audio"), QStringLiteral("audio"));
    addItem(i18n("Text"), QStringLiteral("text"));
    addItem(i18n("Files"), QStringLiteral("application"));
}
