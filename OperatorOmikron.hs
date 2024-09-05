import Data.List (subsequences)

-- Funkce pro generování všech kombinací operátorů
generateExpressions :: [String] -> [String] -> [String]
generateExpressions vars ops = [buildExpr vars ops opsCombo | opsCombo <- subsequences ops]

-- Funkce pro vytvoření výrazu
buildExpr :: [String] -> [String] -> [String] -> String
buildExpr vars ops opsCombo = concat $ intercalateOp (head vars) (tail vars) opsCombo
  where
    intercalateOp :: String -> [String] -> [String] -> [String]
    intercalateOp x [] _ = [x]
    intercalateOp x (y:ys) (o:os) = (x ++ " " ++ o ++ " " ++ y) : intercalateOp y ys os

-- Hlavní funkce pro výpis všech kombinací
main :: IO ()
main = do
  let vars = ["a", "b", "g", "d"]
  let ops = ["+", "-", "*", "/"] -- Seznam operátorů, které chceš použít
  let expressions = generateExpressions vars ops
  mapM_ putStrLn expressions
