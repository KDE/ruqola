/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationgui.h"

#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStandardPaths>

RestAuthenticationGui::RestAuthenticationGui(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QFormLayout(this);
    auto lineEdit = new QLineEdit(this);
    mainLayout->addRow(QStringLiteral("Server Url:"), lineEdit);
}

RestAuthenticationGui::~RestAuthenticationGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    RestAuthenticationGui w;
    w.show();
    return app.exec();
}

#include "moc_restauthenticationgui.cpp"
