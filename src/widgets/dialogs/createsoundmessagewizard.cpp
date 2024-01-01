/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizard.h"
#include "createsoundmessagewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "playsoundwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

CreateSoundMessageWizard::CreateSoundMessageWizard(RocketChatAccount *account, QWidget *parent)
    : QWizard(parent)
    , mCreateSoundMessagePage(new CreateSoundMessagePage(this))
    , mGenerateSoundMessagePage(new GenerateSoundMessagePage(account, this))

{
    setWindowTitle(i18nc("@title:window", "Create Sound Message"));
    setPage(CreateSound, mCreateSoundMessagePage);
    setPage(GenerateSoundMessage, mGenerateSoundMessagePage);
    connect(this, &CreateSoundMessageWizard::currentIdChanged, this, &CreateSoundMessageWizard::slotCurrentIdChanged);
    setButtonText(QWizard::FinishButton, i18n("Send"));
    connect(this, &CreateSoundMessageWizard::finished, this, &CreateSoundMessageWizard::slotAccepted);
    mCreateSoundMessagePage->loadSettings();
}

void CreateSoundMessageWizard::slotAccepted()
{
    mCreateSoundMessagePage->saveSettings();
    accepted();
}

CreateSoundMessageWizard::CreateSoundMessageInfo CreateSoundMessageWizard::soundMessageInfo() const
{
    CreateSoundMessageWizard::CreateSoundMessageInfo info;
    info.mDescription = mGenerateSoundMessagePage->description();
    info.mFileName = mGenerateSoundMessagePage->fileName();
    info.mFileUrl = mGenerateSoundMessagePage->fileNameUrl();
    return info;
}

CreateSoundMessageWizard::~CreateSoundMessageWizard() = default;

void CreateSoundMessageWizard::slotCurrentIdChanged(int id)
{
    if (id == GenerateSoundMessage) {
        mGenerateSoundMessagePage->setFileNamePath(mCreateSoundMessagePage->fileNameUrl().toLocalFile());
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

QUrl CreateSoundMessagePage::fileNameUrl() const
{
    return mCreateSoundMessageWidget->temporaryFilePath();
}

bool CreateSoundMessagePage::isComplete() const
{
    return !mCreateSoundMessageWidget->temporaryFilePath().isEmpty();
}

void CreateSoundMessagePage::loadSettings()
{
    mCreateSoundMessageWidget->loadSettings();
}

void CreateSoundMessagePage::saveSettings()
{
    mCreateSoundMessageWidget->saveSettings();
}

bool CreateSoundMessagePage::validatePage()
{
    return !mCreateSoundMessageWidget->temporaryFilePath().isEmpty();
}

CreateSoundMessagePage::~CreateSoundMessagePage() = default;

GenerateSoundMessagePage::GenerateSoundMessagePage(RocketChatAccount *account, QWidget *parent)
    : QWizardPage(parent)
    , mShowSoundWidget(new PlaySoundWidget(account, this))
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

    new LineEditCatchReturnKey(mFileName, this);
    new LineEditCatchReturnKey(mDescription, this);

    label = new QLabel(i18n("Description:"), this);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mainLayout->addWidget(mDescription);
}

GenerateSoundMessagePage::~GenerateSoundMessagePage() = default;

void GenerateSoundMessagePage::setFileNamePath(const QString &path)
{
    mShowSoundWidget->setAudioPath(path);
}

QUrl GenerateSoundMessagePage::fileNameUrl() const
{
    return mShowSoundWidget->audioUrl();
}

QString GenerateSoundMessagePage::fileName() const
{
    return mFileName->text();
}

QString GenerateSoundMessagePage::description() const
{
    return mDescription->text();
}

QDebug operator<<(QDebug d, const CreateSoundMessageWizard::CreateSoundMessageInfo &t)
{
    d << " mDescription " << t.mDescription;
    d << " mFileName " << t.mFileName;
    d << " mDescription " << t.mDescription;
    return d;
}

#include "moc_createsoundmessagewizard.cpp"
