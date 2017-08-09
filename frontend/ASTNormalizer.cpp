/******************************************************************************
 Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "ASTNormalizer.h"
#include "AST.h"
#include "Assert.h"
#include "Control.h"
#include "CoreTypes.h"
#include "Debug.h"
#include "Literals.h"
#include "Scope.h"
#include "Symbols.h"
#include "SyntaxAmbiguity.h"
#include "TranslationUnit.h"
#include <algorithm>

#define VISITOR_NAME "AstNormalizer"

using namespace CPlusPlus;
using namespace psyche;

ASTNormalizer::ASTNormalizer(TranslationUnit *unit, bool employHeuristic)
    : ASTVisitor(unit)
    , employHeuristic_(employHeuristic)
{}

void ASTNormalizer::Stats::reset()
{
    resolvedAsDecl_ = 0;
    resolvedAsExpr_ = 0;
    guessedAsCall_ = 0;
    guessedAsPtrDecl_ = 0;
}

void ASTNormalizer::normalize(TranslationUnitAST *ast)
{
    if (!ast)
        return;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);
}

bool ASTNormalizer::visit(CompoundStatementAST *ast)
{
    for (StatementListAST *it = ast->statement_list; it; it = it->next) {
        maybeFixAST(it->value);
        accept(it->value);
    }
    return false;
}

bool ASTNormalizer::visit(IfStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    maybeFixAST(ast->else_statement);
    accept(ast->else_statement);
    return false;
}

bool ASTNormalizer::visit(ForStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool ASTNormalizer::visit(LabeledStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool ASTNormalizer::visit(WhileStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool ASTNormalizer::visit(SwitchStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool ASTNormalizer::visit(CaseStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool ASTNormalizer::visit(DoStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

void ASTNormalizer::maybeFixAST(StatementAST *&ast)
{
    if (!ast || !ast->asAmbiguousStatement())
        return;

    AmbiguousStatementAST* amb = ast->asAmbiguousStatement();
    std::vector<Declaration*> suspicious;
    SyntaxAmbiguity::Resolution resolution = amb->info->resolution();
    auto line = amb->info->line();

    if (resolution == SyntaxAmbiguity::Resolution::DefinitelyExpression) {
        suspicious = amb->suspiciousDecls;
        ast = amb->expressionStmt;
        printDebug("Ambiguity at %d can be disambiguated as expression\n", line);
        ++stats_.resolvedAsExpr_;
    } else if (resolution == SyntaxAmbiguity::Resolution::DefinitelyDeclaration) {
        ast = amb->declarationStmt;
        printDebug("Ambiguity at %d can be disambiguated as declaration\n", line);
        ++stats_.resolvedAsDecl_;
    } else if (employHeuristic_) {
        printDebug("Ambiguity at %d cannot be disambiguated, apply heuristics\n", line);
        SyntaxAmbiguity::Variety variety = amb->info->variety();
        if (variety == SyntaxAmbiguity::Variety::MulExpr_X_PointerDecl) {
            ast = amb->declarationStmt;
            ++stats_.guessedAsPtrDecl_;
        } else if (variety == SyntaxAmbiguity::Variety::OneArgCall_X_VarDecl) {
            suspicious = amb->suspiciousDecls;
            ast = amb->expressionStmt;
            ++stats_.guessedAsCall_;
        }
    }

    std::for_each(suspicious.begin(), suspicious.end(),
                  [this] (Declaration* decl) {
                      control()->annulSymbol(decl);
    });
}

namespace psyche {

std::ostream& operator<<(std::ostream& os, const ASTNormalizer::Stats& s)
{
    os << "  Total ambiguities  : " << s.resolvedAsDecl_
          + s.resolvedAsExpr_
          + s.guessedAsCall_
          + s.guessedAsPtrDecl_ << std::endl
       << "  Resolved as decl   : " << s.resolvedAsDecl_ << std::endl
       << "  Resolved as expr   : " << s.resolvedAsExpr_ << std::endl
       << "  Guessed as call    : " << s.guessedAsCall_ << std::endl
       << "  Guessed as ptr decl: " << s.guessedAsPtrDecl_;
    return os;
}

} // namespace psyche