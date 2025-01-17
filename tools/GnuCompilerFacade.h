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

#ifndef PSYCHE_GNU_COMPILER_FACADE_H__
#define PSYCHE_GNU_COMPILER_FACADE_H__

#include <string>
#include <utility>
#include <vector>

namespace psy {

class GnuCompilerFacade
{
public:
    GnuCompilerFacade(const std::string& compilerName,
                   const std::string& std,
                   const std::vector<std::string>& D,
                   const std::vector<std::string>& U);

    std::pair<int, std::string> preprocess(const std::string& srcText);
    std::pair<int, std::string> preprocess_IgnoreIncludes(const std::string& srcText);

private:
    std::string assembleMacroCmd() const;

    std::string compilerName_;
    std::string std_;
    std::vector<std::string> D_;
    std::vector<std::string> U_;
};

} // psy

#endif
