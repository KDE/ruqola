/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewizard.h"
#include "createvideomessagewidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

CreateVideoMessageWizard::CreateVideoMessageWizard(QWidget *parent)
    : QWizard(parent)
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    setPage(CreateVideo, new CreateVideoMessagePage(this));
    setPage(CreateMessage, new CreateMessagePage(this));
}

CreateVideoMessageWizard::~CreateVideoMessageWizard()
{
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

CreateVideoMessagePage::~CreateVideoMessagePage()
{
}

CreateMessagePage::CreateMessagePage(QWidget *parent)
    : QWizardPage(parent)
{
}

CreateMessagePage::~CreateMessagePage()
{
}
