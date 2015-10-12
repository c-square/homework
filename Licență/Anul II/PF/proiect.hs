{- 
Expression Parser and Evaluator
- by Alex Citea
-}
import System.IO
import System.Environment
import System.Exit


------------
-- COMMON --
------------
-- Apendeaza primul argument la ultimul element 
-- din lista primita ca al doilea argument
appendToLast :: String -> [String] -> [String]
appendToLast a [] = []
appendToLast a (h:[]) = (h ++ a):[]
appendToLast a (h:t) = h:(appendToLast a t)

-- Returns True if it finds the first arguent in the list received as the second argument
myFind :: Eq a => a -> [a] -> Bool
myFind a [] = False
myFind a (h:t) = if (a==h) then True else (myFind a t)

-- Eliminates all of the last objects of an array if they are empty
cleanArray :: [String] -> [String]
cleanArray [] = []
cleanArray a | ((last a) == "") = cleanArray (init a)
			 | True = a


-- Verifies if a string is a number
isNum :: String -> Bool
isNum [] = False
isNum (h:t) = if ( (h >= '0' && h <= '9') || h == '.') then
					if (t == []) then 
						True
					else 
						isNum t
			  else
			  		False


trei :: (a,b,c) -> c
trei (a,b,c) = c

doi :: (a,b,c) -> b
doi (a,b,c) = b

unu :: (a,b,c) -> a
unu (a,b,c) = a



------------------------------------------------
showError :: Int -> IO ()
showError 1 = throwInvalidStack
showError 2 = throwNumberOverwrite
showError 3 = throwNumberOverwriteWithVar
showError 4 = throwErrorOnEqual
showError 5 = throwNotRecognized
showError 6 = throwVarNotGood
showError 7 = throwVarNotDeclared
showError 8 = throwErrorOnOperation
showError 9 = throwNonValidExpressions


-- ERRORS !!! ---
throwInvalidStack :: IO ()
throwInvalidStack = (putStrLn  "[Error] To many numbers in stack at evaluation end - invalid expression")

throwNumberOverwrite :: IO ()
throwNumberOverwrite = putStrLn  "[Error] Can't give a number a value"

throwNumberOverwriteWithVar :: IO ()
throwNumberOverwriteWithVar = putStrLn  "[Error] Can't give a number a variable value"
 
throwErrorOnEqual :: IO ()
throwErrorOnEqual = (putStrLn  "[Error] Can't do operation =. Not enough elements in stack")

throwNotRecognized :: IO ()
throwNotRecognized = (putStrLn  "[Error] Operator not recognized")

throwVarNotGood :: IO ()
throwVarNotGood = (putStrLn  "[Error] Variables can't start with a number")

throwVarNotDeclared :: IO ()
throwVarNotDeclared = putStrLn  ("[Error] Variable not declared")

throwErrorOnOperation :: IO ()
throwErrorOnOperation = putStrLn  ("[Error] Can't do operation. Not enough elements in stack")

throwNonValidExpressions :: IO ()
throwNonValidExpressions = putStrLn  ("[Error] Non valid expression!")




-- Variable Cache Functions --
------------------------------

-- Find an item in a [(String, String)]
findCacheItem :: String -> [(String, String)] -> Bool
findCacheItem needle [] = False
findCacheItem needle (h:t) = if ((fst h) == needle) then 
								True 
							else 
								(findCacheItem needle t)

-- Returns the value of a CacheItem
getCacheItem :: String -> [(String, String)] -> String
getCacheItem needle [] = "0"
getCacheItem needle (h:t) = if ((fst h) == needle) then 
								(snd h) 
							else 
								(getCacheItem needle t)


-- Adds an element to the cache
-- If it exists, it does an update
addToCache :: String -> String -> [(String, String)] -> [(String, String)]
addToCache key value [] = [(key, value)]
addToCache key value cache = 	if (findCacheItem key cache) then
									(updateCache key value cache)
								else
									(cache ++ [(key, value)])


-- Update an element in cache
updateCache :: String -> String -> [(String, String)] -> [(String, String)]
updateCache key value [] = []
updateCache key value (h:t) = 	if ((fst h) == key) then
								 	[(key, value)] ++ t
								else
									([h] ++ (updateCache key value t))




-- ISOLATE EXPRESSION COMPONENTS --
-- isolates each component of the expression into its own string --
isolateComp :: String -> [String] 
isolateComp [] = []
isolateComp "\n" = []
isolateComp (h:t) = buildIsolation [[h]] t

buildIsolation :: [String] -> String -> [String] 
buildIsolation retList [] = retList
buildIsolation retList (crt:left) | (crt == '(' || 
								     crt == '+' ||
								     crt == '-' ||
								     crt == '*' ||
								     crt == '/' ||
								     crt == '=') = buildIsolation ((cleanArray retList) ++ [[crt],[]]) left
								  | (crt == ')') = buildIsolation (retList ++ [[crt]]) left
								  | (crt == ' ' ||
								  	 crt == '\t'||
								  	 crt == '\n') = buildIsolation retList left
								  | True = buildIsolation (appendToLast [crt] retList) left

