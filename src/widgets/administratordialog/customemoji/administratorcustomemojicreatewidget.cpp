/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include <KColorScheme>
#include <KLocalizedString>
#include <KStatefulBrush>
#include <KUrlRequester>

AdministratorCustomEmojiCreateWidget::AdministratorCustomEmojiCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mAlias(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
    , mWarningLabel(new QLabel(i18n("The custom emoji name and their aliases should be different."), this))
{
    mWarningLabel->setObjectName(QStringLiteral("mWarningLabel"));
    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QColor color = bgBrush.brush(palette()).color();

    QPalette pal = mWarningLabel->palette();
    pal.setColor(QPalette::WindowText, color);
    mWarningLabel->setPalette(pal);
    mWarningLabel->hide();

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
    mainLayout->addWidget(mWarningLabel);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
    connect(mAlias, &QLineEdit::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
    connect(mSelectFile, &KUrlRequester::urlSelected, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
    connect(mSelectFile, &KUrlRequester::textChanged, this, &AdministratorCustomEmojiCreateWidget::slotUpdateOkButton);
}

AdministratorCustomEmojiCreateWidget::~AdministratorCustomEmojiCreateWidget() = default;

void AdministratorCustomEmojiCreateWidget::setCustomEmojiInfo(const CustomEmojiCreateInfo &info)
{
    mName->setText(info.name);
    mAlias->setText(info.alias);

    // TODO url ???
    slotUpdateOkButton();
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
    if (mName->text().trimmed() != mAlias->text().trimmed()) {
        mWarningLabel->hide();
        Q_EMIT updateOkButton(!mName->text().trimmed().isEmpty() && mSelectFile->url().isValid());
    } else {
        mWarningLabel->show();
        Q_EMIT updateOkButton(false);
    }
}

QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &t)
{
    d << "alias " << t.alias;
    d << "name " << t.name;
    d << "fileNameUrl " << t.fileNameUrl;
    return d;
}
