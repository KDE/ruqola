/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnew/whatsnewdialog.h"
#include <QApplication>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    WhatsNewDialog w;
    w.updateInformations();
    w.show();
    return app.exec();
}
