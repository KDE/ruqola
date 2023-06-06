/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewizard.h"
#include "createvideomessagewidget.h"
#include "showvideowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigCreateVideoMessageWizardGroupName[] = "CreateVideoMessageWizard";
}
CreateVideoMessageWizard::CreateVideoMessageWizard(QWidget *parent)
    : QWizard(parent)
    , mCreateVideoMessagePage(new CreateVideoMessagePage(this))
    , mCreateMessagePage(new GenerateVideoMessagePage(this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    setPage(CreateVideo, mCreateVideoMessagePage);
    setPage(CreateMessage, mCreateMessagePage);
    readConfig();
    connect(this, &CreateVideoMessageWizard::currentIdChanged, this, &CreateVideoMessageWizard::slotCurrentIdChanged);
}

CreateVideoMessageWizard::~CreateVideoMessageWizard()
{
    writeConfig();
}

void CreateVideoMessageWizard::slotCurrentIdChanged(int id)
{
    if (id == CreateMessage) {
        mCreateMessagePage->setFileNamePath(mCreateVideoMessagePage->fileNamePath());
    }
}

CreateVideoMessageWizard::CreateVideoMessageInfo CreateVideoMessageWizard::videoMessageInfo() const
{
    CreateVideoMessageInfo info;
    info.mDescription = mCreateMessagePage->description();
    info.mFileName = mCreateMessagePage->fileName();
    info.mFilePath = mCreateMessagePage->fileNamePath();
    return info;
}

void CreateVideoMessageWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateVideoMessageWizardGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateVideoMessageWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateVideoMessageWizardGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

CreateVideoMessagePage::CreateVideoMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mCreateVideoMessageWidget(new CreateVideoMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateVideoMessageWidget->setObjectName(QStringLiteral("mCreateVideoMessageWidget"));
    mainLayout->addWidget(mCreateVideoMessageWidget);
}

CreateVideoMessagePage::~CreateVideoMessagePage() = default;

QUrl CreateVideoMessagePage::fileNamePath() const
{
    return mCreateVideoMessageWidget->temporaryFilePath();
}

bool CreateVideoMessagePage::validatePage()
{
    // TODO
    return true;
}

GenerateVideoMessagePage::GenerateVideoMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mShowVideoWidget(new ShowVideoWidget(this))
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowVideoWidget->setObjectName(QStringLiteral("mShowVideoWidget"));
    mainLayout->addWidget(mShowVideoWidget);

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

GenerateVideoMessagePage::~GenerateVideoMessagePage() = default;

QString GenerateVideoMessagePage::fileName() const
{
    return mFileName->text();
}

QString GenerateVideoMessagePage::description() const
{
    return mDescription->text();
}

void GenerateVideoMessagePage::setFileNamePath(const QUrl &url)
{
    mShowVideoWidget->setVideoUrl(url);
}

QUrl GenerateVideoMessagePage::fileNamePath() const
{
    return mShowVideoWidget->videoUrl();
}

QDebug operator<<(QDebug d, const CreateVideoMessageWizard::CreateVideoMessageInfo &t)
{
    d << " mDescription " << t.mDescription;
    d << " mFileName " << t.mFileName;
    d << " mDescription " << t.mDescription;
    return d;
}

bool CreateVideoMessageWizard::CreateVideoMessageInfo::isValid() const
{
    return mFilePath.isValid();
}