-- CONVERTING TO POSTFIX ALG --
-- Converteste o lista de componente ale unei expresii
-- intr-o lista de string-uri ce reprezinta expresia in forma postfixa
isOperator :: String -> Bool
isOperator [] = False
isOperator op = myFind op ["+", "-", "*", "/", "sqrt", "sin", "cos", "(", ")", "="]

getPrec "=" = 1
getPrec "+" = 2
getPrec "-" = 2
getPrec "*" = 3
getPrec "/" = 3
getPrec "sqrt" = 4
getPrec "sin" = 4
getPrec "cos" = 4
getPrec "(" = 9
getPrec ")" = 0

getPrec2 "(" = 0
getPrec2 x = getPrec x

convertToPostfix :: [String] -> [String]
convertToPostfix [] = []
convertToPostfix components = buildPostfix [] components 0

buildPostfix :: [String] -> [String] -> Int -> [String] -- MODE folosit pentru paranteze
buildPostfix stack [] 0 = reverse stack
buildPostfix stack left 1 = if ((last stack) == "(") then    -- punem in postfix toti operatorii pana la '('
								(buildPostfix (init stack) left 0)
							 else
							 	([last stack]) ++ (buildPostfix (init stack) left 1)

buildPostfix stack (headComp:otherComp) mode    | (isOperator headComp) = if (stack == []) then -- daca e stiva goala punem direct
																	  		(buildPostfix [headComp] otherComp 0)
															 	  else if (getPrec2 (last stack) < getPrec headComp) then -- daca avem operatori cu precedenta mai mica in stiva
															 		  		(buildPostfix (stack ++ [headComp]) otherComp 0)
															 	  else if (headComp == ")") then -- punem in postfix tot ce era intre paranteze
															 	  			(buildPostfix (stack) (otherComp) 1)
															 	  else if (headComp == "(") then
															 	  			(buildPostfix (stack ++ [headComp]) otherComp 0)
															 	  else
															 		  		([last stack]) ++ (buildPostfix (init stack) (headComp:otherComp) 0)
											    | True = [headComp] ++ (buildPostfix stack otherComp 0)


-- Evaluation Function --
-- Evaluates a postfix notation expression
--------------------------------------------------
getValFromStack :: [String] -> [(String, String)] -> String -> (Double, Int)
getValFromStack [] [] which = (0, 0)
getValFromStack [] cache which = (0, 0)
getValFromStack stack [] which = if (which == "semi") then
									(getValFromStack (init stack) [] "last")
								else
									if (isNum (last stack)) then
										((read (last stack) :: Double), 0)
									else
										(0, 7)

getValFromStack stack cache which = if (which == "semi") then
										(getValFromStack (init stack) cache "last")
									else
										if (isNum (last stack)) then
											((read (last stack) :: Double), 0)
										else
											if (findCacheItem (last stack) cache) then
												((read (getCacheItem (last stack) cache) :: Double), 0)
											else
												(0, 7)
												



evaluate :: [String] -> [(String, String)] -> [String] -> ([(String, String)], Double, Int)
-- params: 1. Postfix expression
--		   2. [(String, String)]
--         3. Stack
-- returs: ([(String, String)] - updated, result of expression, ErrorCode)
evaluate [] cache stack =   if ((length stack) > 1) then
								([], 0, 1)
						  	else
						  		let
						  		{
						  			val = getValFromStack stack cache "last";
						  			error = snd val;
						  			result = fst val;
						  		}
						  		in 
						  		(cache, result, error) -- de returnat ce e in varful stivei

evaluate (h:s) cache stack = 	if (isNum h) then -- daca avem numar
									(evaluate s cache (stack ++ [h])) -- pun pe stiva numarul
								else 
								if (isOperator h) then
									-- EGAL --
									if (h == "=") then 
										if ((length stack) >= 2) then 
											equalCase s cache stack
										else
											([], 0, 4)
									-- SQRT --
									else 
									if (h == "sqrt" ||
										h == "sin"  ||
										h == "cos") then
										if ((length stack) < 1) then 
											([], 0, 8)
										else
											if (h == "sqrt") then
												let 
												{
													newVal = sqrt(fst (getValFromStack stack cache "last"));
												}
												in (evaluate s cache ((init stack) ++ [show (newVal)]) )
											else 
											if (h == "sin") then
												let 
												{
													newVal = sin(fst (getValFromStack stack cache "last"));
												}
												in (evaluate s cache ((init stack) ++ [show (newVal)]) )
											else
												let 
												{
													newVal = cos(fst (getValFromStack stack cache "last"));
												}
												in (evaluate s cache ((init stack) ++ [show (newVal)]) )
									-- OTHER OPERATORS --
									else
										if ((length stack) < 2) then
											([], 0, 8)
										else
											normalCase (h:s) cache stack
																
								-- VARIABILA --
								else -- adaugam pe stiva
									if ( (head h) >= '0' && (head h) <= '9') then
										([], 0, 6)
									else
										let 
										{
											newStack = stack ++ [h];
										}
										in (evaluate s cache newStack)


