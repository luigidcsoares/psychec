// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "TestRunner.h"

#include "../C/tests/BinderTest.h"
#include "../C/tests/ParserTest.h"
#include "../C/tests/TypeCheckerTest.h"

#include <iostream>

using namespace psy;
using namespace C;

void TestRunner::runSuite()
{
    std::cout << "TESTS..." << std::endl;

    // C
    std::cout << "  C" << std::endl;
    ParserTest P;
    P.testAll();

    BinderTest B;
    B.testAll();

    P.summary();
    B.summary();
    std::cout << "Succeeded: " << P.cntOK_ + B.cntOK_ << std::endl
              << "Failed   : " << P.cntER_ + B.cntER_ << std::endl;

}
