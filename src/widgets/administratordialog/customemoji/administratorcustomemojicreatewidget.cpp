/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorcustomemojicreatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QFormLayout>
#include <QLineEdit>

#include <KLocalizedString>
#include <KUrlRequester>

AdministratorCustomEmojiCreateWidget::AdministratorCustomEmojiCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mAlias(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    mName->setClearButtonEnabled(true);
    mAlias->setObjectName(QStringLiteral("mAlias"));
    mAlias->setClearButtonEnabled(true);
    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    new LineEditCatchReturnKey(mName, this);
    new LineEditCatchReturnKey(mAlias, this);
    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Alias:"), mAlias);
    mainLayout->addRow(i18n("File:"), mSelectFile);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
    connect(mAlias, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
}

AdministratorCustomEmojiCreateWidget::~AdministratorCustomEmojiCreateWidget() = default;

void AdministratorCustomEmojiCreateWidget::setCustomEmojiInfo(const CustomEmojiCreateInfo &info)
{
    mName->setText(info.name);
    mAlias->setText(info.alias);

    // TODO url ???
}

AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo AdministratorCustomEmojiCreateWidget::info() const
{
    AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info;
    info.name = mName->text().trimmed();
    info.alias = mAlias->text().trimmed();
    info.fileNameUrl = mSelectFile->url();
    return info;
}

void AdministratorCustomEmojiCreateWidget::slotUpdateOkButton()
{
    Q_EMIT updateOkButton(!mName->text().trimmed().isEmpty() && !mAlias->text().trimmed().isEmpty());
}

QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &t)
{
    d << "alias " << t.alias;
    d << "name " << t.name;
    d << "fileNameUrl " << t.fileNameUrl;
    return d;
}
