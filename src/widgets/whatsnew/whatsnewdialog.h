/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_export.h"
#include <QDialog>
class WhatsNewWidget;
class LIBRUQOLAWIDGETS_EXPORT WhatsNewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WhatsNewDialog(QWidget *parent = nullptr);
    ~WhatsNewDialog() override;

    void updateInformations();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    WhatsNewWidget *const mWhatsNewWidget;
};
