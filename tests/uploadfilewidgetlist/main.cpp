/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadwidget.h"
#include <QApplication>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    auto widget = new UploadWidget();
    widget->resize(600, 400);
    widget->show();
    app.exec();
    return 0;
}
