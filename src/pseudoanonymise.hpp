/*
 * Copyright 2018 Internet Corporation for Assigned Names and Numbers.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*
 * Developed by Sinodun IT (www.sinodun.com)
 */

#ifndef PSEUDOANONYMISE_HPP
#define PSEUDOANONYMISE_HPP

#include <string>

#include <openssl/aes.h>

#include "bytestring.hpp"
#include "ipaddress.hpp"

/**
 * \class PseudoAnonymise
 * \brief (Pseudo)-anonymise IP addresses.
 *
 * Pseudo-anonymisation is done using AES-128 with a key. The (16 byte) key
 * may be supplied directly, or may be generate from a passphrase.
 *
 * Key generation from passphrase and pseudo-anonymisation of IPv6 addresses
 * is done using the mechanisms described in PowerDNS ipcipher. See
 * https://powerdns.org/ipcipher/. Currently there is one difference
 * compared to `ipcipher` - when generating a key from a passphrase a
 * salt of `cdnscdnscdnscdns` is used, rather than `ipcipheripcipher`.
 *
 * Pseudo-anonymisation of IPv4 addresses does not use the `ipcipher`
 * mechanism. Instead, a buffer containing 4 concatenated copies of the
 * the IPv4 address is run through AES-128 and the most significant 4
 * bytes of the result used as the pseudo-anonymised IPv4 address.
 */
class PseudoAnonymise
{
public:
    /**
     * \brief Constructor
     *
     * \param a key pass phrase.
     */
    PseudoAnonymise(const std::string& str);

    /**
     * \brief Constructor
     *
     * \param a key pass phrase.
     */
    PseudoAnonymise(const char* str, const char* salt = "cdnscdnscdnscdns");

    /**
     * \brief Constructor
     *
     * \param a 16 byte key.
     */
    PseudoAnonymise(const byte_string& key);

    /**
     * \brief Anonymise an address.
     *
     * \param addr the address to anonymise.
     * \returns the anonymised address.
     */
    IPAddress address(const IPAddress& addr) const;

    /**
     * \brief Anonymise OPT RDATA.
     *
     * Check OPT RDATA for EDNS0 client subnet options and anonymise
     * those subnet addresses.
     *
     * \param rdata the RDATA to anonymise.
     * \returns anonymised RDATA.
     */
    byte_string opt_rdata(const byte_string& rdata) const;

    /**
     * \brief Generate key from passphrase and salt.
     *
     * \param str  the passphrase.
     * \param salt the salt.
     */
    static byte_string generate_key(const char *str, const char *salt);

private:
    AES_KEY aes_key;
};

#endif
