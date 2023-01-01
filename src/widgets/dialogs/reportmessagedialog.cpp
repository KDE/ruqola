/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagedialog.h"
#include "reportmessagewidget.h"

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
static const char myReportMessageDialogGroupName[] = "ReportMessageDialog";
}
ReportMessageDialog::ReportMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mReportMessageWidget(new ReportMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Report Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mReportMessageWidget->setObjectName(QStringLiteral("mReportMessageWidget"));
    mainLayout->addWidget(mReportMessageWidget);
    mainLayout->addStretch(1);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ReportMessageDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ReportMessageDialog::reject);
    readConfig();

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mReportMessageWidget, &ReportMessageWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ReportMessageDialog::~ReportMessageDialog()
{
    writeConfig();
}

void ReportMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myReportMessageDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ReportMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myReportMessageDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

QString ReportMessageDialog::message() const
{
    return mReportMessageWidget->message();
}

void ReportMessageDialog::setPreviewMessage(const QString &msg)
{
    mReportMessageWidget->setPreviewMessage(msg);
}
