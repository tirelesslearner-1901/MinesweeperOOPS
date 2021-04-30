#include "Menu.h"
#include "SoundManager.h"

#define isValid(i,j) (i>=0 && i<ROW &&j>=0 && j<COL)      //checks whether the position(i,j) lies in grid or not


using namespace sf;
using namespace std;


int SCR_H , SCR_W ;                    //height and width of screen
int ROW;                               //number of rows in grid
int COL;                               //number of columns in grid
int BOMBCOUNT;                         //number of bombs in grid
int COVERCOUNT = ROW*COL;              //number of covered cells in grid
bool GAMEOVER = false;                 //bool variables to store status of game
bool WIN = false;                      // "     "
int w=32;                              //size of each cell
int cnt1 = 0;                          //count the number of flags placed
RenderWindow app(VideoMode(600, 600), "MENU");                               //creates a window with title MENU
Menu menu2(app.getSize().x, app.getSize().y,"Easy","Medium","Difficult");    //object of menu class
Menu menu1(app.getSize().x, app.getSize().y,"Play","Instructions","Exit");   //object of menu class
SoundManager over("sound/mixkit-player-losing-or-failing-2042.wav");         //object of SoundManager class
SoundManager win("sound/preview.wav");                                       //object of SoundManager class
SoundManager lc("sound/mixkit-winning-a-coin-video-game-2069.wav");
SoundManager rc("sound/mixkit-bag-of-coins-touch-3187.wav");


bool isNotInSquare(int x, int y, int i, int j)              //checks whether (x,y) is in 8-neighborhood of (i,j) or not
{
    bool isNot = true;
    int p,q;
    for(p=i-1;p-i+1<3;p++)
    {
        for(q=j-1;q-j+1<3;q++)
        {
            if(isValid(p,q))
            {
                if(p==x && q==y)
                {
                    isNot = false;
                }
            }
        }
    }
    return isNot;
}



//Structure for BOMB
struct BOMB
{
    int x;
    int y;
};

//Game goes here
class GameBoard
{
    vector< vector<int> > mines;//Matrix to store EPMTY SPACE = 0, BOMBs = -1, and NUMBERS>0 = no. of BOMBS around it
    vector< vector<int> > isCovered;//Matrix to store 0 = NOR COVERED, 1 = COVERED
    vector<BOMB> BOMBVector; //Stores exact location of BOMBs in an array of BOMB.
    void setUpBoards();//resize and fill in the initial values of mines[] and isCovered
    void placeBOMBs(int a, int b);//place BOMBs randomly on mines[] board, except in the neighborhood centred at the point of first click (a,b)
    void placeNumbers();//place numbers to show where the BOMBs are, on mines[].
    void unCover(int i, int j);//Uncovers tiles when clicked by user; for the first click, call placeBOMBs and placeNumbers
public:
    GameBoard();         //constructor calling setupBoards()
    void printBoard();   //print the updated board 
    
};


void GameBoard::setUpBoards()
{
    int i,j;
    mines.resize(ROW);
    isCovered.resize(ROW);
    //Initializee mines[]
    for(i=0;i<ROW;i++)
    {
        mines[i].resize(COL);
        for(j=0;j<COL;j++)
        {
            mines[i][j] = 0;
        }
    }
    //Initialize isVisited[]
    for(i=0;i<ROW;i++)
    {
        isCovered[i].resize(COL);
        for(j=0;j<COL;j++)
        {
            isCovered[i][j] = 1;
        }
    }
}

//Place BOMBs randomly on board
void GameBoard::placeBOMBs(int a,int b)
{
    srand(time(NULL));
    BOMBVector.resize(BOMBCOUNT);
    set< pair<int,int> > BOMBSet;
    int i,p,q;
    for(i=0;i<BOMBCOUNT;i++)
    {
        do
        {
            p = rand()%ROW;
            q = rand()%COL;
        }while(!isNotInSquare(p,q,a,b) || !(BOMBSet.find( pair<int,int> (p,q))==BOMBSet.end()));

        BOMBVector[i].x = p;
        BOMBVector[i].y = q;
        BOMBSet.insert( pair<int,int> (p,q));
    }

    for(i=0;i<BOMBCOUNT;i++)
    {
        mines[BOMBVector[i].x][BOMBVector[i].y] = -1; //Place -1 in board where BOMB lies.
    }
}

