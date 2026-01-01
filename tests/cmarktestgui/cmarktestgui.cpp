/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

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
    , mTextEditResultCMark(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTextEdit);

    auto pushButton = new QPushButton(u"Generate Html code"_s, this);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, [this]() {
        const TextConverter::ConvertMessageTextSettings settings(mTextEdit->toPlainText(), u"foo"_s, {}, {}, nullptr, nullptr, {}, {}, {});
        QByteArray needUpdateMessageId;
        int recursiveIndex = 0;
        int numberOfTextSearched = 0;
        const int hightLightStringIndex = 0;
        mTextEditResultCMark->setHtml(
            TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex));
    });

    mTextEditResultCMark->setReadOnly(true);
    auto label = new QLabel(u"Convert with CMark"_s, this);
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
