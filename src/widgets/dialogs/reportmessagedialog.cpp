/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myReportMessageDialogGroupName[] = "ReportMessageDialog";
}
using namespace Qt::Literals::StringLiterals;
ReportMessageDialog::ReportMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mReportMessageWidget(new ReportMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Report Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mReportMessageWidget->setObjectName(u"mReportMessageWidget"_s);
    mainLayout->addWidget(mReportMessageWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ReportMessageDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ReportMessageDialog::reject);
    readConfig();

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    okButton->setText(i18nc("@action:button", "Report"));
    connect(mReportMessageWidget, &ReportMessageWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ReportMessageDialog::~ReportMessageDialog()
{
    writeConfig();
}

void ReportMessageDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myReportMessageDialogGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myReportMessageDialogGroupName), 400, 300);
#endif
}

void ReportMessageDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myReportMessageDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

QString ReportMessageDialog::message() const
{
    return mReportMessageWidget->message();
}

void ReportMessageDialog::setPreviewMessage(const QString &msg)
{
    mReportMessageWidget->setPreviewMessage(msg);
}

#include "moc_reportmessagedialog.cpp"
