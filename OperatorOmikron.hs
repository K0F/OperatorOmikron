import Data.List (subsequences)


generateExpressions :: [String] -> [String] -> [String]
generateExpressions vars ops = [buildExpr vars ops opsCombo | opsCombo <- subsequences ops]


buildExpr :: [String] -> [String] -> [String] -> String
buildExpr vars ops opsCombo = concat $ intercalateOp (head vars) (tail vars) opsCombo
  where
    intercalateOp :: String -> [String] -> [String] -> [String]
    intercalateOp x [] _ = [x]
    intercalateOp x (y:ys) (o:os) = (x ++ " " ++ o ++ " " ++ y) : intercalateOp y ys os


main :: IO ()
main = do
  let vars = ["a", "b", "c", "d"]
  let ops = ["+", "-", "*", "/"] 
  let expressions = generateExpressions vars ops
  mapM_ putStrLn expressions
