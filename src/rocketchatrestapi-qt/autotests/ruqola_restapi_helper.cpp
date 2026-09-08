/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiabstractjob.h"
#include "restapimethod.h"
#include <QMetaType>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTest>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
namespace
{
// A job only stores plain pointers to its RestApiMethod and QNetworkAccessManager, so both must
// outlive the helper that installs them. Parenting the holder to the job ties their lifetime to
// the job the caller declared on its own stack.
class JobEnvironment : public QObject
{
public:
    explicit JobEnvironment(RestApiAbstractJob *job)
        : QObject(job)
        , mJob(job)
    {
        mMethod.setServerUrl(u"http://www.kde.org"_s);
        mJob->setRestApiMethod(&mMethod);
    }

    void installNetworkAccessManager()
    {
        mJob->setNetworkAccessManager(new QNetworkAccessManager(this));
    }

private:
    RestApiMethod mMethod;
    RestApiAbstractJob *const mJob;
};
}

namespace RuqolaRestApiHelper
{
void verifyAuthentication(RestApiAbstractJob *job, QNetworkRequest &request)
{
    const QString authToken = u"foo"_s;
    const QString userId = u"user"_s;
    job->setUserId(userId);
    job->setAuthToken(authToken);
    new JobEnvironment(job);
    request = job->request();
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
}

void verifyDefaultValue(RestApiAbstractJob *job)
{
    QVERIFY(!job->restApiMethod());
    QVERIFY(!job->networkAccessManager());
    QVERIFY(!job->start());
    QVERIFY(job->authToken().isEmpty());
    QVERIFY(job->userId().isEmpty());
    QVERIFY(!job->restApiLogger());
}

void verifyNotStartingJob(RestApiAbstractJob *job)
{
    auto environment = new JobEnvironment(job);
    environment->installNetworkAccessManager();
    QVERIFY(!job->canStart());
    job->setAuthToken(u"foo"_s);
    QVERIFY(!job->canStart());
    job->setUserId(u"foo"_s);
    QVERIFY(!job->canStart());
}
}
