/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidategui.h"
#include "misc/passwordvalidatewidget.h"
#include <QApplication>
#include <QVBoxLayout>
PasswordValidateGui::PasswordValidateGui(QWidget *parent)
    : QWidget{parent}
    , mPasswordValidateWidget(new PasswordValidateWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mPasswordValidateWidget);
    RuqolaServerConfig::PasswordSettings passwordSettings;
    passwordSettings.accountsPasswordPolicyEnabled = true;
    mPasswordValidateWidget->setPasswordValidChecks(passwordSettings);
}

PasswordValidateGui::~PasswordValidateGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PasswordValidateGui w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}

#include "moc_passwordvalidategui.cpp"
