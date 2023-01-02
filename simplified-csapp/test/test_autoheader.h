#pragma once

#ifndef CSAPP_TEST_AUTO_HEADER_H
#define CSAPP_TEST_AUTO_HEADER_H

#include <catch2/catch_session.hpp>     // Catch2 with custom main
#include <catch2/catch_test_macros.hpp> // Catch2 test macros

extern "C"
{
#include <csapp/s_csapp.h>
}

#endif // CSAPP_TEST_AUTO_HEADER_H
