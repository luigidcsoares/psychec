// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_VALUE_SYMBOL_H__
#define PSYCHE_C_VALUE_SYMBOL_H__

#include "API.h"
#include "Fwds.h"
#include "Symbol.h"
#include "TypeClass_NameableSymbol.h"
#include "TypeClass_TypeableSymbol.h"
#include "ValueKind.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The ValueSymbol class.
 */
class PSY_C_API ValueSymbol : public Symbol
                            , public TypeClass_NameableSymbol
                            , public TypeClass_TypeableSymbol
{
public:
    virtual ~ValueSymbol();

    virtual ValueSymbol* asValue() override { return this; }
    virtual const ValueSymbol* asValue() const override { return this; }

    virtual FieldSymbol* asField() { return nullptr; }
    virtual const FieldSymbol* asField() const { return nullptr; }
    virtual VariableSymbol* asVariable() { return nullptr; }
    virtual const VariableSymbol* asVariable() const { return nullptr; }
    virtual ParameterSymbol* asParameter() { return nullptr; }
    virtual const ParameterSymbol* asParameter() const { return nullptr; }

    /**
     * The (value) kind of \c this value.
     */
    ValueKind valueKind() const;

    /**
     * The SymbolName of \c this Symbol.
     */
    const SymbolName* name() const;

    /**
     * The type of \c this value.
     */
    const TypeSymbol* type() const;

protected:
    DECL_PIMPL_SUB(ValueSymbol);

    friend class Binder;

    ValueSymbol(const SyntaxTree* tree,
                const Scope* outerScope,
                const Symbol* containingSym,
                ValueKind valKind);

private:
    virtual void setType(const TypeSymbol* tySym) override;
    virtual void setName(std::unique_ptr<SymbolName> symName) override;
};

} // C
} // psy

#endif
