/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidatewidget.h"
#include "colorsandmessageviewstyle.h"
#include <KLocalizedString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
using namespace Qt::Literals::StringLiterals;

PasswordValidateWidget::PasswordValidateWidget(QWidget *parent)
    : QWidget{parent}
    , mGridLayout(new QGridLayout(this))
{
    mGridLayout->setObjectName("mGridLayout"_L1);
    mGridLayout->setContentsMargins({});
}

PasswordValidateWidget::~PasswordValidateWidget() = default;

void PasswordValidateWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    if (mPasswordSettings != passwordSettings) {
        mPasswordSettings = passwordSettings;
        initializeWidget();
    }
}

void PasswordValidateWidget::addLabel(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, int row, int col, int value)
{
    auto w = new PasswordValidateLabel(this);
    w->setText(c, value);
    mWidgetMap.insert(c, w);
    mGridLayout->addWidget(w, row, col, Qt::AlignLeft);
}

void PasswordValidateWidget::updateGridPosition(int &row, int &col)
{
    col++;
    if (!(col % 2)) {
        row++;
        col = 0;
    }
}

void PasswordValidateWidget::initializeWidget()
{
    // clear grid layout
    if (!mPasswordSettings.accountsPasswordPolicyEnabled) {
        setVisible(false);
    } else {
        setVisible(true);
        int row = 0;
        int col = 0;
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks checks = mPasswordSettings.passwordValidChecks();
        RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneLowercase;
        if (checks & c) {
            addLabel(c, row, col);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneUppercase;
        if (checks & c) {
            addLabel(c, row, col);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneSpecialCharacter;
        if (checks & c) {
            addLabel(c, row, col);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneNumber;
        if (checks & c) {
            addLabel(c, row, col);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MinLengh;
        if (checks & c) {
            addLabel(c, row, col, mPasswordSettings.accountsPasswordPolicyMinLength);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MaxLengh;
        if (checks & c) {
            addLabel(c, row, col, mPasswordSettings.accountsPasswordPolicyMaxLength);
            updateGridPosition(row, col);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharactersCount;
        if (checks & c) {
            addLabel(c, row, col, mPasswordSettings.accountsPasswordPolicyForbidRepeatingCharactersCount);
        }
        c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharacters;
        if (checks & c) {
            addLabel(c, row, col, mPasswordSettings.accountsPasswordPolicyForbidRepeatingCharacters);
        }
    }
    validatePassword(QString()); // Initialize status
}

void PasswordValidateWidget::setValidStatus(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, bool b)
{
    PasswordValidateLabel *label = mWidgetMap.value(c, nullptr);
    if (label) {
        label->setValidStatus(b);
    } else {
        // qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find label for " << c;
    }
}

void PasswordValidateWidget::validatePassword(const QString &password)
{
    if (!mPasswordSettings.accountsPasswordPolicyEnabled) {
        Q_EMIT passwordIsValid(true);
        return;
    }
    const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks checks = mPasswordSettings.validatePassword(password);
    RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneLowercase;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneUppercase;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneSpecialCharacter;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneNumber;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MinLengh;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MaxLengh;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharacters;
    setValidStatus(c, checks & c);
    c = RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharactersCount;
    setValidStatus(c, checks & c);

    Q_EMIT passwordIsValid(mPasswordSettings.isValidatePassword(checks));
}

PasswordValidateLabel::PasswordValidateLabel(QWidget *parent)
    : QWidget(parent)
    , mIconLabel(new QLabel(this))
    , mLabel(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setSpacing(0);

    mIconLabel->setObjectName("mIconLabel"_L1);
    mLabel->setObjectName("mLabel"_L1);
    mainLayout->addWidget(mIconLabel);
    mainLayout->addWidget(mLabel);
}

PasswordValidateLabel::~PasswordValidateLabel() = default;

void PasswordValidateLabel::setText(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, int value)
{
    switch (c) {
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MinLengh:
        mLabel->setText(i18n("At least %1 characters", value));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::MaxLengh:
        mLabel->setText(i18n("At most %1 characters", value));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharactersCount:
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::ForbidRepeatingCharacters:
        mLabel->setText(i18n("Max. %1 repeating characters", value));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneLowercase:
        mLabel->setText(i18n("At least one lowercase letter"));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneUppercase:
        mLabel->setText(i18n("At least one uppercase letter"));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneSpecialCharacter:
        mLabel->setText(i18n("At least one symbol"));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::AtLeastOneNumber:
        mLabel->setText(i18n("At least one number"));
        break;
    case RuqolaServerConfig::PasswordSettings::PasswordSettingCheck::None:
        Q_UNREACHABLE();
        break;
    }
}

void PasswordValidateLabel::setValidStatus(bool b)
{
    mIconLabel->setPixmap(b ? QIcon::fromTheme(u"data-success"_s).pixmap(18, 18) : QIcon::fromTheme(u"data-error"_s).pixmap(18, 18));
    QPalette pal = mLabel->palette();
    pal.setColor(mLabel->foregroundRole(),
                 b ? ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::PositiveText).color()
                   : ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
    mLabel->setPalette(pal);
}

#include "moc_passwordvalidatewidget.cpp"
