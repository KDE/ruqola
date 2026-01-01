/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizard.h"
using namespace Qt::Literals::StringLiterals;

#include "createsoundmessagewidget.h"
#include "playsoundwidget.h"
#include <KLineEditEventHandler>
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
    setButtonText(QWizard::FinishButton, i18nc("@action:button", "Send"));
    connect(this, &CreateSoundMessageWizard::finished, this, &CreateSoundMessageWizard::slotAccepted);
    mCreateSoundMessagePage->loadSettings();
}

void CreateSoundMessageWizard::slotAccepted()
{
    mCreateSoundMessagePage->saveSettings();
    accept();
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
    if (id == CreateSoundMessageEnum::GenerateSoundMessage) {
        mGenerateSoundMessagePage->setFileNamePath(mCreateSoundMessagePage->fileNameUrl().toLocalFile());
    }
}

CreateSoundMessagePage::CreateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mCreateSoundMessageWidget(new CreateSoundMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateSoundMessageWidget->setObjectName(u"mCreateSoundMessageWidget"_s);
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
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowSoundWidget->setObjectName(u"mShowSoundWidget"_s);
    mainLayout->addWidget(mShowSoundWidget);

    auto label = new QLabel(i18nc("@label:textbox", "Filename:"), this);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mFileName->setObjectName(u"mFileName"_s);

    mDescription->setObjectName(u"mDescription"_s);

    mainLayout->addWidget(mFileName);

    KLineEditEventHandler::catchReturnKey(mFileName);
    KLineEditEventHandler::catchReturnKey(mDescription);

    label = new QLabel(i18nc("@label:textbox", "Description:"), this);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mainLayout->addWidget(mDescription);
}

GenerateSoundMessagePage::~GenerateSoundMessagePage() = default;

void GenerateSoundMessagePage::setFileNamePath(const QString &path)
{
    mShowSoundWidget->setLocalAudioPath(path);
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
    d.space() << "mDescription" << t.mDescription;
    d.space() << "mFileName" << t.mFileName;
    d.space() << "mDescription" << t.mDescription;
    return d;
}

#include "moc_createsoundmessagewizard.cpp"
