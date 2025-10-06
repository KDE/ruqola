<!--
SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
SPDX-License-Identifier: MIT
-->
# E2EE TEST GUI

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Usage](#usage)

---

## Overview

This test-gui provides a simple interface to experiment with:

- Master key derivation using a password and userId as salt.
- RSA key pair generation.
- Encryption and decryption of private keys using the master key.
- Session key generation and encryption with RSA keys.
- Message encryption/decryption using a session key.
- Export/Import of public and encrypted private keys in **JWK** format.

The GUI uses **QTextEdit** for input/output, **QPushButton** for triggering actions,
and leverages the `EncryptionUtils` library for all cryptographic operations.

---

## Features

- **Master Key Derivation**: Derive a symmetric master key from a password and
  user ID (used as salt).
- **RSA Key Pair**: Generate a new RSA public/private key pair.
- **Private Key Encryption**: Encrypt the private key using the derived master
  key.
- **Session Key**: Generate a random session key for encrypting messages.
- **Message Encryption/Decryption**: Encrypt or decrypt arbitrary messages
  using the session key.
- **Export Keys**: Export the public key (JWK format) and the encrypted
  private key (JWK format).
- **Reset**: Clear all keys, messages, and intermediate data.

---

## Usage

1. Launch the application. (if you new ./build/bin/encryptiontestgui,
   if on Windows don't forget XLaunch).
2. Derive a master key using your password and user ID (salt).
3. Generate an RSA key pair.
4. Encrypt the private key using the master key.
5. Generate a session key and encrypt it with the RSA public key.
6. Encrypt a message with the session key.
7. Decrypt the session key with the RSA private key.
8. Decrypt the message with the session key.
9. Export public and encrypted private keys in **JWK** format if needed.

All outputs and results appear in the **Output** field.
