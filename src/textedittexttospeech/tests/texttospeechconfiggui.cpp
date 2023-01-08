/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfiggui.h"
#include "texttospeech.h"
#include "texttospeechconfigwidget.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QHBoxLayout>
#include <QStandardPaths>

TextToSpeechConfigGui::TextToSpeechConfigGui(QWidget *parent)
    : QWidget(parent)
{
    auto hbox = new QHBoxLayout(this);
    auto widget = new TextEditTextToSpeech::TextToSpeechConfigWidget(this);
    hbox->addWidget(widget);
}

TextToSpeechConfigGui::~TextToSpeechConfigGui() = default;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto w = new TextToSpeechConfigGui;

    w->show();
    app.exec();
    delete w;
    return 0;
}
