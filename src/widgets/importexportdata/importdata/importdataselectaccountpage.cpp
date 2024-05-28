/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdataselectaccountpage.h"
#include <KLocalizedString>
#include <KUrlRequester>
#include <QLabel>
#include <QVBoxLayout>

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
    mUrlRequester->setNameFilter(QStringLiteral("*.zip"));
    connect(mUrlRequester, &KUrlRequester::urlSelected, this, [this]() {
        Q_EMIT completeChanged();
    });
    connect(mUrlRequester, &KUrlRequester::textChanged, this, [this]() {
        Q_EMIT completeChanged();
    });

    auto label = new QLabel(i18nc("@label:textbox", "Select Zip file:"), this);
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
    if (mUrlRequester->text().trimmed().isEmpty()) {
        return false;
    }
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
