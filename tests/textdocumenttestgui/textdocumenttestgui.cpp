/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textdocumenttestgui.h"
#include <QApplication>
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;
TextDocumentTestGui::TextDocumentTestGui(QWidget *parent)
    : QWidget(parent)
{
}

TextDocumentTestGui::~TextDocumentTestGui() = default;

int main(int argc, char *argv[])
{
    const QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    TextDocumentTestGui w;
    w.show();
    return app.exec();
}
