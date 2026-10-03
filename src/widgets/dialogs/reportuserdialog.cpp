/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserdialog.h"

#include "reportuserwidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myReportUserDialogGroupName[] = "ReportUserDialog";
}
using namespace Qt::Literals::StringLiterals;
ReportUserDialog::ReportUserDialog(QWidget *parent)
    : QDialog(parent)
    , mReportUserWidget(new ReportUserWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Report User"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mReportUserWidget->setObjectName(u"mReportUserWidget"_s);
    mainLayout->addWidget(mReportUserWidget);
    mainLayout->addStretch(1);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ReportUserDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ReportUserDialog::reject);
    readConfig();

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    okButton->setText(i18nc("@action:button", "Report"));
    connect(mReportUserWidget, &ReportUserWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ReportUserDialog::~ReportUserDialog() = default;

void ReportUserDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myReportUserDialogGroupName), QSize(400, 300));
}

QString ReportUserDialog::message() const
{
    return mReportUserWidget->message();
}

void ReportUserDialog::setUserName(const QString &userName)
{
    mReportUserWidget->setUserName(userName);
}

#include "moc_reportuserdialog.cpp"
