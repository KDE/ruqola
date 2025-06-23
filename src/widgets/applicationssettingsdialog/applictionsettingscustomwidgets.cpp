/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applictionsettingscustomwidgets.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
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
    const QString translatedString = mRocketChatAccount->getTranslatedIdentifier(lang, u"app-"_s + QString::fromLatin1(mAppId) + u'.' + id);
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
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
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
    return mCheckBox->isChecked() ? u"true"_s : u"false"_s;
}

ApplictionSettingsCustomWidgetsSpinBox::ApplictionSettingsCustomWidgetsSpinBox(const QByteArray &appId,
                                                                               RocketChatAccount *account,
                                                                               const ApplicationsSettingsSettingsInfo &info,
                                                                               QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, info, parent)
    , mSpinBox(new QSpinBox(this))
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
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
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
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

ApplictionSettingsCustomWidgetsString::ApplictionSettingsCustomWidgetsString(const QByteArray &appId,
                                                                             RocketChatAccount *account,
                                                                             const ApplicationsSettingsSettingsInfo &info,
                                                                             QWidget *parent)
    : ApplictionSettingsCustomWidgetsBase(appId, account, info, parent)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    label->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(label, 0, info.multiLine() ? Qt::AlignTop : Qt::Alignment());
    if (info.multiLine()) {
        mPlainTextEdit = new QPlainTextEdit(this);
        mPlainTextEdit->setObjectName(info.id());
        hbox->addWidget(mPlainTextEdit);
        const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
        mPlainTextEdit->setPlainText(r.toString());

        connect(mPlainTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
            Q_EMIT dataChanged(true);
        });
        connect(this, &ApplictionSettingsCustomWidgetsComboBox::resetValue, this, [this, r]() {
            QSignalBlocker b(mPlainTextEdit);
            mPlainTextEdit->setPlainText(r.toString());
        });
    } else {
        mLineEdit = new QLineEdit(this);
        mLineEdit->setObjectName(info.id());
        hbox->addWidget(mLineEdit);
        const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
        mLineEdit->setText(r.toString());

        connect(mLineEdit, &QLineEdit::textChanged, this, [this]() {
            Q_EMIT dataChanged(true);
        });
        connect(this, &ApplictionSettingsCustomWidgetsComboBox::resetValue, this, [this, r]() {
            QSignalBlocker b(mLineEdit);
            mLineEdit->setText(r.toString());
        });
    }
}

ApplictionSettingsCustomWidgetsString::~ApplictionSettingsCustomWidgetsString() = default;

QString ApplictionSettingsCustomWidgetsString::value() const
{
    if (mLineEdit) {
        return mLineEdit->text();
    } else if (mPlainTextEdit) {
        return mPlainTextEdit->toPlainText();
    }
    return {};
}

#include "moc_applictionsettingscustomwidgets.cpp"
