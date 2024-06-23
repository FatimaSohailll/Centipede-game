#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;
                                  // FUNCTION PROTOTYPES
void poisonousMush(int centipede[][4], int bullet[],sf:: Sprite poisonMush, int mushPois[]);  // spawns a poisonous muhsroom when fighter is het in player area
 
void fighterDestroyed(int centipede[][4], float player[], sf:: Sprite playerSprite, bool &flag,int mushpois[]);  //destroys the player if it gets hit by centipede/poisonous mushroom in the player area
  
void centiInPlayerArea(int centipede[][4]);  // manages the movement of the centipede after it enters the player area
  
void centiHit(int bullet[], int centipede[][4], int &score);  // checks if the centipede is hit by the bullet and splits the centipede accordingly
  
void mushroomDestroy(sf:: Sprite mushroom[],int mushroomy[],int mushroomx[], int bullet[], int n, int mushroomhit[], int &score); /*for deletion of mushroom and updation of score when bullet hits 
mushroom*/

void atemushroom(int mushroomx[],int centipede[][4],int mushroomy[],int num);  //for checking if centipede hits a mushroom and reverses its direction     
                     
void firebullet(int bullet[], float player[]);  // called when user enters a spacerbar and wants to fire a bullet
  
void DrawMushroom(sf::RenderWindow& window,sf::Sprite mushroom[],sf::Texture& mushroomtex, const int);  // draws mushrooms on the screen
  
void Drawcenti(sf::RenderWindow& window, int centipede[][4] ,sf::Sprite Centipede[],int pos, sf::Clock &Centiclock);  // draws centipede on the screen
  
void movecenti(int centipede[][4]);  //manages the movement of the centipede when its not in player area
  
void MovePlayer(float player[], sf::Sprite& playerSprite, sf::Event e);  // manages the movement of player according to users key presses
  
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);  //draws the player on screen

void moveBullet(int bullet[], sf::Clock& bulletClock);  //moves the bullet bu updating its coordinates

