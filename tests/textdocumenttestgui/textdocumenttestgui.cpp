/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textdocumenttestgui.h"
#include <QApplication>
#include <QPlainTextEdit>
#include <QStandardPaths>
#include <QTextEdit>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
TextDocumentTestGui::TextDocumentTestGui(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto input = new QPlainTextEdit(this);
    mainLayout->addWidget(input);

    auto output = new QTextEdit(this);
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    connect(input, &QPlainTextEdit::textChanged, this, [input, output]() {
        output->document()->setHtml(input->toPlainText());
    });
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
