//-----------------------------------------
// NAME: Luo Jiehao
// STUDENT NUMBER: 7852402
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: DR. Mehdi Niknam
// ASSIGNMENT: assignment 2, QUESTION: question 1
// REMARKS: To test, using a backtracking algorithm, if a mouse can escape from a rectangular maze.
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};

typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;

struct CELL_NODE
{
  Cell     cell;
  CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// To store the number of CellNode objects created in order to check memory leak.
int numberOfNodes=0;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for a path out of the maze
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);
// Print the maze.
void printMaze();
// Load the maze by reading a .txt file.
void loadMaze();
// returns true if there's a solution to the maze
Boolean solveMaze();
// our invariant checker
void checkState();
// To check if there is any memory leak.
Boolean checkMemoryLeak();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

//------------------------------------------------------
// main()
//
// PURPOSE: Load and solve a maze.
// INPUT PARAMETERS: int argc, char *argv[]
// OUTPUT PARAMETERS: int EXIT_SUCCESS
//------------------------------------------------------
int main( int argc, char *argv[] )
{
	// Load the maze by reading from a .txt file, then check and print it.
    loadMaze();
    checkState();  
	printMaze();
	
	// Try to solve the maze and print the outcome.
    if ( solveMaze() )
       	printf( "The mouse is free!!!!\n" );
    else
      	printf( "The mouse is trapped!!!!\n" );
      	
   	// Check memory leak and print related information.
    assert( checkMemoryLeak() );   
    if( checkMemoryLeak() )
    {
     	printf( "\nEnd of processing\n" );
	}
	else
	{
		printf( "\nThere is memory leak.\n" );
	}
    
  	return EXIT_SUCCESS;
}

//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////

//------------------------------------------------------
// equalCells
//
// PURPOSE: Returns true if the cells are at the same position in our maze.
// INPUT PARAMETERS:
// 		const Cell cell1: One the cell to be checked.
// 		const Cell cell2: The other cell to be checked.
// OUTPUT PARAMETERS: 
//		Boolean: To show if the cells are at the same position in our maze.
//------------------------------------------------------
Boolean equalCells(const Cell cell1, const Cell cell2)
{
	assert(validCell(cell1));
	assert(validCell(cell2));
	
	// Check if the 2 cells are valid, if they are, return true if the row and column of the 2 cells are the same, or false otherwise.
	if( validCell(cell1) && validCell(cell2) )
	{
		if( cell1.column == cell2.column && cell1.row == cell2.row ) 
		{
	 	 	return true;
		}
		else
		{
		 	return false;
		} 
	}
	else
	{
		printf("Invalid Cells!\n");
		return false;
	}
} 

//------------------------------------------------------
// makeCell
//
// PURPOSE: Returns a new cell object.
// INPUT PARAMETERS:
// 		const int row: the value of row used to create a new Cell.
// 		const int col: the value of column used to create a new Cell.
// OUTPUT PARAMETERS: 
//		Cell: The newly created Cell.
//------------------------------------------------------
Cell makeCell(const int row, const int col)
{
	assert( row > 0 && row < mazeRows-1 && col > 0 && col < mazeCols-1);
	
	// Create a new Cell and initialize it by -1.
	Cell newCell;
	newCell.column = -1;
	newCell.row = -1;
	
	// If the values of row and col are valid, then pass them into the newly created Cell. Otherewise, print error information.
	if( row > 0 && row < mazeRows-1 && col > 0 && col < mazeCols-1 )
	{
		newCell.column=col;
		newCell.row=row;
	}
	else
	{
		printf("Invalid row or column!\n");
	}
	
	assert( validCell(newCell) );

	return newCell;
} 
 
//------------------------------------------------------
// validCell
//
// PURPOSE: Returns true if the cell is within our maze.
// INPUT PARAMETERS:
// 		const Cell theCell: The Cell to be checked.
// OUTPUT PARAMETERS: 
//		Boolean: To represent if the cell is within our maze.
//------------------------------------------------------
Boolean validCell(const Cell theCell)
{	
	// Return true if the Cell is in the "inner" layers (inside the walls) of the maze, or false otherwise.
    if( theCell.column > 0 && theCell.row > 0 && theCell.column < mazeCols-1 && theCell.row < mazeRows-1 )	
    {
	 	return true;
    }
    else
    {
    	return false;
	}
}

