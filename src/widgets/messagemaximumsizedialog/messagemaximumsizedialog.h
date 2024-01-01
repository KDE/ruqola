/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString description() const;
    [[nodiscard]] QString fileName() const;

private:
    MessageMaximumSizeWidget *const mMessageMaximumSizeWidget;
};
