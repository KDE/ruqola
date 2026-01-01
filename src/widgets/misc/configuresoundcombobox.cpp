/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuresoundcombobox.h"
#include "model/notificationdesktopsoundpreferencemodel.h"

ConfigureSoundComboBox::ConfigureSoundComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

ConfigureSoundComboBox::~ConfigureSoundComboBox() = default;

QByteArray ConfigureSoundComboBox::identifier() const
{
    return currentData(NotificationDesktopSoundPreferenceModel::NotificationPreference).toByteArray();
}

void ConfigureSoundComboBox::setCurrentSoundName(const QByteArray &identifier)
{
    setCurrentIndex(findData(identifier, NotificationDesktopSoundPreferenceModel::NotificationPreference));
}

#include "moc_configuresoundcombobox.cpp"
