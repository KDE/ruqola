/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT TimeStampInMessagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesDialog(QWidget *parent = nullptr);
    ~TimeStampInMessagesDialog() override;
};
