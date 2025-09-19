/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class WebDavAddServerWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WebDavAddServerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WebDavAddServerDialog(QWidget *parent = nullptr);
    ~WebDavAddServerDialog() override;

private:
    WebDavAddServerWidget *const mWebDavAddServerWidget;
};