void drawBullet(sf::RenderWindow& window, int bullet[], sf::Sprite& bulletSprite); //draws bullet on screen

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar );

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background2.jpg");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255*0.6)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns-1) * boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	bool fighterdes=true;          // a flag to indicate if fighter is destroyed in player area

	// Initializing Bullet and Bullet Sprites.
	int bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = 0;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	bulletSprite.setColor(sf::Color::White);
	
	//Initiallizing Mushrooms
	const int num= rand()%11+20;         //generating a random number for mushrooms
	sf:: Texture mushroomtex;
	sf:: Sprite mushroom[num];       
	int mushroomhit[num]={0};         //array to count how many times a mushroom was hit
	int mushroomcordx[num];           //array for storing x-coordinates of mushrooms
	int mushroomcordy[num];           //array for storing y-coordinates of mushrooms
	mushroomtex.loadFromFile("Textures/mushroom.png");
	for (int x=0;x<num; x++)
        {
             	mushroomcordx[x]=rand()%928;                  //loop to generate a random x coordinate for every mushroom
        }
        for (int x=0;x<num; x++)
        {
             	mushroomcordy[x]=rand()%736;                 //loop to generate a random y coordinate for every mushroom
        }
        for (int x=0;x<num; x++)
        {
		mushroom[x].setTexture(mushroomtex);
	      	mushroom[x].setTextureRect(sf::IntRect(0, 0, 32,32));
	      	mushroom[x].setPosition(mushroomcordx[x],mushroomcordy[x]);             //giving every mushroom a texture and position
       
        }
	
	//Initiallizing centipede
	int centipede[12][4]={0};              //2d array to store every centipede segment's x position, y position, direction, and wether its an end segment or not
	int centipedepos=rand()%928;                    //random starting position for centipede
	sf::Clock centiclock;
	sf::Texture centipedeTexture;
	sf:: Texture centihead;
	sf::Sprite Centipede[12];
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	centihead.loadFromFile("Textures/c_head_left_walk.png");
	for (int i=0; i<12; i++)
        {  
            	if (i==0)
            	{
               		Centipede[i].setTexture(centihead);                          //giving the head a texture
	       		Centipede[i].setTextureRect(sf::IntRect(0, 0,28,32));
                }
           	else                                                                     //giving each segment a texture
            	{
               		Centipede[i].setTexture(centipedeTexture);
	       		Centipede[i].setTextureRect(sf::IntRect(0, 0,28,32));
                 
            	}
       }
       int pos=28;              
       for (int i=0; i<12; i++)
       {  
           	centipedepos+=pos;                     //incrementing the original random position by 28 as width of each segment is 28 pixels
           	centipede[i][x]=centipedepos;       // 0 coloumn is for x position and 1 for y
                
       }
       for (int i=0; i<12; i++)
       {
       		centipede[i][y]=0;                  // y coordinate of every segment at the start is zero as it starts from the top
       }
       for (int i=11; i>=0; i--)
       {  
           	Centipede[i].setPosition(centipede[i][x], centipede[i][y]);      //setting position of centipede
                 
       }
       for (int i=0; i<12; i++)
       {   
           	centipede[i][2]=-1;           //2 is for direction
                 
       }
       centipede[0][3]=1;           //3 is for indicating the end segments of the centipede, marking head and last segment as end sengemnts
       centipede[11][3]=1;   	   
       for (int i=1; i<11; i++)

       {   
           	centipede[i][3]=0;           //marking rest as 0 because they are not end segments
                 
       }
       pos=1;
       bool flag=false;              // flag to indicate if centipede has entered player area
       
       //initialising the text
       sf::Font fontt;
       fontt.loadFromFile("/usr/share/fonts/type1/urw-base35/C059-Bold.t1");
       sf::Text Score;
       Score.setFont(fontt);
       Score.setFillColor( sf::Color::Black);
       Score.setPosition(0,0);
       Score.setCharacterSize(40);
       Score.setStyle(sf::Text:: Underlined);
       
       //initialising poisonous mushroom
        int mushpois[3] = {0};
	sf::Sprite poisonMush;
	sf::Texture poisonTexture;
	poisonTexture.loadFromFile("Textures/mushroom.png");
	poisonMush.setTexture(poisonTexture);
	poisonMush.setTextureRect(sf::IntRect(0, 32, 32, 32));
                             
        
        int score=0;        //initialises total score
        
       while (window.isOpen()) 
       {

		window.draw(backgroundSprite);
		
		drawPlayer(window, player, playerSprite);
		
		DrawMushroom(window,mushroom,mushroomtex,num);
		
		for (int a=0; a<12; a++)
		{
			if (centipede[a][y]>736) //checks if any segment has entered the player area
				flag=true; 
		} 
		
		if (flag)
		{
		       centiInPlayerArea(centipede);
		       if (mushpois[2]==0)   
		       		poisonousMush(centipede,bullet,poisonMush,mushpois) ;                          //these funtions are only called when centipede is in player area
		       if (fighterdes)
		       		fighterDestroyed(centipede,player, playerSprite, fighterdes,mushpois);
		       if (mushpois[2]==1)
		       		poisonMush.setPosition(mushpois[x],mushpois[y]);
		       		window.draw(poisonMush);
		               
		       	  	
		}
		else
		{
			movecenti(centipede);                           //if centipede is not in player are this manages its movement
			centiHit(bullet,centipede,score);
		}	
		flag=false; //flag set to false as we want to check for for every iteration 
		
		Drawcenti(window, centipede , Centipede,pos, centiclock);
		
		atemushroom(mushroomcordx,centipede,mushroomcordy,num);
		
		mushroomDestroy(mushroom,mushroomcordy,mushroomcordx, bullet,num,mushroomhit,score);
		
		if (bullet[exists] == true)
	        {
		
	           	moveBullet(bullet, bulletClock);
		   	drawBullet(window, bullet, bulletSprite);     //these are only called when bullet is in the frame
		}
                	     
                sf::Event e;
                while (window.pollEvent(e)) 
                {
	              if (e.type == sf::Event::Closed)
	              {
		         	window.close();
	              }
	              if (e.type == sf::Event::KeyPressed)
	              {
	                 	if ((e.key.code==sf::Keyboard::Space)&&(bullet[exists]==0))   //calls firebullet() if user presses space and bullet is not in frame 
	                    		firebullet(bullet, player);
	                 	else
		            		MovePlayer(player,playerSprite,e);   
	              }
               }
               
                Score.setString("Score: " + std::to_string(score));       //sets text to updated score 
                 
                window.draw(Score);
		window.display();
		window.clear();
		
	}
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) 
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(int bullet[], sf::Clock& bulletClock) 
{
        if (bullet[exists]==1)     //if bullet is inside the frame
        {
		if (bulletClock.getElapsedTime().asMilliseconds() < 3)
			return;
     
	        bulletClock.restart();
	        bullet[y] -= 1;
	        	
	        if (bullet[y] < -32)                       //if bullet is out of the frame 
			bullet[exists] = false;
        }
}
void drawBullet(sf::RenderWindow& window, int bullet[], sf::Sprite& bulletSprite) 
{
	 bulletSprite.setPosition(bullet[x], bullet[y]);
	 window.draw(bulletSprite);
}

