/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cmarktestgui.h"
#include "messages/message.h"
#include "textconverter.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
CMarkTestGui::CMarkTestGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
    , mTextEditResult(new QTextEdit(this))
    , mTextEditResultCMark(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTextEdit);

    auto pushButton = new QPushButton(QStringLiteral("Generate Html code"), this);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, [this]() {
        const TextConverter::ConvertMessageTextSettings settings(mTextEdit->toPlainText(), QStringLiteral("foo"), {}, {}, nullptr, nullptr, {}, {});
        QByteArray needUpdateMessageId;
        int recursiveIndex = 0;
        mTextEditResult->setHtml(TextConverter::convertMessageTextRuqola(settings, needUpdateMessageId, recursiveIndex));
        mTextEditResultCMark->setHtml(TextConverter::convertMessageTextCMark(settings, needUpdateMessageId, recursiveIndex));
    });
    mTextEditResult->setReadOnly(true);
    auto label = new QLabel(QStringLiteral("Convert with TextConverter"), this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(mTextEditResult);

    label = new QLabel(QStringLiteral("Convert with CMark"), this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(mTextEditResultCMark);
}

CMarkTestGui::~CMarkTestGui()
{
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CMarkTestGui w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}

#include "moc_cmarktestgui.cpp"
