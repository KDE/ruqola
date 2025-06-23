/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fatotpwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
#include <Prison/Prison>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

MyAccount2FaTotpWidget::MyAccount2FaTotpWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mQRCode(Prison::Barcode::create(Prison::QRCode))
    , mTotpQrCode(new QLabel(this))
    , mQrCoreLabel(new QLabel(this))
    , mVerifyQrCode(new QLineEdit(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTotpQrCode->setObjectName(u"mTotpQrCode"_s);
    mainLayout->addWidget(mTotpQrCode);
    auto label = new QLabel(i18nc("@label:textbox",
                                  "Scan the QR code. It will display a 6 digit code which you need to enter below."
                                  "\n If you can't scan the QR code, you may enter code manually instead:"),
                            this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    mQrCoreLabel->setObjectName(u"mQrCoreLabel"_s);
    mainLayout->addWidget(mQrCoreLabel);
    mQrCoreLabel->setTextFormat(Qt::PlainText);
    mQrCoreLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mVerifyQrCode->setObjectName(u"mVerifyQrCode"_s);
    mVerifyQrCode->setPlaceholderText(i18nc("@info:placeholder", "Enter authentication code"));
    KLineEditEventHandler::catchReturnKey(mVerifyQrCode);
    hboxLayout->addWidget(mVerifyQrCode);

    auto verifyButton = new QPushButton(i18nc("@action:button", "Verify"), this);
    verifyButton->setObjectName(u"verifyButton"_s);
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

MyAccount2FaTotpWidget::~MyAccount2FaTotpWidget() = default;

void MyAccount2FaTotpWidget::slotTotpValid(const QStringList &listCodes)
{
    KMessageBox::information(
        this,
        i18n("Make sure you have a copy of your codes:\n%1\nIf you lose access to your authenticator app, you can use one of these codes to log in.",
             listCodes.join(u'\n')),
        i18nc("@title:window", "Backup Codes"));
    setVisible(false); // Hide it.
}

void MyAccount2FaTotpWidget::slotTotpInvalid()
{
    KMessageBox::error(this, i18n("Invalid two factor code."), i18nc("@title:window", "Check Two Factor Code"));
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