//------------------------------------------------------
// addNeighbours
//
// PURPOSE: Add the neighbouring positions of the mouse into the list.
//------------------------------------------------------
void addNeighbours()
{
	checkState();
	assert( validCell(mouse) );
	
	// If the position of the mouse is valid, begin to add, otherwise print error informaion.
	if( validCell(mouse) )
	{
		// Check the forward, backward, left and right position of the mouse in that order.
		// If a position is passable, add it into the list.
		if( maze[mouse.row-1][mouse.column] == SPACE || maze[mouse.row-1][mouse.column] == EXIT)	
		{
 		 	addCell(makeCell(mouse.row-1, mouse.column));
		} 
		if( maze[mouse.row+1][mouse.column] == SPACE || maze[mouse.row+1][mouse.column] == EXIT)	
		{
 		 	addCell(makeCell(mouse.row+1, mouse.column));
		}
		if( maze[mouse.row][mouse.column-1] == SPACE || maze[mouse.row][mouse.column-1] == EXIT)	
		{	 	
		    addCell(makeCell(mouse.row, mouse.column-1));
		}
		if( maze[mouse.row][mouse.column+1] == SPACE || maze[mouse.row][mouse.column+1] == EXIT)	
		{	 
 			addCell(makeCell(mouse.row, mouse.column+1));
		}
	}
	else
	{
		printf("Invalid position of the mouse!\n");
	}
	
	checkState();
}
//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////

