/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfiguredialog.h"
#include "translatorconfigurewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
const char myTranslatorConfigureDialogConfigGroupName[] = "TranslatorConfigureDialog";
}

using namespace PimCommonTextTranslator;
TranslatorConfigureDialog::TranslatorConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mTranslatorConfigureWidget(new TranslatorConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Translator"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTranslatorConfigureWidget->setObjectName(QStringLiteral("mTranslatorConfigureWidget"));
    mainLayout->addWidget(mTranslatorConfigureWidget);
    mainLayout->addStretch();

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &TranslatorConfigureDialog::slotAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TranslatorConfigureDialog::reject);
    mTranslatorConfigureWidget->loadSettings();
    readConfig();
}

TranslatorConfigureDialog::~TranslatorConfigureDialog()
{
    writeConfig();
}

void TranslatorConfigureDialog::slotAccept()
{
    mTranslatorConfigureWidget->saveSettings();
    accept();
}

void TranslatorConfigureDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTranslatorConfigureDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TranslatorConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTranslatorConfigureDialogConfigGroupName);
    group.writeEntry("Size", size());
}
