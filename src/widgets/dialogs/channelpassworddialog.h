/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ChannelPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChannelPasswordDialog(QWidget *parent = nullptr);
    ~ChannelPasswordDialog() override;

    Q_REQUIRED_RESULT QString password() const;

private:
    ChannelPasswordWidget *const mChannelPasswordWidget;
};
