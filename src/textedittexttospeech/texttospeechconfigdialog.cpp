/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfigdialog.h"
#include "texttospeechconfigwidget.h"
#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myTextToSpeechConfigDialogConfigGroupName[] = "TextToSpeechConfigDialog";
}
using namespace TextEditTextToSpeech;

TextToSpeechConfigDialog::TextToSpeechConfigDialog(QWidget *parent)
    : QDialog(parent)
    , mTextToSpeechConfigWidget(new TextToSpeechConfigWidget(parent))
{
    setWindowTitle(i18nc("@title:window", "Configure Text-To-Speech"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mTextToSpeechConfigWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TextToSpeechConfigDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TextToSpeechConfigDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &TextToSpeechConfigDialog::slotRestoreDefaults);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

TextToSpeechConfigDialog::~TextToSpeechConfigDialog()
{
    writeConfig();
}

void TextToSpeechConfigDialog::slotRestoreDefaults()
{
    mTextToSpeechConfigWidget->restoreDefaults();
}

void TextToSpeechConfigDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 200));
    KConfigGroup group(KSharedConfig::openStateConfig(), myTextToSpeechConfigDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
    mTextToSpeechConfigWidget->readConfig();
}

void TextToSpeechConfigDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTextToSpeechConfigDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void TextToSpeechConfigDialog::slotAccepted()
{
    mTextToSpeechConfigWidget->writeConfig();
    accept();
}
