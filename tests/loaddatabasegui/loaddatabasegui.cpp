/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loaddatabasegui.h"
#include <QApplication>
#include <QStandardPaths>
#include <QVBoxLayout>

LoadDataBaseGui::LoadDataBaseGui(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    // TODO
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    LoadDataBaseGui w;
    w.show();
    return app.exec();
}
