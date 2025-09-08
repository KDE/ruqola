/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

/**
 * @class SessionKeyDistributionTest
 * @brief Autotest for Rocket.Chat E2EE session key distribution and acceptance/rejection flows.
 *
 * This test simulates two users in an end-to-end encrypted room:
 *
 * - User1:
 *   - Receives the suggested group (session) key.
 *   - Decrypts it with their private key.
 *   - Accepts it using AcceptSuggestedGroupKeyJob (acceptJob1).
 *
 * - User2:
 *   - Receives the suggested group (session) key.
 *   - Decrypts it with their private key.
 *   - Rejects it using RejectSuggestedGroupKeyJob (rejectJob2).
 *
 * The test verifies:
 *   - Correct encryption and decryption of the session key for both users.
 *   - Proper API communication for distributing, accepting, and rejecting session keys.
 *   - That only users with the correct private key can decrypt the session key.
 *   - That the session key is correctly assigned or rejected in the users’ room subscriptions.
 *
 *
 *  * Prerequisites:
 *
 *  - The .env file must contain credentials for at least two users (USERNAME1, PASSWORD1, USERNAME2, PASSWORD2).
 *
 *  - The test room must exist and be accessible by both users.
 */
class SessionKeyDistributionTest : public QObject
{
    Q_OBJECT
public:
    explicit SessionKeyDistributionTest(QObject *parent = nullptr);
    ~SessionKeyDistributionTest() override = default;
private Q_SLOTS:
    // void testSessionKeyDistribution();
    void testJsonPayload();
    void testCanStartValidation();
};
