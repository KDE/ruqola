/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizard.h"
#include "createsoundmessagewidget.h"
#include "playsoundwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

CreateSoundMessageWizard::CreateSoundMessageWizard(QWidget *parent)
    : QWizard(parent)
    , mCreateSoundMessagePage(new CreateSoundMessagePage(this))
    , mGenerateSoundMessagePage(new GenerateSoundMessagePage(this))

{
    setWindowTitle(i18nc("@title:window", "Create Sound Message"));
    setPage(CreateSound, mCreateSoundMessagePage);
    setPage(GenerateSoundMessage, mGenerateSoundMessagePage);
    connect(this, &CreateSoundMessageWizard::currentIdChanged, this, &CreateSoundMessageWizard::slotCurrentIdChanged);
    setButtonText(QWizard::FinishButton, i18n("Send"));
}

CreateSoundMessageWizard::~CreateSoundMessageWizard() = default;

void CreateSoundMessageWizard::slotCurrentIdChanged(int id)
{
    if (id == GenerateSoundMessage) {
        mGenerateSoundMessagePage->setFileNamePath(mCreateSoundMessagePage->fileNamePath());
    }
}

CreateSoundMessagePage::CreateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mCreateSoundMessageWidget(new CreateSoundMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateSoundMessageWidget->setObjectName(QStringLiteral("mCreateSoundMessageWidget"));
    mainLayout->addWidget(mCreateSoundMessageWidget);
    connect(mCreateSoundMessageWidget, &CreateSoundMessageWidget::recordDone, this, &CreateSoundMessagePage::completeChanged);
}

QUrl CreateSoundMessagePage::fileNamePath() const
{
    // TODO
    return {};
    // return mCreateSoundMessageWidget->temporaryFilePath();
}

bool CreateSoundMessagePage::validatePage()
{
    // TODO
    return true;
}

CreateSoundMessagePage::~CreateSoundMessagePage() = default;

GenerateSoundMessagePage::GenerateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mShowSoundWidget(new PlaySoundWidget(this))
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowSoundWidget->setObjectName(QStringLiteral("mShowSoundWidget"));
    mainLayout->addWidget(mShowSoundWidget);

    auto label = new QLabel(i18n("Filename:"), this);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mFileName->setObjectName(QStringLiteral("mFileName"));

    mDescription->setObjectName(QStringLiteral("mDescription"));

    mainLayout->addWidget(mFileName);

    label = new QLabel(i18n("Description:"), this);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mainLayout->addWidget(mDescription);
}

GenerateSoundMessagePage::~GenerateSoundMessagePage() = default;

void GenerateSoundMessagePage::setFileNamePath(const QUrl &url)
{
}

QString GenerateSoundMessagePage::fileName() const
{
    return mFileName->text();
}

QString GenerateSoundMessagePage::description() const
{
    return mDescription->text();
}

QUrl GenerateSoundMessagePage::fileNamePath() const
{
    // TODO
    return {};
}
