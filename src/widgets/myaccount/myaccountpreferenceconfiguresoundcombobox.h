/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPreferenceConfigureSoundComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyAccountPreferenceConfigureSoundComboBox(QWidget *parent = nullptr);
    ~MyAccountPreferenceConfigureSoundComboBox() override;

    void setCurrentSoundName(const QByteArray &identifier);
    [[nodiscard]] QByteArray identifier() const;
};
