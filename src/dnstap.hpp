/*
 * Copyright 2021 Internet Corporation for Assigned Names and Numbers.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*
 * Developed by Sinodun IT (www.sinodun.com)
 */

#ifndef DNSTAP_HPP
#define DNSTAP_HPP

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

#include "config.h"

#include "dnsmessage.hpp"

#if ENABLE_DNSTAP

/**
 * \exception dnstap_invalid
 * \brief Signals uncompliant DNSTAP input.
 *
 * Signals that the input is not conformant DNSTAP.
 */
class dnstap_invalid : public std::runtime_error
{
public:
    /**
     * \brief Constructor.
     *
     * \param what      message describing the problem.
     */
    explicit dnstap_invalid(const std::string& what)
        : std::runtime_error(what){};

    /**
     * \brief Constructor.
     *
     * \param what      message describing the problem.
     */
    explicit dnstap_invalid(const char* what)
        : std::runtime_error(what){};
};

/**
 * \class DnsTap
 * \brief Machinery for receiving DNSTAP input.
 */
class DnsTap
{
public:
    /**
     * \typedef DNSSink
     * \brief Sink function for DNS messages.
     */
    using DNSSink = std::function<void (std::unique_ptr<DNSMessage>&)>;

    /**
     * \brief Constructor.
     *
     * \param dns_sink          sink for DNS messages.
     */
    explicit DnsTap(DNSSink dns_sink);

    /**
     * \brief Process input.
     *
     * Receive and process DNSTAP until end of file.
     *
     * \param stream            DNSTAP data source.
     */
    void process_stream(std::iostream& stream);

protected:
    /**
     * \brief process control frame.
     *
     * \param stream    DNSTAP data source.
     * \param t         the control frame type.
     * \returns `false` if FINISH read.
     */
    bool process_control_frame(std::iostream& stream, uint32_t t);

    /**
     * \brief process data frame contents.
     *
     * \param msg DNS message.
     */
    void process_data_frame(std::unique_ptr<DNSMessage> msg);

    /**
     * \brief read a control frame and return its type.
     *
     * \param stream    DNSTAP data source.
     * \returns control frame type.
     */
    uint32_t read_control_frame(std::iostream& stream);

    /**
     * \brief read DNS message from data frame.
     *
     * \param stream    DNSTAP data source.
     * \param len       length of data.
     */
    std::unique_ptr<DNSMessage> read_data_frame(std::iostream& stream, uint32_t len);

    /**
     * \brief send a control message
     *
     * \param stream     DNSTAP data source.
     * \param msg        the message.
     * \param ignore_err ignore any send errors.
     */
    void send_control(std::iostream& stream, const std::string& msg, bool ignore_err = false);

    /**
     * \brief receive 4 byte bigendian value.
     *
     * \param stream    DNSTAP data source.
     * \returns the value.
     */
    uint32_t get_value(std::iostream& stream);

    /**
     * \brief receive buffer of given size.
     *
     * \param stream    DNSTAP data source.
     * \param len       length of buffer to receive.
     * \returns the data buffer.
     */
    std::string get_buffer(std::iostream& stream, uint32_t len);

    /**
     * \brief make an ACCEPT frame.
     *
     * \returns the ACCEPT frame.
     */
    static std::string make_accept();

    /**
     * \brief make a FINISH frame.
     *
     * \returns the FINISH frame.
     */
    static std::string make_finish();

private:
    /**
     * \brief use bidirectional transmission?
     */
    bool bidirectional_;

    /**
     * \brief sink function for read DNS messages.
     */
    DNSSink dns_sink_;

    /**
     * \brief frame processing state
     */
    int state_;
};

#endif

#endif
