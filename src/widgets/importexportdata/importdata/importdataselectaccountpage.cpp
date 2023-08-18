/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdataselectaccountpage.h"
#include <KLocalizedString>
#include <KUrlRequester>
#include <QLabel>
#include <QVBoxLayout>
#include <kio_version.h>

ImportDataSelectAccountPage::ImportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
    , mUrlRequester(new KUrlRequester(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mUrlRequester->setObjectName(QStringLiteral("mUrlRequester"));
#if KIO_VERSION < QT_VERSION_CHECK(5, 108, 0)
    mUrlRequester->setFilter(QStringLiteral("*.zip"));
#else
    mUrlRequester->setNameFilter(QStringLiteral("*.zip"));
#endif
    connect(mUrlRequester, &KUrlRequester::urlSelected, this, [this]() {
        Q_EMIT completeChanged();
    });
    connect(mUrlRequester, &KUrlRequester::textChanged, this, [this]() {
        Q_EMIT completeChanged();
    });

    auto label = new QLabel(i18n("Select Zip file:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mUrlRequester);
}

ImportDataSelectAccountPage::~ImportDataSelectAccountPage() = default;

QUrl ImportDataSelectAccountPage::zipFileUrl() const
{
    return mUrlRequester->url();
}

bool ImportDataSelectAccountPage::verifySelectedUrl() const
{
    const auto url = mUrlRequester->url();
    return (url.isValid() && !url.toLocalFile().isEmpty());
}

bool ImportDataSelectAccountPage::validatePage()
{
    return verifySelectedUrl();
}

bool ImportDataSelectAccountPage::isComplete() const
{
    return verifySelectedUrl();
}

#include "moc_importdataselectaccountpage.cpp"
