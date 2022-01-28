/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditdialog.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorOauthEditDialog::AdministratorOauthEditDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    // TODO
}

AdministratorOauthEditDialog::~AdministratorOauthEditDialog()
{
}
