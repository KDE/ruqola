/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewComboBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewComboBoxWidget(QWidget *parent = nullptr);
    ~WhatsNewComboBoxWidget() override;

private:
    QComboBox *const mVersionComboBox;
};
