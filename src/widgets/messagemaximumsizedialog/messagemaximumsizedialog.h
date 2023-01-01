/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class MessageMaximumSizeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageMaximumSizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MessageMaximumSizeDialog(QWidget *parent = nullptr);
    ~MessageMaximumSizeDialog() override;

    Q_REQUIRED_RESULT QString description() const;
    Q_REQUIRED_RESULT QString fileName() const;

private:
    MessageMaximumSizeWidget *const mMessageMaximumSizeWidget;
};
