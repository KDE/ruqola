/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratoroautheditwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorOauthEditDialog(QWidget *parent = nullptr);
    ~AdministratorOauthEditDialog() override;

    [[nodiscard]] AdministratorOauthEditWidget::OauthEditInfo oauthInfo() const;
    void setOauthInfo(const AdministratorOauthEditWidget::OauthEditInfo &info);

private:
    void writeConfig();
    void readConfig();
    AdministratorOauthEditWidget *const mOauthEditWidget;
    QPushButton *mOkButton = nullptr;
};