-- Normal Op Case --
normalCase :: [String] -> [(String, String)] -> [String] -> ([(String, String)], Double, Int)
normalCase (h:s) cache stack = 	if (h == "+") then
									let 
									{
										var1 = (getValFromStack stack cache "semi");
										var2 = (getValFromStack stack cache "last");
									}
									in (
										if ((snd var1) == 0 && (snd var2) == 0) then
											let 
											{
												newStack = ((init (init stack)) ++ [show ( (fst var1) + (fst var2) )]);
											}
											in (evaluate s cache newStack)
										else
											if ((snd var1) == 0) then
												([], 0, snd var1)
											else
												([], 0, snd var2)
										)
								else
								if (h == "-") then
									let 
									{
										var1 = (getValFromStack stack cache "semi");
										var2 = (getValFromStack stack cache "last");
									}
									in (
										if ((snd var1) == 0 && (snd var2) == 0) then
											let 
											{
												newStack = ((init (init stack)) ++ [show ( (fst var1) - (fst var2) )]);
											}
											in (evaluate s cache newStack)
										else
											if ((snd var1) == 0) then
												([], 0, snd var1)
											else
												([], 0, snd var2)
										)
								else
								if (h == "*") then
									let 
									{
										var1 = (getValFromStack stack cache "semi");
										var2 = (getValFromStack stack cache "last");
									}
									in (
										if ((snd var1) == 0 && (snd var2) == 0) then
											let 
											{
												newStack = ((init (init stack)) ++ [show ( (fst var1) * (fst var2) )]);
											}
											in (evaluate s cache newStack)
										else
											if ((snd var1) == 0) then
												([], 0, snd var1)
											else
												([], 0, snd var2)
										)
								else
								if (h == "/") then
									let 
									{
										var1 = (getValFromStack stack cache "semi");
										var2 = (getValFromStack stack cache "last");
									}
									in (
										if ((snd var1) == 0 && (snd var2) == 0) then
											let 
											{
												newStack = ((init (init stack)) ++ [show ( (fst var1) / (fst var2) )]);
											}
											in (evaluate s cache newStack)
										else
											if ((snd var1) == 0) then
												([], 0, snd var1)
											else
												([], 0, snd var2)
										)
								else
									([], 0, 5)


-- EQUAL Case --
equalCase :: [String] -> [(String, String)] -> [String] -> ([(String, String)], Double, Int)
equalCase s cache stack = 	let
							{
								newStack = init (init stack);	
							}
							in (
									if (isNum (last stack) &&
									    isNum (last (init stack) ) 
									   ) then 
									   	([], 0, 2)		   	
									else
									-- daca primul element e o variabila si al doilea un numar
									if (isNum (last stack )) then
										-- adaugam variabila in cache
										let 
										{
											key = (last (init stack));
											value = last stack;
										}
										in evaluate s (addToCache key value cache) newStack
														
									else
									-- daca primul element e un numar si al doilea o variabila
									if (isNum (last (init stack) )) then
										([], 0, 3)
									else
									-- daca ambele elemente sunt variabile
										if (findCacheItem (last stack) cache) then
											let 
											{
												key = last (init stack);
												value = getCacheItem (last stack) cache;
											}
											in evaluate s (addToCache key value cache) newStack
										else
											([], 0, 7)
								)



																

-- Validation --
validate :: [String] -> Int
validate [] = 0
validate (h:t) = if (h == "") then
					9
				else
					validate t			


-- THE PROCESSING LOOP --
process :: Handle -> IO ()
process inHandle = processExps inHandle []

processExps :: Handle -> [(String, String)] -> IO ()
processExps inHandle cache = do {
								eof <- hIsEOF inHandle;
								if eof
									then return ()
								else
									do
									{
										exp <- hGetLine(inHandle);
										print exp;

										let 
										{
											converted = convertToPostfix(isolateComp exp);
											error = validate (converted);
											result = if (error == 0) then 
														evaluate converted cache []
													else
														([], 0, error)
										}
										in (showResultAndContinue inHandle result);
									};
								}

showResultAndContinue :: Handle -> ([(String, String)], Double, Int) -> IO ()
showResultAndContinue inHandle result = do {
												if ((trei result) /= 0) then
													showError (trei result)
												else
													do 
													{
														print (doi result);
														processExps inHandle (unu result);
													};
											}



-- MAIN FUNCTION --
main :: IO ()
main = do {
		cmdArgs <- getArgs;
    	fHandle <- openFile (cmdArgs !! 0) ReadMode;

		process fHandle;

		hClose fHandle;
		} 