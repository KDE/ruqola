/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    auto w = new TextTranslator::TranslatorConfigureLanguageListWidget(QStringLiteral("from"), this);
    mainLayout->addWidget(w);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, [w]() {
        qDebug() << " select " << w->selectedLanguages();
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TranslatorConfigureLanguageListWidget_gui::accept);
    const QMap<TextTranslator::TranslatorUtil::Language, QString> languages; // = TextTranslator::TranslatorUtil::translatedLanguages();

    QMapIterator<TextTranslator::TranslatorUtil::Language, QString> i(languages);
    TextTranslator::TranslatorUtil translatorUtil;
    while (i.hasNext()) {
        i.next();
        const QString languageCode = TextTranslator::TranslatorUtil::languageCode(i.key());
        w->addItem(i.value(), languageCode);
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
