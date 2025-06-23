/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdataselectaccountpage.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <KUrlRequester>
#include <QLabel>
#include <QVBoxLayout>

ImportDataSelectAccountPage::ImportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
    , mUrlRequester(new KUrlRequester(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mUrlRequester->setObjectName(u"mUrlRequester"_s);
    mUrlRequester->setNameFilter(u"*.zip"_s);
    connect(mUrlRequester, &KUrlRequester::urlSelected, this, [this]() {
        Q_EMIT completeChanged();
    });
    connect(mUrlRequester, &KUrlRequester::textChanged, this, [this]() {
        Q_EMIT completeChanged();
    });

    auto label = new QLabel(i18nc("@label:textbox", "Select Zip file:"), this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::PlainText);
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
