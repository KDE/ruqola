/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "quicktextformatgui.h"

#include <QApplication>
#include <QStandardPaths>

QuickTextFormatGui::QuickTextFormatGui(QWidget *parent)
    : QWidget{parent}
{
}

QuickTextFormatGui::~QuickTextFormatGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    QuickTextFormatGui w;
    w.show();
    return app.exec();
}
