/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserdialog.h"
#include "reportuserwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myReportUserDialogGroupName[] = "ReportUserDialog";
}
ReportUserDialog::ReportUserDialog(QWidget *parent)
    : QDialog(parent)
    , mReportUserWidget(new ReportUserWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Report User"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mReportUserWidget->setObjectName(QStringLiteral("mReportUserWidget"));
    mainLayout->addWidget(mReportUserWidget);
    mainLayout->addStretch(1);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ReportUserDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ReportUserDialog::reject);
    readConfig();

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mReportUserWidget, &ReportUserWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ReportUserDialog::~ReportUserDialog()
{
    writeConfig();
}

void ReportUserDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myReportUserDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ReportUserDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myReportUserDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

QString ReportUserDialog::message() const
{
    return mReportUserWidget->message();
}

#include "moc_reportuserdialog.cpp"
