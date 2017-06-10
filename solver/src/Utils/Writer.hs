-- Copyright (c) 2016 Rodrigo Ribeiro (rodrigo@decsi.ufop.br)
-- Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
-- Copyright (c) 2016 Marcus Rodrigues (demaroar@gmail.com)
--
-- This library is free software; you can redistribute it and/or modify it under
-- the terms of the GNU Lesser General Public License as published by the Free
-- Software Foundation; either version 2.1 of the License, or (at your option)
-- any later version.
--
-- This library is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
-- for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with this library; if not, write to the Free Software Foundation, Inc.,
-- 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA


module Utils.Writer where

import Data.Type
import Data.BuiltIn
import Utils.Pretty
import Solver.ConstraintSolver
import Solver.SolverMonad (TyCtx (..), VarCtx(..))
import Text.PrettyPrint.HughesPJ
import Utils.Pretty (pprint, (<+>), text)
import Utils.DeclSorter
import qualified Data.Map as Map
import qualified Data.List as List

import Debug.Trace


writeCFile :: (TyCtx, VarCtx) -> String
writeCFile ctxs@(tcx, _) =
  writeIncs
  ++ writeBoolDef
  ++ writeFwdDecls tcx
  ++ writeDecls ctxs


-- TODO: Filter includes, write only those that are needed.
writeIncs :: String
writeIncs =
  "/* WIP: In the future, only needed includes will be written. */\n" ++
  "#include <stdint.h>\n" ++
  "#include <stddef.h>\n" ++
  "#include <stdlib.h>\n" ++
  "#include <string.h>\n" ++
  "#include <sys/types.h>\n" ++
  "#include <sys/stat.h>\n" ++
  "\n"


-- TODO: By default, our generator parses a `bool' type and interpret it as an `int'.
-- We'll make this optional.
writeBoolDef :: String
writeBoolDef =
  "/* By default, we accept bool (as int). */\n" ++
  "typedef int bool;\n" ++
  "const bool false = 0;\n" ++
  "const bool true = 1;\n" ++
  "\n"


-- | Write forward declarations.
writeFwdDecls :: TyCtx -> String
writeFwdDecls tcx =
  cmt ++ Map.foldrWithKey (\k (t, _) acc -> acc ++ (genfwd k t)) [] (tyctx tcx) ++ "\n"
 where
  genfwd k (RecTy _ n)
    | isElab k = ""
    | otherwise = show $ text "typedef " <+> pprint n <+> text " " <+> pprint k <+> text ";\n"
  genfwd _ _ = ""
  cmt = "/* Forward declarations */\n"

-- | Write types and variable declarations.
writeDecls :: (TyCtx, VarCtx) -> String
writeDecls (tcx,vcx) =
  cmt1 ++ t ++ "\n" ++ cmt2 ++ v ++ "\n"
 where
  t = foldr gentydef [] (sortDecls tcx)
  gentydef (n, t) ac = (show $ writeTyDecl t n <+> text ";\n") ++ ac
  -- Constant-expression counter begins at 128 to avoid colisions with ASCII characters.
  -- Eventually, the generator will send "best" values (no duplicates risk).
  (v, _) = Map.foldrWithKey genvardef ([], 128) (varctx vcx)
  genvardef n (VarInfo t _ ro) (l, cnt)
    | ro = ((show $ writeDefine n cnt <+> text "\n") ++ l, cnt + 1)
    | otherwise = ((show $ writeVarDecl t n <+> text "\n") ++ l, cnt)
  cmt1 = "/* Type definitions */\n"
  cmt2 = "/* Variables and functions */\n"

-- | Write variable declarations.
writeVarDecl :: Ty -> Name -> Doc
writeVarDecl (FunTy ret params) n =
  pprint ret <+> pprint n <+> parens (hcat $ punctuate comma (map pprint params)) <+> text ";"
writeVarDecl t n = pprint t <+> pprint n <+> text ";"


-- | Write #defines for constant-expressions.
writeDefine :: Show a => Name -> a -> Doc
writeDefine n cnt = text "#define " <+> pprint n <+> text (show cnt)


-- | Write type declarations.
writeTyDecl :: Ty -> Name -> Doc
writeTyDecl t@(RecTy _ _) n = pprint t
writeTyDecl (FunTy t tx) n =
  text "typedef " <+> pprint t <+> text "(*" <+> pprint n <+> text ")" <+>
    parens (hcat $ punctuate comma (map pprint tx))
writeTyDecl t n = text "typedef " <+> pprint t <+> pprint (ensurePlain n)