void MovePlayer(float player[], sf::Sprite& playerSprite,sf:: Event e) 
{
         if (e.key.code==sf::Keyboard::Down)
         {  
        	 if (player[y]==928)                        //bottom-most position
                 	player[y]+=0;
                 else
                        playerSprite.setPosition(player[x], player[y]+=32);
         }      
	 if (e.key.code==sf::Keyboard::Up)
	 {
	         if (player[y]==768)               // top most position which is allowed for player
	                player[y]+=0;
                 else
                        playerSprite.setPosition(player[x], player[y]-=32);
         }          
         if (e.key.code==sf::Keyboard::Left)
         {
                 if (player[x]==0)         //left most position
                        player[x]+=0;         
                 else
                        playerSprite.setPosition(player[x]-=32, player[y]);
         }      
         if (e.key.code==sf::Keyboard::Right)
         {
                 if (player[x]==928)          //right most position
                        player[x]+=0;
                 else
                        playerSprite.setPosition(player[x]+=32, player[y]);
         }
}

void Drawcenti(sf::RenderWindow& window, int centipede[][4] ,sf::Sprite Centipede[],int pos, sf:: Clock& Centiclock)
{    
	if (Centiclock.getElapsedTime().asMilliseconds() < 3)      //to maintime time delays as we do not want our centipede to move too fast
        {
        	for (int i=11; i>=0; i--)
                {   
                     	window.draw(Centipede[i]);         //draws centipede with the same coordinates as before  
                }
    	        return;
        }
        else
        {
                Centiclock.restart();
                for (int i=11; i>=0; i--)
                {  
                     	centipede[i][x]+=(pos*centipede[i][2]);
               		Centipede[i].setPosition(centipede[i][x], centipede[i][y]);          //now updates its coordinates
                	window.draw(Centipede[i]);
                } 
        }               
}

