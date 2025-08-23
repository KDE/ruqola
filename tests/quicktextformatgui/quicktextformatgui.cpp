/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "quicktextformatgui.h"
#include "room/quicktextformatmessage.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTextEdit>
using namespace Qt::Literals::StringLiterals;
QuickTextFormatGui::QuickTextFormatGui(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    auto textEdit = new QTextEdit(this);
    auto quickTextFormatMessage = new QuickTextFormatMessage(textEdit, this);
    connect(quickTextFormatMessage, &QuickTextFormatMessage::quickTextFormatRequested, this, [textEdit](QuickTextFormatMessage::QuickTextFormatType type) {
        switch (type) {
        case QuickTextFormatMessage::QuickTextFormatType::Bold: {
            QTextCharFormat format;
            format.setFontWeight(textEdit->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
            textEdit->textCursor().mergeCharFormat(format);
            break;
        }
        case QuickTextFormatMessage::QuickTextFormatType::Italic: {
            QTextCharFormat format;
            format.setFontItalic(!textEdit->fontItalic());
            textEdit->textCursor().mergeCharFormat(format);
            break;
        }
        case QuickTextFormatMessage::QuickTextFormatType::StrikeThrough:
            // TODO
            break;
        case QuickTextFormatMessage::QuickTextFormatType::CodeBlock:
            // TODO
            break;
        case QuickTextFormatMessage::QuickTextFormatType::BlockQuote:
            // TODO
            break;
        case QuickTextFormatMessage::QuickTextFormatType::InsertLink:
            // TODO
            break;
        }
    });
    mainLayout->addWidget(textEdit);
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

#include "moc_quicktextformatgui.cpp"
