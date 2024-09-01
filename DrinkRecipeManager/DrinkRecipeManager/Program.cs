using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using System.Linq;

namespace DrinkRecipeManager
{
    class Program
    {
        static List<string> recipes;
        static int numberOfErrors = 0;
        static ConsoleKeyInfo keyPress;
        static string textEntered;
        static Regex allowedChars = new Regex(@"^[a-zA-Z0-9 ]+$");

        static void Main(string[] args)
        {
            //First check recipes for errors
            if (!LoadAndCheckFile())
            {
                Console.ReadKey();
                return;
            }
            else if (numberOfErrors > 0)
            {
                Console.WriteLine();
                Console.Write("Total Errors: ");
                Console.WriteLine(numberOfErrors);
                Console.ReadKey();
                return;
            }

            //Everything Fine -> Menu
            Console.WriteLine("File OK");
            Console.WriteLine();
            
            while (true)
            {
                Console.WriteLine("Choose one of the following:");
                Console.WriteLine("----------------------------");
                Console.WriteLine("1) Add Recipe");
                Console.WriteLine("2) Delete Recipe");
                Console.WriteLine("3) Change Recipe");
                Console.WriteLine("x) Exit");
                keyPress = Console.ReadKey();
                
                if(keyPress.KeyChar == 'x' || keyPress.KeyChar == 'X')
                    break;

                if (keyPress.KeyChar == '1')
                    AddRecipe();
                else if (keyPress.KeyChar == '2')
                    DeleteRecipe();
                else if (keyPress.KeyChar == '3')
                    ChangeRecipe();

                Console.Clear();
            }          
        }
   