//------------------------------------------------------
// noMoreCells
//
// PURPOSE: Returns true if there are no more cells to try.
// OUTPUT PARAMETERS: 
//		Boolean: To represent if there are no more cells to try.
//------------------------------------------------------
Boolean noMoreCells()
{
	if( top == NULL )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------
// nextCell
//
// PURPOSE: Returns the next cell to try for a path out of the maze
// OUTPUT PARAMETERS: 
//		Cell: The next Cell in the list.
//------------------------------------------------------
Cell nextCell()
{
	assert( !noMoreCells() );
	
	// A varaible to store the Cell to be returned later, initialized by -1.
	Cell result;
	result.column = -1;
	result.row = -1;
	
	// If the list is not empty, take the next Cell, otherwise print error information.
	if( !noMoreCells() )
	{
		// Let "result" be the next Cell of top.
	 	result = top->cell;
		// Store the object pointed by top to free later.
		CellNode *toBeFreed = top;
		// Delete the first node in the list.
		top = top->next;
		// Free the deleted node and refresh numberOfNodes.
		free(toBeFreed);
		toBeFreed=NULL;
		numberOfNodes--;
	}
	else
	{
		printf("No more cells!\n");
	}
	return result;
}

// 
//------------------------------------------------------
// addCell
//
// PURPOSE: Introduce a new cell to try.
// INPUT PARAMETERS:
// 		const Cell cell: The Cell to be added.
//------------------------------------------------------
void addCell(const Cell cell)
{
	assert( validCell(cell) );
	// If the Cell to be added is valid, add the Cell into a newly created CellNode then add the CellNode into the list.
	// Otherwise, print error inforamtion.
	if( validCell(cell) )
	{	
		// Create a new newNode.
	    CellNode * newNode = (CellNode*) malloc(sizeof(CellNode));
		numberOfNodes++;
		assert( newNode );
		
		// If the newNode was created successfully, add the Cell into the CellNode then add the CellNode into the list.
	    if( newNode != NULL)
	    {
	    	newNode->cell = cell; 
			newNode->next = top;
			top = newNode;
		}
		// Otherwise, free the newNode and print error information.
	    else
	    {
	    	free(newNode);
	    	newNode=NULL;
	    	numberOfNodes--;
	    	printf("Run out of memory!\n");
		}
	}
	else
	{
		printf("Invalid Cell!\n");
	}
}

//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////

//------------------------------------------------------
// printMaze
//
// PURPOSE: Print the maze.
//------------------------------------------------------
void printMaze()
{
	checkState();
	int row;
	int col;
	for(row=0; row<mazeRows ;row++)
	{
		for(col=0; col<mazeCols ; col++)
		{
			printf("%c", maze[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

// 
//------------------------------------------------------
// loadMaze
//
// PURPOSE: Load the maze by reading a .txt file. 
//------------------------------------------------------
void loadMaze()
{
	// The variable to store one line of input.
	char input[MAX_DIMENSION];
	// Read one line from the .txt file to obtain the numbers of rows and columns of the maze. If read failed, print the erroe information.
	if( fgets(input, MAX_DIMENSION+1, stdin) != NULL )
	{
		assert( strlen(input) >= 3 );
		// Check if the line read is at least 3-character long. If not, print error inforamtion.
	 	if( strlen(input) >= 3 )
	 	{
	 		// Take the 2 numbers in the line read by token, and pass them into mazeRows and mazeCols.
	 		mazeRows = atoi( strtok(input," ") ); 
			mazeCols = atoi( strtok(NULL," ") ); 
			
			assert(mazeRows>0);
			assert(mazeCols>0);
			
			// Check if the numbers of rows and columns are valid. If not, print error information.
			if( mazeRows>0 && mazeCols>0 )
			{
				int row;
				// Read every line from the .txt line.
				for(row=0; row<mazeRows; row++)
				{
	   			    if( fgets(input, MAX_DIMENSION+1, stdin) != NULL )
	   			    {
	   			    	assert( strlen(input) >=  mazeCols*2-1 );
	   			    	// if the length of the line newly read is valid. If not, print error information.
	   			    	if( strlen(input) >=  mazeCols*2-1 )
	   			    	{
		 	        		int col;
		 	        		// Read every character of the line.
							for( col = 0; col < mazeCols; col++ )
							{
								assert( input[col*2] == WALL || input[col*2] == SPACE || input[col*2] == MOUSE || input[col*2] == EXIT || input[col*2] == VISITED );
								// Check if a character is valid. If not, print error information.
								if( input[col*2] == WALL || input[col*2] == SPACE || input[col*2] == MOUSE || input[col*2] == EXIT || input[col*2] == VISITED )
								{	
									// Pass the character into the corresponding cell of the maze.															
						  		    maze[row][col] = input[col*2];
						  		    
						  		    // Pass the character into mouse or escape if it is "m" or "e".
									if(maze[row][col] == MOUSE)
									{
								        mouse = makeCell(row, col);
									}
									else if(maze[row][col] == EXIT)
									{
					        	        escape = makeCell(row, col);
									} 	
								}
								else
								{
								    printf( "The #%d line of the input file is invalid.\n", row);
								}
							}
						}
						else
						{
							printf("The #%d line of the input file doesn't contain enough characters.\n", row);
						}
					}
					else
					{
					 	printf("The #%d line of the input file doesn't exist.\n", row);
					}
				}
			}
			else
			{
	 		    printf("The numbers of the rows and columns of the maze is invalid.\n");
			}
		}
		else
		{
		 	printf("The numbers of the rows and columns of the maze is in wrong format.\n");
		}
	}
	else
	{
		printf("Invalid input!\n");
	} 
}

// 
//------------------------------------------------------
// solveMaze
//
// PURPOSE: Returns true if there's a solution to the maze.
// Output parameters: 
//		Boolean: To represent if the mouse is free or trapped.		   		  
//------------------------------------------------------
Boolean solveMaze()
{
	checkState();
	assert( validCell(mouse) );
	
	// Check if the cell of mouse is within the maze. Print erroe information if not.
	if( validCell(mouse) )
	{
		// Make the position of the mouse be visited, then add its neighbouring positions.
		maze[mouse.row][mouse.column]='.';
		addNeighbours();
		assert( validCell(escape) );
		// Check if the cell of escape is within the maze. Print erroe information if not.
		if( validCell(escape) )
		{
			// Take the next Cell from the list and make the mouse move to it. 
			// Keep this process until there is no more Cells to try or the mouse has reached the exit.
			while( noMoreCells() != true && ( !equalCells(mouse, escape)) )
			{  	  
				printMaze();
		        mouse = nextCell();
	        	assert( validCell(mouse) );
	        	if( validCell(mouse) )
	        	{
	        		maze[mouse.row][mouse.column]='.';
					addNeighbours();
				}		   
				else
				{
 	 			 	printf("The cell of mouse is outside the maze!\n");
				}    	
			}
			// If there are Cells left in the list after the mouse has escaped, free them.
			while( top != NULL )
			{
				CellNode * current = top;
				free(current);
				current=NULL;
				top = top->next;
				numberOfNodes--;
			}
			// Return the boolean value to show if the mouse has escaped.
			if( equalCells(mouse, escape) )
			{
				return true;
			}
			else
			{
				printMaze();
				return false;
			}
		}
		else
		{
 		    printf("The cell of exit is outside the maze!\n");
 		    return false;
		}
	}
	else
	{
 	 	printf("The cell of mouse is outside the maze!\n");
 	 	return false;
	}
}

// 
//------------------------------------------------------
// checkState
//
// PURPOSE: Our invariant checker.	  
//------------------------------------------------------
void checkState()
{
	assert(mazeRows>0);
	assert(mazeCols>0);

	int row;
	int col;
	// To check if the first row and the last row of the maze are all "1"(Wall).
	for(col=0; col < mazeCols; col++)
	{
		assert(maze[0][col] == WALL);
		assert(maze[mazeRows-1][col] == WALL);
	} 
	// To check if the first col and the last col of the maze are all "1"(Wall).
	for(row=1; row < mazeRows-1; row++)
	{
		assert(maze[row][0] == WALL);
		assert(maze[row][mazeCols-1] == WALL);
	}
	// To check if all inner cells in the maze are valid.
	for(row=1; row < mazeRows-1; row++)
	{
		for(col=1; col < mazeCols-1; col++)
		{
			assert( maze[row][col]==SPACE || maze[row][col]==VISITED || maze[row][col]==MOUSE || maze[row][col]==EXIT || maze[row][col]==WALL);
		}
	}
	// To check if the cells of mouse and escape are within the maze.
	validCell(mouse);
	validCell(escape);
} 
 
// 
//------------------------------------------------------
// checkMemoryLeak
//
// PURPOSE: Our MemoryLeak checker.
// Output parameters:
//		Boolean: Return true if numberOfNodes equal to 0, return false otherwise.
//------------------------------------------------------
Boolean checkMemoryLeak()
{
	if( numberOfNodes == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