void GameBoard::placeNumbers()
{
    int i,j,p,q,cnt;
    for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            if(mines[i][j]!=-1)
            {
                cnt=0;
                for(p=i-1;p-i+1<3;++p)
                {
                    for(q=j-1;q-j+1<3;++q)
                    {
                        if(isValid(p,q))
                        {
                            if(mines[p][q] == -1) cnt++;
                        }
                    }
                }
                mines[i][j] = cnt;
            }
        }
    }
}

void GameBoard::unCover(int i, int j)
{
    if(COVERCOUNT == ROW*COL) //WHEN BOARD CLICKED FOR THE FIRST TIME
    {
    	
        placeBOMBs(i,j);
        placeNumbers();
    }

    if(mines[i][j]>0)
    {
        isCovered[i][j] = 0;
        COVERCOUNT--;
    }
    else if(mines[i][j] == -1)
    {
        int p;
        int e,f;
        for(e=0;e<ROW;e++)
        {
            for(f=0;f<COL;f++)
            {
                isCovered[e][f] = 0;
            }
        }
        COVERCOUNT = 0;
        GAMEOVER = true;
        over.playLaunch();                   //plays sound when game overs
    }
    else
    {
        queue< pair<int,int> > que;
        int p,q;
        que.push(pair<int,int> (i,j));
        isCovered[i][j] = 0;
        COVERCOUNT--;
        while(que.size())
        {
            p=que.front().first-1;
            q=que.front().second;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first-1;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first-1;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            que.pop();
        }
    }
    if (COVERCOUNT <= BOMBCOUNT)
    {
    	win.playLaunch();               //plays sound when user wins
	}
}

void GameBoard::printBoard()
{
    RenderWindow window(VideoMode(SCR_W,SCR_H), "Minesweeper");   //creates a window with title minesweeper
	Texture tile;
	Font font;
    font.loadFromFile("font/Pirata_One/PirataOne-Regular.ttf");   //loads the font from memory
	
	tile.loadFromFile("images/tiles.jpg");                        //loads the cell image from memory
	
	Sprite sprite(tile);
	
	Text text;
	text.setFont(font);
	
	int grid[ROW][COL];                                          //actual grid displayed in minesweeper window
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			grid[i][j]=10;                                      //displays uncovered cell
		}
	}
	
	while(window.isOpen())
	{ 		
		Vector2i pos = Mouse::getPosition(window);
		int x = pos.x / w;
		int y = pos.y / w;
		Event event;
		while(window.pollEvent(event))
		{
			if(event.type==Event::Closed)
			{
				GAMEOVER=true;
				window.close();
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{   
				    lc.playLaunch();                        //plays sound for left click
					unCover(x,y);
					for(int i=0;i<ROW;i++)
					{
						for(int j=0;j<COL;j++)
						{
							if(isCovered[i][j])
			            	{
			                	grid[i][j]=10;
			            	}
			            	else if(mines[i][j] == 0)
			            	{
			                	grid[i][j]=0;               //displays blank cell
			            	}
			            	else if(mines[i][j] == -1 && grid[i][j]==11)
			            	{
			            		grid[i][j]=11;              //displays flagged cell
			            	}
			            	else if(mines[i][j]==-1)
			            	{
			            		grid[i][j]=9;               //displays bombs 
							}
			            	else
			            	{
			            		grid[i][j]=mines[i][j];      //displays numbered cell
							}
						}
					}
				}
				else if (event.key.code == Mouse::Right)
				{   
				    rc.playLaunch();                         //plays sound for right click
					if (grid[x][y] == 11)
					{
						isCovered[x][y]=1;
						grid[x][y] = 10;
						cnt1++;
					}
					else if (grid[x][y] == 10 && cnt1>0)
					{
						isCovered[x][y]=0;
						grid[x][y] = 11;
						cnt1--;
					}
				}
			}
		}
		window.clear();
		if(GAMEOVER==true)
        {
        	for(int i=0;i<ROW;i++)
			{
				for(int j=0;j<COL;j++)
				{
					sprite.setTextureRect(IntRect(grid[i][j]*w, 0, w, w));
					sprite.setPosition(i*w, j*w);
					window.draw(sprite);
					
				}
			}
			window.display();
			window.clear();
        	text.setString("GAME OVER :(\n YOU CLICKED A BOMB !");
        	FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
			text.setPosition(sf::Vector2f(SCR_W/2.0f,SCR_H/2.0f));
			window.draw(text);	
		}
		else if(COVERCOUNT <= BOMBCOUNT)
        {   
            
            WIN = true;
            
            text.setString("YOU WON!!!!");
        	FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
			text.setPosition(sf::Vector2f(SCR_W/2.0f,SCR_H/2.0f));
        	window.draw(text);
            
            
        }
        else
        {
        	for(int i=0;i<ROW;i++)
			{
				for(int j=0;j<COL;j++)
				{
					sprite.setTextureRect(IntRect(grid[i][j]*w, 0, w, w));
					sprite.setPosition(i*w, j*w);
					window.draw(sprite);
					
				}
			}
		}
		
		window.display();
		
		
	}
		
}
    

