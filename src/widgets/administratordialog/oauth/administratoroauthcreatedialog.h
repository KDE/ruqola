/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

#include "administratoroauthcreatewidget.h"
#include "libruqolawidgets_private_export.h"
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorOauthCreateDialog(QWidget *parent = nullptr);
    ~AdministratorOauthCreateDialog() override;

    [[nodiscard]] AdministratorOauthCreateWidget::OauthCreateInfo oauthInfo() const;
    void setOauthInfo(const AdministratorOauthCreateWidget::OauthCreateInfo &info);

private:
    void readConfig();
    void writeConfig();
    AdministratorOauthCreateWidget *const mCreateWidget;
    QPushButton *mOkButton = nullptr;
};
