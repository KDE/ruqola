/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BannerInfoDialog(QWidget *parent = nullptr);
    ~BannerInfoDialog() override;

private:
    void readConfig();
    void writeConfig();
};