GameBoard::GameBoard()
{
    //Initialize the board with rows and columns. Initialize mines[][]
    setUpBoards();
}


void newGame(int dif=1)                    //creates a game with given difficulty(1,2,3)
{   
    app.close(); 
    int bomb;
	switch(dif)
    {
    case 1:
        bomb = 10;
        ROW = COL = 9;
        SCR_H = w*ROW ;
		SCR_W = w*COL ; 
        break;
    case 2:
        bomb = 40;
        ROW = COL = 16;
        SCR_H = w*ROW ;
		SCR_W = w*COL ; 
        break;
    case 3:
    	bomb = 99;
    	ROW = COL = 20;
        SCR_H = w*ROW ;
		SCR_W = w*COL ; 
    	break;
    }
    BOMBCOUNT = bomb;
    cnt1 = bomb;
    COVERCOUNT = ROW*COL;
    GAMEOVER = false;
    WIN = false;
    GameBoard g;
    
    while(!GAMEOVER && !WIN)
    {
        g.printBoard();
        
    }
    
}

void selectOptions()                        //functiion to select options from the menu window
{   
    Texture bg;
    bg.loadFromFile("images/bg3.jpg");
    Sprite bg_s;
    bg_s.setTexture(bg);
    bg_s.setScale(0.75,0.75);
	while (app.isOpen())
	{
		Event e;

		while (app.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::KeyReleased:
				switch (e.key.code)
				{
				case Keyboard::Up:
					menu2.MoveUp();
					break;

				case Keyboard::Down:
					menu2.MoveDown();
					break;

				case Keyboard::Return:
					switch (menu2.GetPressedItem())
					{
					case 0:
						newGame(1);
						break;
					case 1:
						newGame(2);
						break;
					case 2:
					    newGame(3);
						break;
					}

					break;
				}

				break;
			case Event::Closed:
				app.close();

				break;

			}
		}

		app.clear();
        app.draw(bg_s);
		menu2.draw(app);

		app.display();
	}
	
}

void instructions()                         //creates the instructions window
{  
    RenderWindow inst(VideoMode(600, 600), "INSTRUCTIONS");
    Font font1;
    font1.loadFromFile("font/Pirata_One/PirataOne-Regular.ttf");
	Text texIN ;
	texIN.setFont(font1); 
	while (inst.isOpen())
	{
		Event e1;
        while (inst.pollEvent(e1))
		{
           	if(e1.type==Event::Closed)
			{
				inst.close();
			}
	    }
		inst.clear();
		texIN.setString("1) You are presented with a board of squares.\n2) Some squares contain mines (bombs), others don't.\n3)If you click on a square containing a bomb, you lose.\n4) If you manage to click all the squares (without clicking on any bombs) you win.\n5) Clicking a square which doesn't have a bomb reveals the number of\n neighbouring squares containing bombs.\n6) Use this information plus some guess work to avoid the bombs.\n7) To open a square, point at the square and click on it.\n8) To mark a square you think is a bomb, point and right-click.\n9) Easy level : 9x9 grid & 10 bombs\n Medium level : 16x16 grid & 40 bombs\n Difficult level : 20x20 grid & 99 bombs\n");
        texIN.setCharacterSize(18); 
        inst.draw(texIN);
		inst.display();
    }
}


int main()                            //to create the first menu
{
  Texture bg;
  bg.loadFromFile("images/bg3.jpg");
  Sprite bg_s;
  bg_s.setTexture(bg);
  bg_s.setScale(0.75,0.75);
  while (app.isOpen())
	{
		Event e;

		while (app.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::KeyReleased:
				switch (e.key.code)
				{
				case Keyboard::Up:
					menu1.MoveUp();
					break;

				case Keyboard::Down:
					menu1.MoveDown();
					break;

				case Keyboard::Return:
					switch (menu1.GetPressedItem())
					{
					case 0:
						selectOptions();
						break;
					case 1:
						instructions();
						break;
					case 2:
					    app.close();
						break;
					}

					break;
				}

				break;
			case Event::Closed:
				app.close();

				break;

			}
		}

		app.clear();
        app.draw(bg_s);
		menu1.draw(app);
		
		app.display();
	}
    
	
}
    

