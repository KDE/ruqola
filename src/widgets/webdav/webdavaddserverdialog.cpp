/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavaddserverdialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
WebDavAddServerDialog::WebDavAddServerDialog(QWidget *parent)
    : QDialog(parent)
    , mWebDavAddServerWidget(new WebDavAddServerWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add New WebDAV Server"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mWebDavAddServerWidget->setObjectName(u"mWebDavAddServerWidget"_s);
    mainLayout->addWidget(mWebDavAddServerWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    mainLayout->addWidget(buttonBox);
    buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Add WebDAV Server"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &WebDavAddServerDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &WebDavAddServerDialog::accept);
}

WebDavAddServerDialog::~WebDavAddServerDialog() = default;

WebDavAddServerWidget::WebDavAddServerInfo WebDavAddServerDialog::addServerInfo() const
{
    return mWebDavAddServerWidget->addServerInfo();
}

#include "moc_webdavaddserverdialog.cpp"
