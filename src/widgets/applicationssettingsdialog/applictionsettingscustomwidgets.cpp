/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applictionsettingscustomwidgets.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

ApplictionSettingsCustomWidgetsBase::ApplictionSettingsCustomWidgetsBase(const QByteArray &appId,
                                                                         RocketChatAccount *account,
                                                                         const ApplicationsSettingsSettingsInfo &info,
                                                                         QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mAppId(appId)
    , mInfo(info)
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

ApplicationsSettingsSettingsInfo ApplictionSettingsCustomWidgetsBase::info() const
{
    return mInfo;
}

ApplictionSettingsCustomWidgetsCheckBox::ApplictionSettingsCustomWidgetsCheckBox(const QByteArray &appId,
                                                                                 RocketChatAccount *account,
                                                                                 const ApplicationsSettingsSettingsInfo &info,
                                                                                 QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, info, parent)
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

ApplictionSettingsCustomWidgetsSpinBox::ApplictionSettingsCustomWidgetsSpinBox(const QByteArray &appId,
                                                                               RocketChatAccount *account,
                                                                               const ApplicationsSettingsSettingsInfo &info,
                                                                               QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, info, parent)
    , mSpinBox(new QSpinBox(this))
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    hbox->addWidget(label);
    mSpinBox->setMaximum(999999999);
    mSpinBox->setObjectName(info.id());
    mSpinBox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(mSpinBox);
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    mSpinBox->setValue(r.toInt());
    connect(mSpinBox, &QSpinBox::valueChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplictionSettingsCustomWidgetsSpinBox::resetValue, this, [this, r]() {
        QSignalBlocker b(mSpinBox);
        mSpinBox->setValue(r.toInt());
    });
}

ApplictionSettingsCustomWidgetsSpinBox::~ApplictionSettingsCustomWidgetsSpinBox() = default;

QString ApplictionSettingsCustomWidgetsSpinBox::value() const
{
    return QString::number(mSpinBox->value());
}

ApplictionSettingsCustomWidgetsComboBox::ApplictionSettingsCustomWidgetsComboBox(const QByteArray &appId,
                                                                                 RocketChatAccount *account,
                                                                                 const ApplicationsSettingsSettingsInfo &info,
                                                                                 QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, info, parent)
    , mComboBox(new QComboBox(this))
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    hbox->addWidget(label);
    mComboBox->setObjectName(info.id());
    mComboBox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(mComboBox);
    // Fill Combobox
    QMapIterator<QString, QString> i(info.values());
    while (i.hasNext()) {
        i.next();
        mComboBox->addItem(getTranslatedIdentifier(lang, i.value()), i.key());
    }
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    mComboBox->setCurrentIndex(mComboBox->findData(r.toString()));
    connect(mComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplictionSettingsCustomWidgetsComboBox::resetValue, this, [this, r]() {
        QSignalBlocker b(mComboBox);
        mComboBox->setCurrentIndex(mComboBox->findData(r.toString()));
    });
}

ApplictionSettingsCustomWidgetsComboBox::~ApplictionSettingsCustomWidgetsComboBox() = default;

QString ApplictionSettingsCustomWidgetsComboBox::value() const
{
    return mComboBox->currentText();
}

#include "moc_applictionsettingscustomwidgets.cpp"
