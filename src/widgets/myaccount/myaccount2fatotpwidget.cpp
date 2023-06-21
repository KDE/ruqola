/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fatotpwidget.h"
#include "ddpapi/ddpclient.h"
#include "misc/lineeditcatchreturnkey.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <prison/Prison>
#else
#include <Prison/Prison>
#endif

MyAccount2FaTotpWidget::MyAccount2FaTotpWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mQRCode(Prison::createBarcode(Prison::QRCode))
    , mTotpQrCode(new QLabel(this))
    , mQrCoreLabel(new QLabel(this))
    , mVerifyQrCode(new QLineEdit(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTotpQrCode->setObjectName(QStringLiteral("mTotpQrCode"));
    mainLayout->addWidget(mTotpQrCode);
    auto label = new QLabel(i18n("Scan the QR code. It will display a 6 digit code which you need to enter below."
                                 "\n If you can't scan the QR code, you may enter code manually instead:"),
                            this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mQrCoreLabel->setObjectName(QStringLiteral("mQrCoreLabel"));
    mainLayout->addWidget(mQrCoreLabel);
    mQrCoreLabel->setTextFormat(Qt::PlainText);
    mQrCoreLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mVerifyQrCode->setObjectName(QStringLiteral("mVerifyQrCode"));
    mVerifyQrCode->setPlaceholderText(i18n("Enter authentication code"));
    new LineEditCatchReturnKey(mVerifyQrCode, this);
    hboxLayout->addWidget(mVerifyQrCode);

    auto verifyButton = new QPushButton(i18n("Verify"), this);
    verifyButton->setObjectName(QStringLiteral("verifyButton"));
    hboxLayout->addWidget(verifyButton);
    verifyButton->setEnabled(false);
    connect(verifyButton, &QPushButton::clicked, this, &MyAccount2FaTotpWidget::slotVerify);
    connect(mVerifyQrCode, &QLineEdit::textChanged, this, [verifyButton](const QString &str) {
        verifyButton->setEnabled(!str.trimmed().isEmpty());
    });
    connect(mVerifyQrCode, &QLineEdit::returnPressed, this, &MyAccount2FaTotpWidget::slotVerify);
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::totpResult, this, &MyAccount2FaTotpWidget::slotTotpResult);
        connect(mRocketChatAccount, &RocketChatAccount::totpInvalid, this, &MyAccount2FaTotpWidget::slotTotpInvalid);
        connect(mRocketChatAccount, &RocketChatAccount::totpValid, this, &MyAccount2FaTotpWidget::slotTotpValid);
    }
    mainLayout->addStretch(1);
}

MyAccount2FaTotpWidget::~MyAccount2FaTotpWidget()
{
    delete mQRCode;
}

void MyAccount2FaTotpWidget::slotTotpValid(const QStringList &listCodes)
{
    KMessageBox::information(
        this,
        i18n("Make sure you have a copy of your codes:\n%1\nIf you lose access to your authenticator app, you can use one of these codes to log in.",
             listCodes.join(QLatin1Char('\n'))),
        i18n("Backup Codes"));
    setVisible(false); // Hide it.
}

void MyAccount2FaTotpWidget::slotTotpInvalid()
{
    KMessageBox::error(this, i18n("Invalid two factor code."), i18n("Check Two Factor Code"));
}

void MyAccount2FaTotpWidget::slotVerify()
{
    mRocketChatAccount->ddp()->validateTempToken2fa(mVerifyQrCode->text());
}

void MyAccount2FaTotpWidget::slotTotpResult(const QString &secret, const QString &url)
{
    mQrCoreLabel->setText(secret);
    mQRCode->setData(url);
    mTotpQrCode->setPixmap(QPixmap::fromImage(mQRCode->toImage(mQRCode->preferredSize(QGuiApplication::primaryScreen()->devicePixelRatio()).toSize())));
    Q_EMIT show2FaEnabledWidget();
}

#include "moc_myaccount2fatotpwidget.cpp"
