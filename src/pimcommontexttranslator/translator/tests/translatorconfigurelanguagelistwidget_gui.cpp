/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurelanguagelistwidget_gui.h"
#include "translator/misc/translatorutil.h"
#include "translator/widgets/translatorconfigurelanguagelistwidget.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QVBoxLayout>

TranslatorConfigureLanguageListWidget_gui::TranslatorConfigureLanguageListWidget_gui(QWidget *parent)
    : QDialog(parent)
{
    auto mainLayout = new QVBoxLayout(this);

    auto w = new PimCommonTextTranslator::TranslatorConfigureLanguageListWidget(QStringLiteral("from"), this);
    mainLayout->addWidget(w);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, [w]() {
        qDebug() << " select " << w->selectedLanguages();
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TranslatorConfigureLanguageListWidget_gui::accept);
    auto languages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
    const int fullListLanguageSize(languages.count());
    for (int i = 0; i < fullListLanguageSize; ++i) {
        const QPair<QString, QString> currentLanguage = languages.at(i);
        w->addItem(currentLanguage);
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto dialog = new TranslatorConfigureLanguageListWidget_gui;

    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}
