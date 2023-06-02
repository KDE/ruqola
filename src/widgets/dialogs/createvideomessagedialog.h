/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class CreateVideoMessageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVideoMessageDialog(QWidget *parent = nullptr);
    ~CreateVideoMessageDialog() override;

    Q_REQUIRED_RESULT QString temporaryFilePath() const;

private:
    void readConfig();
    void writeConfig();
    CreateVideoMessageWidget *const mCreateVideoMessageWidget;
};