void movecenti(int centipede[][4])
{      
	for(int i=0; i<12; i++)            
	{
	       if (centipede[i][3]==1)               // checks if the segment is an end-segment
	       {
	       		int next =0;
	                for(int j=i+1; j<12 ;j++)
	                {
	                	if(centipede[j][3]==1)     //searches for the next end segment
	                	{
	                		next=j;
	                		break;
	                	}
	                }
	                if(centipede[next][x]<0)                     //condition if it reaches the left corner
	                {
	                	if (centipede[i][x]<centipede[next][x])
	                	{
	                		int temp;
	                        	temp=centipede[i][x];
	              			centipede[i][x]=centipede[next][x];           //manages the turning of the centipede from the first to the nxt end segment
	             			centipede[next][x]=temp;
	             			for( int k=i; k<=next; k++)
	             			{
	                        		centipede[k][y] +=32;            //moves every segment b/w these two end segments one row down
	                        	}
	             			for (int l=i;l<=next;l++)
	              			{
                     		        	centipede[l][2]*=-1;            //reverse the direction of every segment b/w these two end segments
                                	}  
	                	}
	                }         
              		else if (centipede[next][x]>928)                 //condition if it reaches the right corner
              		{
                      		if (centipede[i][x]>centipede[next][x])
	               	        {
	                  		int temp;
	                 		temp=centipede[i][x];
	                 		centipede[i][x]=centipede[next][x];
	                  		centipede[next][x]=temp;
	                 		for( int k=i; k<=next; k++)
	             			{
	                        		centipede[k][y] +=32;
	                        	} 
	                 		for (int l=i;l<=next;l++)
	              			{
                     		        	centipede[l][2]*=-1;
                                	}   
	              		}  
             		}
             		i=next;
             }
	}
             
}

void DrawMushroom(sf::RenderWindow& window,sf::Sprite mushroom[],sf::Texture& mushroomtex, int n)
{   
	for (int x=0;x<n; x++)
        {
		window.draw(mushroom[x]);
        }
}

void firebullet(int bullet[], float player[])
{
	bullet[x]=player[x];
        bullet[y]=player[y];            //cordinates of bullet are the same as player as it is fired from the player
        bullet[exists]=1;
}

void atemushroom(int mushroomx[],int centipede[][4],int mushroomy[],int num)
{
	for(int i=0; i<num; i++)
        {
        	if ((centipede[0][x]>=mushroomx[i]&&(centipede[0][x]<=mushroomx[i]+32))&&((centipede[0][y]>=mushroomy[i])&&(centipede[0][y]<=mushroomy[i]+32))||(centipede[0][x]>=mushroomx[i]&&(centipede[0][x]<=mushroomx[i]+32))&&((centipede[0][y]>=mushroomy[i])&&(centipede[0][y]<=mushroomy[i]+32))) //condition to check for collision
                {
                	for(int i=0; i<12; i++)
			{
	       			if (centipede[i][3]==1)
	       			{
	       				int next =0;
	                		for(int j=i+1; j<12 ;j++)
	                		{
	                			if(centipede[j][3]==1)
	                			{		                //searches for two adjacent end segments
	                				next=j;
	                				break;
	                			}
	              			}
	                		
	                		int temp;
	                        	temp=centipede[i][x];
	              			centipede[i][x]=centipede[next][x];
	             			centipede[next][x]=temp; 
	             			for( int k=i; k<=next; k++)                      
	             			{                                            //moves every segment b/w these two end segments one row down
	                        		centipede[k][y] +=32;
	                        	} 
	             			for (int l=i;l<=next;l++)
	              			{
                     		        	centipede[l][2]*=-1;                 // turns the centipede by switching direction
                                	}  
                                	i=next;
	                	}
	                
             		}
             		
                }
	}
             
}
                        

void mushroomDestroy(sf:: Sprite mushroom[],int mushroomy[],int mushroomx[], int bullet[], int n, int mushroomhit[], int &score)
{
	for (int i=0; i<n; i++)
	{ 
		if ((bullet[y]>=mushroomy[i])&&(bullet[y]<=mushroomy[i]+32)&&(bullet[x]>=mushroomx[i])&&(bullet[x]<=mushroomx[i]+32))  //collision detection
		{
		        
			mushroomhit[i]++;                //counts the number of hits that mushroom has
			if (mushroomhit[i]==2)
			{
				mushroom[i].setPosition(-32,-32);    // removes mushroom from screen 
				score++;                            //updates score
			}
			
			return;
		}
	}
}
				
