/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentcombobox.h"
using namespace Qt::Literals::StringLiterals;

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
    addItem(i18n("Videos"), u"video"_s);
    addItem(i18n("Images"), u"image"_s);
    addItem(i18n("Audios"), u"audio"_s);
    addItem(i18n("Texts"), u"text"_s);
    addItem(i18n("Files"), u"application"_s);
}

#include "moc_showattachmentcombobox.cpp"
