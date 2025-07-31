/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewizard.h"
using namespace Qt::Literals::StringLiterals;

#include "createvideomessagewidget.h"
#include "showvideowidget.h"
#include <KConfigGroup>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigCreateVideoMessageWizardGroupName[] = "CreateVideoMessageWizard";
}
CreateVideoMessageWizard::CreateVideoMessageWizard(RocketChatAccount *account, QWidget *parent)
    : QWizard(parent)
    , mCreateVideoMessagePage(new CreateVideoMessagePage(this))
    , mCreateMessagePage(new GenerateVideoMessagePage(account, this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    setPage(CreateVideo, mCreateVideoMessagePage);
    setPage(CreateMessage, mCreateMessagePage);
    readConfig();
    connect(this, &CreateVideoMessageWizard::currentIdChanged, this, &CreateVideoMessageWizard::slotCurrentIdChanged);
    setButtonText(QWizard::FinishButton, i18nc("@action:button", "Send"));
    connect(this, &CreateVideoMessageWizard::finished, this, &CreateVideoMessageWizard::slotAccepted);
    mCreateVideoMessagePage->loadSettings();
}

CreateVideoMessageWizard::~CreateVideoMessageWizard()
{
    writeConfig();
}

void CreateVideoMessageWizard::slotAccepted()
{
    mCreateVideoMessagePage->saveSettings();
    accepted();
}

void CreateVideoMessageWizard::slotCurrentIdChanged(int id)
{
    if (id == CreateMessage) {
        mCreateMessagePage->setFileNamePath(mCreateVideoMessagePage->fileNamePath().toLocalFile());
    }
}

CreateVideoMessageWizard::CreateVideoMessageInfo CreateVideoMessageWizard::videoMessageInfo() const
{
    CreateVideoMessageInfo info;
    info.mDescription = mCreateMessagePage->description();
    info.mFileName = mCreateMessagePage->fileName();
    info.mFileUrl = mCreateMessagePage->fileNameUrl();
    return info;
}

void CreateVideoMessageWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigCreateVideoMessageWizardGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateVideoMessageWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigCreateVideoMessageWizardGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

CreateVideoMessagePage::CreateVideoMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mCreateVideoMessageWidget(new CreateVideoMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateVideoMessageWidget->setObjectName(u"mCreateVideoMessageWidget"_s);
    mainLayout->addWidget(mCreateVideoMessageWidget);
    connect(mCreateVideoMessageWidget, &CreateVideoMessageWidget::recordDone, this, &CreateVideoMessagePage::completeChanged);
}

CreateVideoMessagePage::~CreateVideoMessagePage() = default;

void CreateVideoMessagePage::loadSettings()
{
    mCreateVideoMessageWidget->loadSettings();
}

void CreateVideoMessagePage::saveSettings()
{
    mCreateVideoMessageWidget->saveSettings();
}

QUrl CreateVideoMessagePage::fileNamePath() const
{
    return mCreateVideoMessageWidget->temporaryFilePath();
}

bool CreateVideoMessagePage::isComplete() const
{
    return !mCreateVideoMessageWidget->temporaryFilePath().isEmpty();
}

bool CreateVideoMessagePage::validatePage()
{
    return !mCreateVideoMessageWidget->temporaryFilePath().isEmpty();
}

GenerateVideoMessagePage::GenerateVideoMessagePage(RocketChatAccount *account, QWidget *parent)
    : QWizardPage(parent)
    , mShowVideoWidget(new ShowVideoWidget(account, this))
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowVideoWidget->setObjectName(u"mShowVideoWidget"_s);
    mainLayout->addWidget(mShowVideoWidget);

    auto label = new QLabel(i18nc("@label:textbox", "Filename:"), this);
    label->setObjectName(u"label-filename"_s);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mFileName->setObjectName(u"mFileName"_s);
    mDescription->setObjectName(u"mDescription"_s);

    KLineEditEventHandler::catchReturnKey(mFileName);
    KLineEditEventHandler::catchReturnKey(mDescription);

    mainLayout->addWidget(mFileName);

    label = new QLabel(i18nc("@label:textbox", "Description:"), this);
    label->setObjectName(u"label-description"_s);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mainLayout->addWidget(mDescription);
}

GenerateVideoMessagePage::~GenerateVideoMessagePage() = default;

QString GenerateVideoMessagePage::fileName() const
{
    return mFileName->text();
}

QString GenerateVideoMessagePage::description() const
{
    return mDescription->text();
}

void GenerateVideoMessagePage::setFileNamePath(const QString &url)
{
    mShowVideoWidget->setVideoPath(url, true);
}

QUrl GenerateVideoMessagePage::fileNameUrl() const
{
    return mShowVideoWidget->videoUrl();
}

QDebug operator<<(QDebug d, const CreateVideoMessageWizard::CreateVideoMessageInfo &t)
{
    d.space() << "mDescription " << t.mDescription;
    d.space() << "mFileName " << t.mFileName;
    d.space() << "mDescription " << t.mDescription;
    return d;
}

bool CreateVideoMessageWizard::CreateVideoMessageInfo::isValid() const
{
    return mFileUrl.isValid();
}

#include "moc_createvideomessagewizard.cpp"
