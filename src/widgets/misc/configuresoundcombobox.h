/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureSoundComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ConfigureSoundComboBox(QWidget *parent = nullptr);
    ~ConfigureSoundComboBox() override;

    void setCurrentSoundName(const QByteArray &identifier);
    [[nodiscard]] QByteArray identifier() const;
};