void centiHit(int bullet[], int centipede[][4], int &score)
{
        
	for (int i=0; i<12;i++)
	{
        	if (((bullet[x]>=centipede[i][x]-28)&&(bullet[x]<=centipede[i][x]+28))&&(centipede[i][y]+32>=bullet[y]))        //checks for collision
        	{  
        		if (centipede[i][3]==1)
        		{
        			score+=20;    //if bullet hits head add 20
        			int next =0;
        		        for(int j=i+1; j<12 ;j++)
	                	{
	                		if (centipede[j][2]==centipede[i][2])
	                		{		
	                			centipede[j][x]=-100;
	                			centipede[j][x]=-100;              //elliminates this centipede portion as the head was hit by the bullet
	                			next=j;
	                		}
	                		else
	                			break;
	                	}                
	                }
        		else
        	        {
        	        	score+=10;                 // if bullet hits body add 10
        			int next =0;
        			centipede[i][3]=1;
	               		for(int j=i+1; j<12 ;j++)
	                	{
	                		if (centipede[j][2]==centipede[i][2])
	                		{		
	                			centipede[j][2]*=-1;            //reverses direction to split the centipede
	                			next=j;
	                		}
	                		else
	                			break;
	                	}
	             		centipede[i][2]*=-1;
	                	centipede[next][3]=1;         // marks it as an end segmant
        		}
        	}
        }
}

void centiInPlayerArea(int centipede[][4])
{
	for(int i=0; i<12; i++)
	{
		if((centipede[i][3]=1&&(centipede[i][y]>736)))             // checks if segment is end segment and is in the player area
		{
			int next =0;
	                for(int j=i+1; j<12 ;j++)
	                {
	                	if(centipede[j][3]==1)
	                	{		                                 //searches for next end segmant
	               			next=j;
	                		break;
	                	}
	            	}
	                		
	                if(centipede[next][x]<0)
	                {
	                	if (centipede[i][x]<centipede[next][x])
	                	{
	                		int temp;
	                        	temp=centipede[i][x];
	              			centipede[i][x]=centipede[next][x];
	             			centipede[next][x]=temp;
	             			for (int l=i;l<=next;l++)
	              			{
                     		        	centipede[l][2]*=-1;                      //reverses direction
                                	}  
	                	}
	                }         
              		else if (centipede[next][x]>928)
              		{
                      		if (centipede[i][x]>centipede[next][x])
	               	        {
	                  		int temp;
	                 		temp=centipede[i][x];
	                 		centipede[i][x]=centipede[next][x];     
	                  		centipede[next][x]=temp; 
	                 		for (int l=i;l<=next;l++)
	              			{
                     		        	centipede[l][2]*=-1;
                                	}   
	              		}  
             		}
             		i=next;
                }
        }
}

void fighterDestroyed(int centipede[][4], float player[], sf:: Sprite playerSprite, bool &flag, int mushpois[])
{
        for (int i=0; i<12; i++)
        {
                
		if (((centipede[i][x]==player[x])&&(centipede[i][y]==player[y]))||((mushpois[x]==player[x])&&(mushpois[y]==player[y])))  //checks if player collided with the centipede
		{
		   flag=false;    // sets flag false to idicate that it is hit
		    player[x]=-100;
		    player[y]=-100;
			playerSprite.setPosition(player[x],player[y]);   //eliminates player from screen
			break;
		}
	}
}

void poisonousMush(int centipede[][4], int bullet[],sf:: Sprite poisonMush, int mushPois[])
{
	for (int i=0; i<12; i++)
        {
                
		if (((bullet[x]>=centipede[i][x]-28)&&(bullet[x]<=centipede[i][x]+28))&&(centipede[i][y]+32>=bullet[y]))     //collision detection condition
		{
		   mushPois[x]=bullet[x];
		   mushPois[y]=bullet[y];      // cordinates of poisonous mushroom would be the same as bullet at that point
		   mushPois[2]=1;  // indicates that now poisonous mushroom will be drawn
		   poisonMush.setPosition(mushPois[x],mushPois[y]);                     //sets position of poisonous mushroom
		   
		   break;
		}
	}
}
