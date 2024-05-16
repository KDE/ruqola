/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpreferenceconfiguresoundcombobox.h"
#include "model/notificationdesktopsoundpreferencemodel.h"

MyAccountPreferenceConfigureSoundComboBox::MyAccountPreferenceConfigureSoundComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

MyAccountPreferenceConfigureSoundComboBox::~MyAccountPreferenceConfigureSoundComboBox() = default;

QByteArray MyAccountPreferenceConfigureSoundComboBox::identifier() const
{
    return currentData(NotificationDesktopSoundPreferenceModel::NotificationPreference).toByteArray();
}

void MyAccountPreferenceConfigureSoundComboBox::setCurrentSoundName(const QByteArray &identifier)
{
    setCurrentIndex(findData(identifier, NotificationDesktopSoundPreferenceModel::NotificationPreference));
}
