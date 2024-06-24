/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cmarktestgui.h"
#include <QApplication>
CMarkTestGui::CMarkTestGui(QWidget *parent)
    : QWidget{parent}
{
}

CMarkTestGui::~CMarkTestGui()
{
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CMarkTestGui w;
    w.show();
    return app.exec();
}
