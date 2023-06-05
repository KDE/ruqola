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
    , mCreateMessagePage(new CreateMessagePage(this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    setPage(CreateVideo, mCreateVideoMessagePage);
    setPage(CreateMessage, mCreateMessagePage);
    readConfig();
}

CreateVideoMessageWizard::~CreateVideoMessageWizard()
{
    writeConfig();
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

bool CreateVideoMessagePage::validatePage()
{
    // TODO
    return true;
}

CreateMessagePage::CreateMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mShowVideoWidget(new ShowVideoWidget(this))
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowVideoWidget->setObjectName(QStringLiteral("mShowVideoWidget"));
    mainLayout->addWidget(mShowVideoWidget);

    // TODO setVideoUrl
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

CreateMessagePage::~CreateMessagePage() = default;
