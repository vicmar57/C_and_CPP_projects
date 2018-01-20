
/* Assignment: 1
Author1: Victor Martinov,
ID: 307835249
Author2: Tsur Avneri,
ID: 308134485
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define black X
#define white O
#define BoardSize 8

int islegal(char board[][BoardSize],int rows,int cols);//islegal checks if the move is legal according to the game rules return 1 if true,else return 0.
void move(char board[][BoardSize],int rows,int cols,char sign);//simulates a move in the game
void printboard(char board[][BoardSize]);
void menu ();
void decreaseThree(char * password);//recieves a sring and decreases the ascii value of each char by 3
void decrypt(char* password);//decodes the encrypted string
void swap(char* password);//swaps the adjacent chars in the string
void reverse(char* password);//reverse a given string
void startgame(char board[][BoardSize]);//Initializes the board
void ror(char* password);//rotate right by 2
int whostarts();// the function determine who will be the player to start the game by random
int printscore(char board[][BoardSize]);//prints the score and the winner
void playGM1(char board[][BoardSize]);//playing reversi agianst the computer
void playGM2(char board[][BoardSize]);//playing reversi against another human opponent
int counteatables(char board[][BoardSize] ,int row ,int col, char sign , int dir, int count,int toeat);//count the number of eatable pawns and eats them if toeat=1 in one direction
//row,col are the coordinates of the point the function is checking, sign is the sign of the player, dir is the direction that is being checked. count is the number of pawns avilable to eat,toeat is the order to eat the pawns available
void AIMove(char board[][BoardSize]);//simulates the computer move
void AIMove(char board[][BoardSize])
{
	int i,j,k,maxr=0,count=0,maxc=0,countmax=0;
	for(i=0;i<BoardSize;i++)
		for(j=0;j<BoardSize;j++)
		{
			if(islegal(board,i,j) && (board[i][j]=='_'))
			{
				for(k=1;k<=8;k++)
					count+=counteatables(board,i,j,'O',k,0,0);
				if(count>=countmax)
				{
					countmax=count;
					maxr=i;
					maxc=j;
				}
				count=0;
			}
		}
	for(k=1;k<=8;k++)
	{
		if (counteatables(board,maxr,maxc,'O',k,0,0)>0)//if count>0 meaning theres something to eat
			counteatables(board,maxr,maxc,'O',k,0,1);//start eating
	}
	board[maxr][maxc]='O';
	printf("computer played %d %d \n \n",maxr,maxc);
}
void playGM2(char board[][BoardSize])
{

	int rows=0, cols=0, counter=0,totalscoreO=0,totalscoreX=0,winner;
	char shouldwecontinueplaying;
	do{
		if (whostarts()==0)
		{
			printf("player O starts first .\n\n");
			while(counter<((BoardSize)*(BoardSize)-4))//the number of plays available in total according to the given board size
			{
				do
				{
					printf("player O : enter your move (rows,columns)\n");
					scanf("%d %d",&rows,&cols);
					if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
						printf("wrong coordinates, please try again\n");
				}while(!islegal(board,rows,cols) || (board[rows][cols]!='_'));
				move(board,rows,cols,'O');
				counter++;
				printboard(board);
				if (counter<(BoardSize)*(BoardSize)-4)
				{
					do
					{
						printf("player X : enter your move (rows,columns)\n");
						scanf("%d %d",&rows,&cols);
						if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
							printf("wrong coordinates, please try again\n");
					}while(!islegal(board,rows,cols)|| (board[rows][cols]!='_'));
					move(board,rows,cols,'X');
					printboard(board);
					counter++;
				}
			}
		}
		else
		{
			printf("player X starts first.\n\n");
			while(counter<(BoardSize)*(BoardSize)-4)
			{
				do
				{
					printf("player X : enter your move (rows,columns)\n");
					scanf("%d %d", &rows,&cols);
					if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
						printf("wrong coordinates, please try again\n");
				}while(!islegal(board,rows,cols) || (board[rows][cols]!='_'));
				move(board,rows,cols,'X');
				counter++;
				printboard(board);
				if (counter<(BoardSize)*(BoardSize)-4)
				{
					do
					{
						printf("player O : enter your move (rows,columns)\n");
						scanf("%d %d",&rows,&cols);
						if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
							printf("wrong coordinates, please try again\n");
					}while(!islegal(board,rows,cols)||(board[rows][cols]!='_'));
					move(board,rows,cols,'O');
					counter++;
					printboard(board);
				}
			}
		}
		winner=printscore(board);
		if(winner==1)
		{
			totalscoreO++;
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		}
		if(winner==2)
		{
			totalscoreX++;
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		}
		if(winner==0)
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		printf("would you like to continue? Y/N \n");
		do{
			while(getchar()!='\n');
			scanf("%c",&shouldwecontinueplaying);
			if(shouldwecontinueplaying!='Y' && shouldwecontinueplaying!='N')
				printf("wrong input, please try again \n");
		}while(shouldwecontinueplaying!='Y' && shouldwecontinueplaying!='N');
		counter=0;
		if(shouldwecontinueplaying=='Y')
		{
			startgame(board);
			printboard(board);
		}
		else
			if (totalscoreO > totalscoreX)
				printf("The winner is player O with the score of %d:%d \n", totalscoreO, totalscoreX);
		if (totalscoreX > totalscoreO)
			printf("The winner is player X with the score of %d:%d \n", totalscoreX, totalscoreO);
		 if (totalscoreX == totalscoreO)
			printf("There is no winner, the total score is %d:%d \n", totalscoreX, totalscoreO);
	}while(shouldwecontinueplaying=='Y');
}
int counteatables(char board[][BoardSize] ,int row ,int col, char sign , int dir, int count,int toeat)
{
	int nextx, nexty;
	switch(dir)//the "direction" the function is "going to"
	{
	case 1:nexty = col-1  , nextx=row; break;
	case 2:nexty = col+1  , nextx=row; break;
	case 3:nexty = col  , nextx=row-1; break;
	case 4:nexty = col  , nextx=row+1; break;
	case 5:nexty = col+1, nextx=row-1; break;
	case 6:nexty = col-1, nextx=row-1; break;
	case 7:nexty = col-1, nextx=row+1; break;
	case 8:nexty = col+1, nextx=row+1; break;
	}
	if (islegal(board, nextx,nexty) && board[nextx][nexty]!=sign && board[nextx][nexty]!='_')
	{
		if(toeat==1)
		{
			board[nextx][nexty]=sign;
			return counteatables(board ,nextx ,nexty, sign , dir, --count,1);
		}
		return counteatables(board ,nextx ,nexty, sign , dir, ++count,0);
	}
	else if(islegal(board, nextx,nexty) && board[nextx][nexty]==sign)
		return count;
	return 0;
}
int printscore(char board[][BoardSize])
{
	int pl1count = 0 , pl2count = 0 , i , j;
	char winner;
	for(i=0 ; i<BoardSize ; i++)
	{
		for(j=0 ; j<BoardSize ; j++)
		{
			if( board[i][j] == 'X')
			{
				pl1count ++;
			}
			else if( board[i][j] == 'O')
			{
				pl2count ++;
			}
		}
	}
	if (pl1count > pl2count)
		winner = 'X';
	else if (pl1count == pl2count)
		winner = 0;
	else
		winner = 'O';
	if (winner != 0)
		printf("Player %c won: %d %d \n" ,winner , pl2count , pl1count);
	else
		printf("It's a draw!\n");
	printf("\n");
	if(winner=='X')
		return 2;
	if(winner=='O')
		return 1;
	return 0;
}
void playGM1(char board[][BoardSize])
{
	int rows=0, cols=0, counter=0,totalscoreO=0,totalscoreX=0,winner;
	char shouldwecontinueplaying;
	do{
		if (whostarts()==0)
		{
			printf("Computer starts first.\n\n");
			while(counter<((BoardSize)*(BoardSize)-4))
			{
				AIMove(board);
				counter++;
				printboard(board);
				if (counter<(BoardSize)*(BoardSize)-4)
				{
					do
					{
						printf("player X : enter your move (rows,columns)\n");
						scanf("%d %d",&rows,&cols);
						if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
							printf("wrong coordinates, please try again\n");
					}while(!islegal(board,rows,cols)|| (board[rows][cols]!='_'));
					move(board,rows,cols,'X');
					printboard(board);
					counter++;
				}
			}
		}
		else
		{
			printf("player X start first.\n\n");
			while(counter<(BoardSize)*(BoardSize)-4)
			{
				do
				{
					printf("player X : enter your move (rows,columns)\n");
					scanf("%d %d", &rows,&cols);
					if(!islegal(board,rows,cols)|| (board[rows][cols]!='_'))
						printf("wrong coordinates, please try again\n");
				}while(!islegal(board,rows,cols) || (board[rows][cols]!='_'));
				move(board,rows,cols,'X');
				counter++;
				printboard(board);
				if (counter<(BoardSize)*(BoardSize)-4)
				{
					AIMove(board);
					counter++;
					printboard(board);
				}
			}
		}
		winner=printscore(board);
		if(winner==1)
		{
			totalscoreO++;
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		}
		if(winner==2)
		{
			totalscoreX++;
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		}
		if(winner==0)
			printf("Total score: the score is %d for Player O and %d for player X \n\n",totalscoreO,totalscoreX);
		printf("would you like to continue? Y/N \n");
		do{
			while(getchar()!='\n');
			scanf("%c",&shouldwecontinueplaying);
			if(shouldwecontinueplaying!='Y' && shouldwecontinueplaying!='N')
				printf("wrong input, please try again \n");
		}while(shouldwecontinueplaying!='Y' && shouldwecontinueplaying!='N');
		counter=0;
		if(shouldwecontinueplaying=='Y')
		{
			startgame(board);
			printboard(board);
		}
		else
			if (totalscoreO > totalscoreX)
				printf("The winner is player O with the score of %d:%d \n", totalscoreO, totalscoreX);
		if (totalscoreX > totalscoreO)
			printf("The winner is player X with the score of %d:%d \n", totalscoreX, totalscoreO);
		if (totalscoreX == totalscoreO)
			printf("There is no winner, the total score is %d:%d \n", totalscoreX, totalscoreO);
	}while(shouldwecontinueplaying=='Y');
	printscore(board);
}
int whostarts()
{
	srand(time(NULL));
	return rand()%2;
}
int islegal(char board[][BoardSize],int rows,int cols)
{
	if(cols<BoardSize && cols >=0 && rows>=0 && rows<BoardSize)
	{
		int i,j;
		for(i=-1;i<=1;i++)
			for(j=-1;j<=1;j++)
			{
				if((cols+j)>=0 && (cols+j)<BoardSize && (rows+i)>=0 && (rows+i) <BoardSize)
					if((i!=0 || j!=0) && (board[rows+i][cols+j]=='X' || board[rows+i][cols+j]=='O'))
						return 1;
			}
	}
	return 0;
}
void move(char board[][BoardSize],int rows,int cols,char sign)
{
	int i;
	board[rows][cols]=sign;
	for(i=1;i<=8;i++)
		if(counteatables(board,rows,cols,sign,i,0,0)>0)
			counteatables(board,rows,cols,sign,i,0,1);

}
void printboard(char board[][BoardSize])
{
	int i,j,k;
	for(k=0;k<BoardSize;k++)
	{
		printf("%5d", k);
	}
	printf ("\n");
	for(i=0;i<BoardSize;i++)
	{
		if (i < BoardSize)
			printf("%d" ,i);
		for(j=0;j<BoardSize;j++)
			printf("%4c ",board[i][j]);
		printf("\n");
	}
	printf("\n");
}
void startgame(char board[][BoardSize])
{
	int i,j;
	for(i=0;i<BoardSize;i++)
		for(j=0;j<BoardSize;j++)
			board[i][j]='_';
	board[BoardSize/2-1][BoardSize/2]='X';
	board[BoardSize/2-1][BoardSize/2-1]='O';
	board[BoardSize/2][BoardSize/2-1]='X';
	board[BoardSize/2][BoardSize/2]='O';
}
void reverse(char* password)
{
	int i;
	char temp;
	for(i=0;i<4;i++)
	{
		temp=password[i];
		password[i]=password[7-i];
		password[7-i]=temp;
	}
}
void ror(char* password)
{
	int i;
	char temp1,temp2,temp3,temp4;
	for(i=0;i<BoardSize;i++)
	{
		if(i==0)
		{
			temp1=password[(i+2)%BoardSize];
			password[(i+2)%BoardSize]=password[i];
		}
		else if(i==1)
		{
			temp2=password[(i+2)%BoardSize];
			password[(i+2)%BoardSize]=password[i];
		}
		else if (i%2==0)
		{
			temp3=password[(i+2)%BoardSize];
			password[(i+2)%BoardSize]=temp1;
			temp1=temp3;
		}
		else
		{
			temp4=password[(i+2)%BoardSize];
			password[(i+2)%BoardSize]=temp2;
			temp2=temp4;
		}
	}
}
void swap(char* password)
{
	int i;
	char temp;
	for(i=1;i<BoardSize;i+=2)
	{
		temp=password[i-1];
		password[i-1]=password[i];
		password[i]=temp;
	}
}
void decrypt(char* password)
{
	int i;
	for(i=0;i<BoardSize;i++)
	{
		switch(password[i])
		{
		case 'S':password[i]='A'; break;
		case '5':password[i]='B'; break;
		case 'P':password[i]='C'; break;
		case 'W':password[i]='D'; break;
		case 'A':password[i]='E'; break;
		case 'R':password[i]='F'; break;
		case '8':password[i]='G'; break;
		case 'Q':password[i]='H'; break;
		case 'I':password[i]='I'; break;
		case 'V':password[i]='J'; break;
		case 'B':password[i]='K'; break;
		case '0':password[i]='L'; break;
		case 'D':password[i]='M'; break;
		case 'Z':password[i]='N'; break;
		case '3':password[i]='O'; break;
		case 'C':password[i]='P'; break;
		case '2':password[i]='Q'; break;
		case 'H':password[i]='R'; break;
		case '4':password[i]='S'; break;
		case '1':password[i]='T'; break;
		case 'K':password[i]='U'; break;
		case '7':password[i]='V'; break;
		case 'G':password[i]='W'; break;
		case 'Y':password[i]='X'; break;
		case '6':password[i]='Y'; break;
		case 'O':password[i]='Z'; break;
		case 'J':password[i]='0'; break;
		case 'E':password[i]='1'; break;
		case 'U':password[i]='2'; break;
		case 'N':password[i]='3'; break;
		case 'F':password[i]='4'; break;
		case '9':password[i]='5'; break;
		case 'M':password[i]='6'; break;
		case 'L':password[i]='7'; break;
		case 'T':password[i]='8'; break;
		case 'X':password[i]='9'; break;
		}
	}
}
void menu ()

{
	printf("1.Encryption\n");
	printf("2.Reversi\n");
	printf("3.Exit\n");
}
void decreaseThree(char * password)
{
	int i;
	for (i=0;i<BoardSize;i++)
	{
		if(password[i]>='A' && password[i]<='Z')
		{
			password[i]=(password[i]-3);
			if(password[i]<'A')
				password[i]='['-('A'-password[i]);
		}
		if(password[i]>='0' && password[i]<='9')
		{
			password[i]=(password[i]-3);
			if(password[i]<'0')
				password[i]=':'-('0'-password[i]);
		}
	}
}
int main(void)
{
	int option=0,i,letters=0,nums=0,length=0,gmode=0;
	char password[9],board[BoardSize][BoardSize];
	while(option!=3)
	{
		menu();
		scanf("%d",&option);
		switch (option)
		{
		case 1:
		{
			do{
				printf("Please enter your password\n");
				scanf("%s",password);
				letters=0;
				nums=0;
				length=0;
				for(i=0;i<8;i++)
				{
					if(password[i]==0)
						break;
					length++;
					if (password[i]>='A' && password[i]<='Z')
						letters++;
					if(password[i]>='0' && password[i]<='9')
						nums++;
				}
				if(nums<2 || letters<2 || length!=8)
					printf("wrong password please enter the encrypted password again.");
			}
			while(nums<2 || letters<2 || length!=8 );
			decreaseThree(password);
			decrypt(password);
			swap(password);
			reverse(password);
			ror(password);
			printf("the word is:");
			puts(password);
			break;
		}
		case 2:
		{
			printf("1.computer VS\n");
			printf("2.2 players\n");
			scanf("%d",&gmode);
			printf("\n");
			startgame(board);
			printboard(board);
			if(gmode==1)
				playGM1(board);
			if(gmode==2)
				playGM2(board);
			break;
		}
		case 3:
			break;
		}
	}
	return 0;
}

