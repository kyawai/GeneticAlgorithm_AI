#include <iostream>
#include <fstream>
#include <Windows.h>
#include <time.h>
#include <array>
#include <ctime>

int mazeFunc(float secondNum, float firstNum, int yCurrent_pos, int xCurrent_pos, int character, int mazeNumber, int *wallX, int *wallY);

int main()
{
	//initialisation of veriables
	char fileSize = 0;
	char currNumChar; 
	int firstNum = 0, firstNum2 = 0, secondNum = 0, secondNum2 = 0;
	float fitness[61];
	float allFitness = 0;
	float individual[61]; 
	float individualPrev[61]; 
	float individualChosen[61];
	float chromosoneNum[61];
	int chrom[61][401];
	int wallX[400]; int wallY[400];
	int yFinish, yStart, xStart, xFinish, yCurrent_pos, xCurrent_pos;
	int pos;
	int wallAmount = 0; 
	int character = 4;
	int foundChrom[401];
	std::array<float, 60> highestFitness;
	int chromLength = 10, halfChromLength = 5;
	int testChromN = 0;
	int count = 0;
	int genCount = 0;
	int mazeNumber = 0;
	int countChrom = 0;
	//bools
	bool wallColl = false;
	bool found = false;

	//srand used for creating random numbers used later in the program based on time
	srand(time(NULL));

	// load text file
	std::fstream input("Maze6.txt");

	//if the file is open:
	if (input.is_open())
	{
		//start the clock
			clock_t start = clock();
		//checks the firt sets of numbers to find the size of the maze.
		//row
		for (int num1 = 0; num1 < 1; num1++)
		{
			input >> fileSize;
			if (fileSize != '\0' && fileSize != (-1) && fileSize != (-2) && fileSize != 32 && fileSize != (-52))
			{
				// check if there is a second number (double digit size of maze)
				firstNum = fileSize;
				for (int num1two = 1; num1two < 3; num1two++)
				{
					input >> fileSize;
					if (fileSize != '\0' && fileSize != (-1) && fileSize != (-2) && fileSize != 32 && fileSize != (-52))
					{
						firstNum2 = fileSize;
						if (firstNum2 == '0') { firstNum = firstNum - 48; firstNum = firstNum * 10; }
						else if (firstNum2 == '1') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 1; }
						else if (firstNum2 == '2') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 2; }
						else if (firstNum2 == '3') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 3; }
						else if (firstNum2 == '4') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 4; }
						else if (firstNum2 == '5') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 5; }
						else if (firstNum2 == '6') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 6; }
						else if (firstNum2 == '7') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 7; }
						else if (firstNum2 == '8') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 8; }
						else if (firstNum2 == '9') { firstNum = firstNum - 48; firstNum = firstNum * 10 + 9; }
					}
					else if (num1two == 2)
					{
						firstNum = firstNum - 48; //take away 48 from char to get the size of the maze
					}
				}
			}
			else
			{
				num1 = num1 - 1;
			}
		}
		//above repeated for column
		for (int num1 = 0; num1 < 1; num1++)
		{
			input >> fileSize;
			if (fileSize != '\0' && fileSize != (-1) && fileSize != (-2) && fileSize != 32 && fileSize != (-52))
			{
				secondNum = fileSize;
				for (int num1two = 1; num1two < 3; num1two++)
				{
					input >> fileSize;
					if (fileSize != '\0' && fileSize != (-1) && fileSize != (-2) && fileSize != 32 && fileSize != (-52))
					{
						secondNum2 = fileSize;
						if (secondNum2 == '0') { secondNum = secondNum - 48; secondNum = secondNum * 10; }
						else if (secondNum2 == '1') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 1; }
						else if (secondNum2 == '2') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 2; }
						else if (secondNum2 == '3') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 3; }
						else if (secondNum2 == '4') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 4; }
						else if (secondNum2 == '5') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 5; }
						else if (secondNum2 == '6') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 6; }
						else if (secondNum2 == '7') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 7; }
						else if (secondNum2 == '8') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 8; }
						else if (secondNum2 == '9') { secondNum = secondNum - 48; secondNum = secondNum * 10 + 9; }
					}
					else if (num1two == 2)
					{
						secondNum = secondNum - 48;
					}
				}
			}
			else
			{
				num1 = num1 - 1;
			}
		}

		//find start and end pos
		for (int i = 0; i < secondNum; i++)
		{
			for (int j = 0; j < firstNum; j++)
			{
				input >> currNumChar;
				if (currNumChar == '2')
				{
					yStart = i;
					xStart = j;
				}

				if (currNumChar == '3')
				{
					xFinish = j;
					yFinish = i;
				}
				if (currNumChar == (-1) || currNumChar == (-2) || currNumChar == (32) || currNumChar == (-52) || currNumChar == ('\0'))
				{
					j = j - 1;
				}
				if (currNumChar == '1')    //if there is a 1, add it to the wall amount and save its position
				{
						wallAmount++;
						wallX[count] = j;
						wallY[count] = i;
						count++;
				}
			}
		}
		// create the population of chromosones 
		for (int i = 0; i < 60; i++)
		{
				for (int j = 0; j < 400; j++)
				{
						float r = rand() % 2;
						if (r <= 0.5) { chrom[i][j] = 0; }
						else { chrom[i][j] = 1; }
				}
		}
		
		// the GA program
		while (found == false)
		{
				//sets all fitness to 0, gets reset at every new generation
				allFitness = 0.0f;
				//for loops for movement
				for (int chromN = 0; chromN < 60; chromN++)
				{
						xCurrent_pos = xStart; yCurrent_pos = yStart; pos = 0;  //every start of chromosone, current positons are set to start 
						countChrom = 0;
						for (pos; pos < chromLength; pos++)
						{
								if (chrom[chromN][pos] == 0 && chrom[chromN][pos + 1] == 0) //move up
								{
										wallColl = false;
										for (count = 0; count < wallAmount; count++) //check if there is a wall
										{
												if (yCurrent_pos - 1 == wallY[count] && xCurrent_pos == wallX[count])
												{
														xCurrent_pos = xCurrent_pos;
														yCurrent_pos = yCurrent_pos;
														wallColl = true;
														break;
												}
										}
										if ((yCurrent_pos - 2) < 0 || xCurrent_pos < 0) //check if its out of boundaries
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;
										}
										else if (wallColl)   //if nool wallColl was turned on (if the next coordinates = a wall) dont move
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;

										}
										else               //if the other conditions are not true, move up
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos - 1;
										}
								}
								if (chrom[chromN][pos] == 0 && chrom[chromN][pos + 1] == 1)  //move right
								{
										wallColl = false;
										for (count = 0; count < wallAmount; count++)
										{
												if (yCurrent_pos == wallY[count] && xCurrent_pos + 1 == wallX[count])
												{
														xCurrent_pos = xCurrent_pos;
														yCurrent_pos = yCurrent_pos;
														wallColl = true;
														break;
												}
										}
										if (xCurrent_pos + 2 > firstNum)
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;
										}
										else if (wallColl)
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;

										}
										else        //if the other conditions are not true, move right
										{
												xCurrent_pos = xCurrent_pos + 1;
												yCurrent_pos = yCurrent_pos;
										}
								}
								if (chrom[chromN][pos] == 1 && chrom[chromN][pos + 1] == 0) // move down
								{
										wallColl = false;
										for (count = 0; count < wallAmount; count++)
										{
												if (yCurrent_pos + 1 == wallY[count] && xCurrent_pos == wallX[count])
												{
														xCurrent_pos = xCurrent_pos;
														yCurrent_pos = yCurrent_pos;
														wallColl = true;
														break;
												}
										}
										if (yCurrent_pos + 2 > secondNum)
										{

												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;
										}
										else if (wallColl)
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;

										}
										else     //if the other conditions are not true, move down
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos + 1;
										}

								}
								if (chrom[chromN][pos] == 1 && chrom[chromN][pos + 1] == 1) // move left
								{
										wallColl = false;
										for (count = 0; count < wallAmount; count++)
										{
												if (yCurrent_pos == wallY[count] && xCurrent_pos - 1 == wallX[count])
												{
														xCurrent_pos = xCurrent_pos;
														yCurrent_pos = yCurrent_pos;
														wallColl = true;
														break;
												}
										}
										if (yCurrent_pos < 0 || xCurrent_pos - 1 < 0)
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;
										}
										else if (wallColl)
										{
												xCurrent_pos = xCurrent_pos;
												yCurrent_pos = yCurrent_pos;

										}
										else
										{
												xCurrent_pos = xCurrent_pos - 1;   //if the other conditions are not true, move left
												yCurrent_pos = yCurrent_pos;
										}
								}
								if (yCurrent_pos == yFinish && xCurrent_pos == xFinish) //check if the move is equal to the finishing coordinates
								{
										found = true;
										for (int i = 0; i < chromLength; i++)//if the move is in finishing cooridnates, copy the chromosome used, save chromosome number and break out of loop
										{
												foundChrom[i] = chrom[chromN][i];
										}
										testChromN = chromN;
										break;
								}
								pos++;
						}
						if (found == true) { break; } //if the bool found was true, the maze is finished, break up of the loop

						//fitness calculation

						//set veriables for the fitness
						float highestFitness[61];
						float prevIndividual[61] = { 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
						bool broken = false;
						bool numSet = false;

						//calculate the fitness
						float xDiff = xFinish - xCurrent_pos;
						if (xDiff < 0.0f)
						{
								xDiff = abs(xDiff);  //if the number is negative, make it positive+
						}
						float yDiff = yFinish - yCurrent_pos;
						if (yDiff < 0.0f)
						{
								yDiff = abs(yDiff);  //if the number is negative, make it positive+
						}
						float fitnessTemp = xDiff + yDiff + 1;
						fitness[chromN] = 1.0f / fitnessTemp; //fitness
						
							//while its the last chromosone arrange fitness from highest to lowest
						if (chromN >= 59)
						{
								for (int i = 0; i < chromN+1; i++)
								{
										highestFitness[i] = fitness[i];

								}
								int n = sizeof(highestFitness) / sizeof(highestFitness[1]);
								std::sort(highestFitness, highestFitness + n, std::greater<float>());  //highest to lowest fitness

								//set base chromosone numbers (chromosone numbers used for picking what chromosome will be used for the crossover
								for (int i = 0; i < 60; i++) 
								{ chromosoneNum[i] = i;}

								//calculate the sum of all of the gitness
								for (int i = 0; i < 60; i++) { allFitness = allFitness + highestFitness[i]; }

								//chromosone is 39, add 1 to make it 40 for the next for loop
								int tempChromNum = chromN + 1;

								//set the chromosone Number to either the high fitness or the rest in order (rest are
								//throw aways), only used if they get a high fitness
								for (int j = 0; j < tempChromNum; j++)
								{
										numSet = false;          //reset the bool. Bool represents whether the new chromosome number has been set 
												for (int i = 0; i <= chromN; i++)  //check through all the chromosomes
												{
														broken = false;   
														if (fitness[i] == highestFitness[j])   //if the current fitness 
														{
																chromosoneNum[j] = i;          //set the chromosome number used to the chosen number

																for (int k = 0; k < chromN; k++) //check if any of the previous chosen chromosome numbers have been used, if so, ignore them
																{
																		if (prevIndividual[k] == chromosoneNum[j])
																		{
																				broken = true;
																				break;
																		}
																}
																if (broken == false)
																{
																		prevIndividual[j] = chromosoneNum[j];  //set previous chromosome to current chromosome
																		numSet = true;

																}
																if (numSet == true) { break; } //if the chromosome has been set, break out of the look and restart on the next chromosome
														}
												}

												//do roulette wheel for the first 20 numbers for crossover
												float rChrom = rand() % 100;  //pick only the first half of the highest chromosones
												individualPrev[0] = 0;
												for (int i = 0; i < chromN; i++) //go through all of the chromosomes
												{
														individual[i] = (highestFitness[j] / allFitness) * 100;
														individual[i] = individual[i] + individualPrev[i];
														//pick chromosones to do crossover on
														if (rChrom >= individualPrev[i - 1] && rChrom < individual[i]) { individualChosen[j] = i; chromosoneNum[j] = i;  break; } //roulette wheel, go through all chromosone % and 
														individualPrev[i + 1] = individual[i];                                                                                    //see which onnes equal to the random number, then pick that chromosome
												}
										
								}

								//crossover and mutation
								int newArr[401];int newArr2[401];float tempChrom[61][401];
								for (int i = 0; i < 60; i++)
								{
										int temp[401], temp2[401], temp3[401], temp4[401];
										int tempNum = chromosoneNum[i];  //set the chosen chromosomes in order and set them as temp numbers
										int tempNum2 = chromosoneNum[i + 1]; //done in pair
										float crossRand = rand() % 100;
										if (crossRand <= 70) //get a random number up to 100 and if its less or equal to 70, do crossover
										{

												for (int j = 0; j < chromLength; j++) //crossover. go through every chromosome 
												{
														if (j <= halfChromLength) //first half of the chromosomes are moved to temp then from temp to new array
														{

																newArr[j] = chrom[tempNum][j];
																newArr2[j] = chrom[tempNum2][j];

														}
														if (j > halfChromLength)  //second half of the chromosomes are moved to temp then from temp to new array
														{

																newArr[j] = chrom[tempNum2][j];
																newArr2[j] = chrom[tempNum][j];

														}
														float mutationRand = rand() % 10000; //mutation. get a number between 1 and 10,000
														
														if (mutationRand < 10) //if the mutation number is less that 10, mutate the number (change 1 to 0, 0 to 1)
														{
																if (newArr[j] == 1)
																{
																		newArr[j] = 0;
																}
																else
																{
																		newArr[j] = 1;
																}
														}
														if (mutationRand <= 10) // do the same for the second chromosome (as the mutation and crossover I have done in pairs)
														{
																if (newArr2[j] == 1)
																{
																		newArr2[j] = 0;
																}
																else
																{
																		newArr2[j] = 1;
																}
														}
														tempChrom[i][j] = newArr[j];      //put the 2 new chromosomes into temporary chromosone
														tempChrom[i + 1][j] = newArr2[j];

												}
										}
										else   //do the same mutation as above for the chromosomes that were not picked to be crossed over
										{
												for (int j = 0; j < chromLength; j++)
												{

														float mutationRand = rand() % 10000;
														if (mutationRand <= 10)
														{
																if (chrom[tempNum][j] == 1)
																{
																		chrom[tempNum][j] = 0;
																}
																else
																{
																		chrom[tempNum][j] = 1;
																}
														}
														if (mutationRand <= 10) 
														{
																if (chrom[tempNum+1][j] == 1)
																{
															  	chrom[tempNum][j] = 0;
																}
																else
																{
																		chrom[tempNum][j] = 1;
																}
														}
														tempChrom[i][j] = chrom[tempNum][j];  //set the new chromosones into the temp chromosone
														tempChrom[i + 1][j] = chrom[tempNum2][j];
												}
										}
										for (int j = 0; j < chromLength; j++)    //after the crossover and the mutations have been done, put the new chromosomes back into the original ones
										{
												chrom[i][j] = tempChrom[i][j];
												chrom[i + 1][j] = tempChrom[i + 1][j];
										}
										i++;
								}
								
						}
				}
				if (found == true) //if the bool found is true, the finish has been found
				{
					clock_t end = clock();                                                   //print out the relevant information and stop the clock
					double time_elapsed = double(end - start);

					//code below prints the path of the final chromosome
					//this takes the chromosone that found the solution, goes through the step process as seen above, and prints the movement of the chromosone
					xCurrent_pos = xStart;
					yCurrent_pos = yStart;  //resets the position for shwoing the movement of the solution

					pos = 0;
					countChrom = 0;
					xCurrent_pos = xStart; yCurrent_pos = yStart; pos = 0;  //every start of chromosone, current positons are set to start 

					for (pos; pos < chromLength; pos++)
					{
							if (chrom[testChromN][pos] == 0 && chrom[testChromN][pos + 1] == 0) //move up
							{
									wallColl = false;
									for (count = 0; count < wallAmount; count++) //check if there is a wall
									{
											if (yCurrent_pos - 1 == wallY[count] && xCurrent_pos == wallX[count])
											{
													xCurrent_pos = xCurrent_pos;
													yCurrent_pos = yCurrent_pos;
													wallColl = true;
													break;
											}
									}
									if ((yCurrent_pos - 2) < 0 || xCurrent_pos < 0) //check if its out of boundaries
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;
									}
									else if (wallColl)   //if nool wallColl was turned on (if the next coordinates = a wall) dont move
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;

									}
									else               //if the other conditions are not true, move up
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos - 1;
									}
							}
							if (chrom[testChromN][pos] == 0 && chrom[testChromN][pos + 1] == 1)  //move right
							{
									wallColl = false;
									for (count = 0; count < wallAmount; count++)
									{
											if (yCurrent_pos == wallY[count] && xCurrent_pos + 1 == wallX[count])
											{
													xCurrent_pos = xCurrent_pos;
													yCurrent_pos = yCurrent_pos;
													wallColl = true;
													break;
											}
									}
									if (xCurrent_pos + 2 > firstNum)
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;
									}
									else if (wallColl)
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;

									}
									else        //if the other conditions are not true, move right
									{
											xCurrent_pos = xCurrent_pos + 1;
											yCurrent_pos = yCurrent_pos;
									}
							}
							if (chrom[testChromN][pos] == 1 && chrom[testChromN][pos + 1] == 0) // move down
							{
									wallColl = false;
									for (count = 0; count < wallAmount; count++)
									{
											if (yCurrent_pos + 1 == wallY[count] && xCurrent_pos == wallX[count])
											{
													xCurrent_pos = xCurrent_pos;
													yCurrent_pos = yCurrent_pos;
													wallColl = true;
													break;
											}
									}
									if (yCurrent_pos + 2 > secondNum)
									{

											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;
									}
									else if (wallColl)
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;

									}
									else     //if the other conditions are not true, move down
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos + 1;
									}

							}
							if (chrom[testChromN][pos] == 1 && chrom[testChromN][pos + 1] == 1) // move left
							{
									wallColl = false;
									for (count = 0; count < wallAmount; count++)
									{
											if (yCurrent_pos == wallY[count] && xCurrent_pos - 1 == wallX[count])
											{
													xCurrent_pos = xCurrent_pos;
													yCurrent_pos = yCurrent_pos;
													wallColl = true;
													break;
											}
									}
									if (yCurrent_pos < 0 || xCurrent_pos - 1 < 0)
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;
									}
									else if (wallColl)
									{
											xCurrent_pos = xCurrent_pos;
											yCurrent_pos = yCurrent_pos;

									}
									else
									{
											xCurrent_pos = xCurrent_pos - 1;   //if the other conditions are not true, move left
											yCurrent_pos = yCurrent_pos;
									}

							}
							mazeFunc(secondNum, firstNum, yCurrent_pos, xCurrent_pos, character, mazeNumber, &wallX[0], &wallY[0]); //print the maze movement to visually show the movement of the final chromosone and how it got to the finish
							if (xCurrent_pos == xFinish && yCurrent_pos == yFinish) //break out of loop when maze is finished
							{
									break;
							}
							pos++;
							countChrom = countChrom + 2;
					}
					std::cout << "*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*" << std::endl;      //print out end results
					std::cout << "*_*_*_*_*_*_*SOLUTION FOUND*_*_*_*_*_*_*" << std::endl;
					std::cout << "time took to find the solution (ms) :  " << time_elapsed << std::endl;
					std::cout << "chromosone that found it : " << testChromN << " in generation: " << genCount << " chromosone binary: " << std::endl;
					for (int i = 0; i < countChrom; i++)
					{
							std::cout << foundChrom[i];
					}
					std::cout << std::endl;
					std::cout << "*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*" << std::endl;
				}
				genCount++;          //everytime one generation is finished, add one to the generation counter
				if (chromLength <  400)    //because my chromosone length increases depending on how quickly the finish is found. I have put a cap on the length
				{
						chromLength = chromLength + 2;
						halfChromLength++;
				}
		}
	}	
	//close the file and pause the program
	input.close();
	system("Pause");
	return 0;
}
int mazeFunc(float secondNum, float firstNum, int yCurrent_pos, int xCurrent_pos, int character, int mazeNumber, int *wallX, int *wallY)
{
		char mazeSize = 0; float tempFirstNum = 0; float tempSecNum = 0;

		// load text file0
		std::fstream inputMaze("Maze1.txt");
		//if the file is open:
		if (inputMaze.is_open())
		{
				//check for first digits for maze size and not use them 
				for (int num1 = 0; num1 < 1; num1++)
				{
						inputMaze >> mazeSize;
						if (mazeSize != '\0' && mazeSize != (-1) && mazeSize != (-2) && mazeSize != 32 && mazeSize != (-52))
						{
								// check if there is a second number (double digit size of maze)

								for (int num1two = 1; num1two < 3; num1two++)
								{
										inputMaze >> mazeSize;
										if (mazeSize != '\0' && mazeSize != (-1) && mazeSize != (-2) && mazeSize != 32 && mazeSize != (-52))
										{
												int doubleDigitNum = mazeSize;
										}
										else if (num1two == 2)

										{
												tempFirstNum = tempFirstNum - 48; //take away 48 from char to get the size of the maze
										}
								}
						}
						else
						{
								num1 = num1 - 1;
						}
				}
				//second digit/digits
				for (int num1 = 0; num1 < 1; num1++)
				{
						inputMaze >> mazeSize;
						if (mazeSize != '\0' && mazeSize != (-1) && mazeSize != (-2) && mazeSize != 32 && mazeSize != (-52))
						{
								// check if there is a second number (double digit size of maze)

								for (int num1two = 1; num1two < 3; num1two++)
								{
										inputMaze >> mazeSize;
										if (mazeSize != '\0' && mazeSize != (-1) && mazeSize != (-2) && mazeSize != 32 && mazeSize != (-52))
										{
												int doubleDigitNum = mazeSize;
										}
										else if (num1two == 2)
										{
												tempSecNum = tempSecNum - 48; //take away 48 from char to get the size of the maze
										}
								}
						}
						else
						{
								num1 = num1 - 1;
						}
				}
				int tCount = 0;
				//draw maze with character
				for (int i = 0; i < secondNum; i++)
				{

						for (int j = 0; j < firstNum; j++)
						{
								inputMaze >> mazeSize;
								if (mazeSize == '0' || mazeSize == '1' || mazeSize == '2' || mazeSize == '3')
								{
										mazeNumber = mazeSize;
										if (j == xCurrent_pos && i == yCurrent_pos && mazeSize != '1')
										{
												std::cout << character;
										}
										else
										{
												if (mazeSize == '0')   //replace 1 and 0 of the maze to different characters
												{
														std::cout << "-";
												}
												else
												{
														if (mazeSize == '1')
														{
																std::cout << "|";
																*(wallX + tCount) = j;// add to wall count (for showing the final movement)
																*(wallY + tCount) = i;// as above
																tCount++;
														}
														else
														{
																mazeNumber = mazeNumber - 48;
																std::cout << mazeNumber;
														}

												}

										}
								}
								else
								{
										j = j - 1;
								}

						}
						std::cout << "\n";
				}
		}
		std::cout << " " << std::endl;
		inputMaze.close();
		return 0;
}