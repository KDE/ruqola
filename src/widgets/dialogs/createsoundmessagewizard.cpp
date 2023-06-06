/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizard.h"
#include "createsoundmessagewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
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
}

CreateSoundMessageWizard::~CreateSoundMessageWizard() = default;

void CreateSoundMessageWizard::slotCurrentIdChanged(int id)
{
    if (id == GenerateSoundMessage) {
        // TODO mCreateMessagePage->setFileNamePath(mCreateVideoMessagePage->fileNamePath());
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
}

bool CreateSoundMessagePage::validatePage()
{
    // TODO
    return true;
}

CreateSoundMessagePage::~CreateSoundMessagePage() = default;

GenerateSoundMessagePage::GenerateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // mShowVideoWidget->setObjectName(QStringLiteral("mShowVideoWidget"));
    // mainLayout->addWidget(mShowVideoWidget);

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

QString GenerateSoundMessagePage::fileName() const
{
    return mFileName->text();
}

QString GenerateSoundMessagePage::description() const
{
    return mDescription->text();
}