        static bool LoadAndCheckFile()
        {
            Console.WriteLine("Opening File 'Drinks.TXT' - Make sure file is in same folder as DrinkRecipeManager.exe");
            Console.WriteLine();

            try
            {
                recipes = File.ReadAllLines(@"Drinks.TXT")?.ToList();
                Console.Clear();
                int lineNumber = 0;
                int recipeNumber;
                int procentageNumber;
                int ingredientNumber;
                byte[] totalProcentageArray = new byte[8] {0,0,0,0,0,0,0,0};
                int totalProcentage;

                
                
                foreach (string recipe in recipes)
                {
                    lineNumber++;
                    Array.Clear(totalProcentageArray, 0, 8);
                    totalProcentage = 0;

                    //Check Recipe Length
                    if (recipe.Length != 100)
                    {
                        Console.Write("Wrong lenght in line: ");
                        Console.WriteLine(lineNumber);
                        numberOfErrors++;
                        break;
                    }

                    //Check number
                    if(Int32.TryParse(recipe.Substring(0,4), out recipeNumber))
                    {
                        if(recipeNumber != lineNumber)
                        {
                            Console.Write("Wrong number in line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }

                    }
                    else
                    {
                        Console.Write("Number Error in line: ");
                        Console.WriteLine(lineNumber);
                        numberOfErrors++;
                    }

                    //Check Recipe Name
                    if(!NameOkay(recipe.Substring(4, 16)))
                    {
                        Console.Write("Name contains illegal characters in line: ");
                        Console.WriteLine(lineNumber);
                        numberOfErrors++;
                    }

                    //Check if theres any drink with the same Name
                    for(int i = 1; i<lineNumber; i++)
                    {
                        if(recipes[i-1].Substring(4, 16) == recipe.Substring(4,16))
                        {
                            Console.Write("Name in line: ");
                            Console.Write(lineNumber);
                            Console.WriteLine(" ... has already been used");
                            numberOfErrors++;
                        }
                    }                   
                    
                    int[] ingredientNumbers = new int[8] {0,0,0,0,0,0,0,0};
                    for (int i = 0; i<8; i++)
                    {
                        //Check [ and (
                        if (recipe[20+10*i] == '[' || recipe[20 + 10 * i] == '(') 
                        {
                            //Proceed
                        }
                        else
                        {
                            Console.Write("( or [ doesn't match on line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }

                        //Check ] and )
                        if (recipe[29 + 10 * i] == ']' || recipe[29 + 10 * i] == ')')
                        {
                            //Proceed
                        }
                        else
                        {
                            Console.Write(") or ] doesn't match on line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }

                        //Check %-char
                        if (recipe[24 + 10 * i] != '%')
                        {
                            Console.Write("% doesn't match on line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }

                        //Check %-numbers & add to array for sum-calculation
                        if (Int32.TryParse(recipe.Substring(21 + 10 * i, 3), out procentageNumber))
                        {
                            if (procentageNumber < 0 || procentageNumber > 100)
                            {
                                Console.Write("Procentage Number too HIGH or LOW in line: ");
                                Console.WriteLine(lineNumber);
                                numberOfErrors++;
                            }
                            else
                            {
                                totalProcentageArray[i] = (byte)procentageNumber;
                            }
                        }
                        else
                        {
                            Console.Write("Wrong Procentage Number in line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }


                        //Check ingredient-numbers
                        if (Int32.TryParse(recipe.Substring(25 + 10 * i, 4), out ingredientNumber))
                        {
                            if (ingredientNumber < 0)
                            {
                                Console.Write("Ingreedient number is lower than 0 in line: ");
                                Console.WriteLine(lineNumber);
                                numberOfErrors++;
                            }

                            if (ingredientNumber > recipes.Count)
                            {
                                Console.Write("Ingreedient number doesnt exist in line: ");
                                Console.WriteLine(lineNumber);
                                numberOfErrors++;
                            }

                            ingredientNumbers[i] = ingredientNumber;
                        }
                        else
                        {
                            Console.Write("Wrong Ingredient Number in line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                        }                   
                    }

                    var hashSet = new HashSet<double>();
                    foreach (var x in ingredientNumbers)
                    {
                        if (!hashSet.Add(x) && x != 0)
                        {
                            Console.Write("2+ Instances of same ingredient in line: ");
                            Console.WriteLine(lineNumber);
                            numberOfErrors++;
                            break;
                        }
                    }

                    //Check Total Procentage
                    for (int i = 0; i<8; i++)
                    {
                        totalProcentage += totalProcentageArray[i];
                    }
                    if(totalProcentage == 0 || totalProcentage == 100)
                    {
                        //Proceed
                    }
                    else
                    {
                        Console.Write("Total Procentage isn't 0 or 100 on line: ");
                        Console.WriteLine(lineNumber);
                        numberOfErrors++;
                    }
                }


                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine($"{e.GetType().Name}: {e.Message}");
            }

            return false;
        }

        static bool NameOkay(string name)
        {           
            if (!allowedChars.IsMatch(name))
            {
                return false;
            }

            return true;
        }

        static bool NameAlreadyUsed(string name)
        {
            foreach(string recipe in recipes)
            {
                if(name == recipe.Substring(4, 16))
                {
                    return true;
                }
            }

            return false;
        }

        static void AddRecipe()
        {
            string drinkName = "";
            int[] ingredientNumbers = new int[8] { 0, 0, 0, 0, 0, 0, 0, 0 }; //Ingreedient numbers
            byte[] procentNumbers = new byte[8] { 0, 0, 0, 0, 0, 0, 0, 0 }; //Amount in procentage
            bool[] drinkIsStrong = new bool[8] { false, false, false, false, false, false, false, false };

            //Find DrinkName
            while (true)
            {
                Console.Clear();
                Console.WriteLine("Write name of the drink. Enter empty name to exit without adding a new recipe.");
                Console.WriteLine("-------------------------------------------------------------------------");
                Console.Write("Drink Name: ");
                Console.Write(drinkName);
                keyPress = Console.ReadKey();

                if (keyPress.Key == ConsoleKey.Enter)
                {
                    if (drinkName == "")
                    {
                        return;
                    }
                    else
                    {
                        //Add white space so name length is 16
                        while(drinkName.Length<16)
                        {
                            drinkName += " ";
                        }

                        if(NameOkay(drinkName) && !NameAlreadyUsed(drinkName))
                        {
                            while (true)
                            {
                                Console.Clear();
                                Console.WriteLine("Want to proceed with drink name: {0}?", drinkName.TrimEnd());
                                Console.WriteLine("------------------------------");
                                Console.WriteLine("y) Yes");
                                Console.WriteLine("n) No");
                                keyPress = Console.ReadKey();
                                if(keyPress.KeyChar == 'n' || keyPress.KeyChar == 'N')
                                {
                                    break;
                                }
                                else if(keyPress.KeyChar == 'y' || keyPress.KeyChar == 'Y')
                                {
                                    int currentIngreedientNumber = 0;
                                    byte currentProcentage = 0;
                                    byte amountLeft = 100;

                                    //Find ingredients, amount & if strong
                                    for(int i = 1; i<=8; i++)
                                    {
                                        Console.Clear();
                                        Console.WriteLine("Choose ingreedient {0} in: {1}", i, drinkName);
                                        Console.WriteLine("Enter x to exit without adding recipe");
                                        if(i == 1)
                                            Console.WriteLine("Enter s to skip if there's no ingredients");
                                        Console.WriteLine("------------------------");
                                        for(int e = 0; e < 8; e++)
                                        {
                                            if (ingredientNumbers[e] > 0)
                                            {
                                                Console.Write(ingredientNumbers[e]);
                                                Console.Write(". ");
                                                Console.Write(recipes[ingredientNumbers[e]-1].Substring(4, 16));
                                                Console.Write(" ");
                                                Console.Write(procentNumbers[e]);
                                                Console.Write("%");
                                                Console.Write(" ");
                                                Console.WriteLine(drinkIsStrong[e] ? "STRONG" : "SOFT");
                                            }                                                  
                                        }
                                        Console.WriteLine("------------------------");
                                        for (int lineNumber = 1; lineNumber<=recipes.Count; lineNumber++)
                                        {   if (ingredientNumbers.Contains(lineNumber))
                                                continue;
                                            Console.Write(lineNumber);
                                            Console.Write(") ");
                                            Console.WriteLine(recipes[lineNumber-1].Substring(4,16));
                                        }
                                        textEntered = Console.ReadLine();
                                        if(textEntered == "x" || textEntered == "X")
                                        {
                                            return;
                                        }
                                        else if (textEntered == "s" || textEntered == "S")
                                        {
                                            if(amountLeft == 100)
                                            {
                                                break;                                               
                                            }
                                            else
                                            {
                                                i--; //Input Number again
                                                continue;
                                            }
                                            
                                        }
                                        else if(Int32.TryParse(textEntered, out currentIngreedientNumber))
                                        {
                                            if(currentIngreedientNumber > 0 && currentIngreedientNumber <= recipes.Count && !ingredientNumbers.Contains(currentIngreedientNumber))
                                            {
                                                //Recipe number okay
                                                ingredientNumbers[i-1] = currentIngreedientNumber;
                                                
                                                //Find Amount
                                                while(true)
                                                {
                                                    Console.Clear();
                                                    Console.WriteLine("Choose amount (0-{0})% of {1}",amountLeft , recipes[currentIngreedientNumber - 1].Substring(4, 16));
                                                    Console.WriteLine("------------------------");

                                                    textEntered = Console.ReadLine();

                                                    if (byte.TryParse(textEntered, out currentProcentage))
                                                    {
                                                        if(currentProcentage >= 0 && currentProcentage <= amountLeft)
                                                        {
                                                            procentNumbers[i-1] = currentProcentage;
                                                            amountLeft -= currentProcentage;
                                                            while (true)
                                                            {
                                                                Console.Clear();
                                                                Console.WriteLine("Is {0} a soft (x) or strong (y) ingreedient?", recipes[currentIngreedientNumber - 1].Substring(4, 16).TrimEnd());
                                                                keyPress = Console.ReadKey();
                                                                if(keyPress.KeyChar == 'x' || keyPress.KeyChar == 'X')
                                                                {
                                                                    drinkIsStrong[i - 1] = false;
                                                                    break;
                                                                }
                                                                else if(keyPress.KeyChar == 'y' || keyPress.KeyChar == 'Y')
                                                                {
                                                                    drinkIsStrong[i - 1] = true;
                                                                    break;
                                                                }
                                                            }
                                                                                                                       
                                                            break;
                                                        }
                                                    }
                                                }                                            
                                            }
                                            else
                                            {
                                                i--;
                                                continue;
                                            }
                                        }
                                        else
                                        {
                                            i--; //Input Number again
                                            continue;
                                        }
                                        
                                        if(amountLeft == 0)
                                        {
                                            break;
                                        }
                                    }

                                    //Ask if drink is okay
                                    while(true)
                                    {
                                        Console.Clear();
                                        Console.WriteLine("Is {0} ingreedients right?", drinkName.TrimEnd());
                                        Console.WriteLine("-------------------------------------");
                                        for (int i = 0; i < 8; i++)
                                        {
                                            if (ingredientNumbers[i] > 0)
                                            {
                                                Console.Write(ingredientNumbers[i]);
                                                Console.Write(". ");
                                                Console.Write(recipes[ingredientNumbers[i] - 1].Substring(4, 16));
                                                Console.Write(" ");
                                                Console.Write(procentNumbers[i]);
                                                Console.Write("%");
                                                Console.Write(" ");
                                                Console.WriteLine(drinkIsStrong[i] ? "STRONG" : "SOFT");
                                            }
                                        }
                                        Console.WriteLine("-------------------------------------");
                                        Console.WriteLine("y) Yes");
                                        Console.WriteLine("n) No");
                                        keyPress = Console.ReadKey();
                                        if (keyPress.KeyChar == 'y' || keyPress.KeyChar == 'Y')
                                        {
                                            //Create Recipe Text
                                            string recipeText = "";
                                            recipeText += (recipes.Count+1).ToString().PadLeft(4, '0');
                                            recipeText += drinkName;
                                            for(int i = 0; i<8; i++)
                                            {
                                                recipeText += drinkIsStrong[i] ? "[" : "(";
                                                recipeText += procentNumbers[i].ToString().PadLeft(3, '0');
                                                recipeText += '%';
                                                recipeText += ingredientNumbers[i].ToString().PadLeft(4, '0');
                                                recipeText += drinkIsStrong[i] ? "]" : ")";
                                            }
                                            recipes.Add(recipeText);

                                            //Add Recipe Text to document
                                            File.WriteAllLines(@"Drinks.TXT", recipes);
                                            return;
                                        }
                                        else if (keyPress.KeyChar == 'n' || keyPress.KeyChar == 'N')
                                        {
                                            break;
                                        }
                                    }                                                                     
                                }
                            }                           
                        }

                        drinkName = drinkName.TrimEnd();
                    }                 
                }
                else if(keyPress.Key == ConsoleKey.Delete)
                {
                    drinkName = "";
                }
                else if (keyPress.Key == ConsoleKey.Backspace && drinkName.Length > 0)
                {
                    drinkName = drinkName.Remove(drinkName.Length - 1);
                }
                else
                {
                    if(allowedChars.IsMatch(keyPress.KeyChar.ToString()) && drinkName.Length <= 16)
                    {
                        drinkName += keyPress.KeyChar;
                    }
                }
            }
        }
        static void DeleteRecipe()
        {
            Console.WriteLine("Delete Recipe");
            Console.ReadKey();
        }

        static void ChangeRecipe()
        {
            Console.WriteLine("Change Recipe");
            Console.ReadKey();
        }
    }
}
