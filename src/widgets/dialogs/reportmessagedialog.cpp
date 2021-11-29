/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagedialog.h"
#include "reportmessagewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
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
    KConfigGroup group(KSharedConfig::openStateConfig(), myReportMessageDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ReportMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myReportMessageDialogGroupName);
    group.writeEntry("Size", size());
}

QString ReportMessageDialog::message() const
{
    return mReportMessageWidget->message();
}

void ReportMessageDialog::setPreviewMessage(const QString &msg)
{
    mReportMessageWidget->setPreviewMessage(msg);
}
