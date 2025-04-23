/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applictionsettingscustomwidgets.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <QCheckBox>
#include <QHBoxLayout>

ApplictionSettingsCustomWidgetsBase::ApplictionSettingsCustomWidgetsBase(const QByteArray &appId, RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mAppId(appId)
{
}

ApplictionSettingsCustomWidgetsBase::~ApplictionSettingsCustomWidgetsBase() = default;

QString ApplictionSettingsCustomWidgetsBase::getTranslatedIdentifier(const QString &lang, const QString &id) const
{
    if (id.isEmpty()) {
        return {};
    }
    const QString translatedString =
        mRocketChatAccount->getTranslatedIdentifier(lang, QStringLiteral("app-") + QString::fromLatin1(mAppId) + QLatin1Char('.') + id);
    if (translatedString.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " Translated string not found: " << QString::fromLatin1(mAppId) + id;
        return id;
    }
    return translatedString;
}

ApplictionSettingsCustomWidgetsCheckBox::ApplictionSettingsCustomWidgetsCheckBox(const QByteArray &appId,
                                                                                 RocketChatAccount *account,
                                                                                 const ApplicationsSettingsSettingsInfo &info,
                                                                                 QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, parent)
    , mCheckBox(new QCheckBox(this))
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    mCheckBox->setText(getTranslatedIdentifier(lang, info.i18nLabel()));
    mCheckBox->setObjectName(info.id());
    mCheckBox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(mCheckBox);
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    mCheckBox->setCheckState(r.toBool() ? Qt::Checked : Qt::Unchecked);

    connect(mCheckBox, &QCheckBox::clicked, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplictionSettingsCustomWidgetsCheckBox::resetValue, this, [this, r]() {
        QSignalBlocker b(mCheckBox);
        mCheckBox->setCheckState(r.toBool() ? Qt::Checked : Qt::Unchecked);
    });
}

ApplictionSettingsCustomWidgetsCheckBox::~ApplictionSettingsCustomWidgetsCheckBox() = default;

QString ApplictionSettingsCustomWidgetsCheckBox::value() const
{
    return mCheckBox->isChecked() ? QStringLiteral("true") : QStringLiteral("false");
}

#include "moc_applictionsettingscustomwidgets.cpp"
