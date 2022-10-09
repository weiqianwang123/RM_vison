//
// Created by oem on 2022/10/6.
//

#ifndef RM_VISON_BASE_H
#define RM_VISON_BASE_H


#pragma once

#include <memory>
#include <string>

namespace transporter {

    class Base
    {
    public:
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual int read(void * buffer, size_t len) = 0;
        virtual int write(const void * buffer, size_t len) = 0;
        virtual bool is_open() = 0;
        virtual std::string error_message() = 0;
    };


#endif //RM_VISON_BASE_H
