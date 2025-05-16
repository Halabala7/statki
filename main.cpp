#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
class Game;
class Ship;
class Board;
class Player;
class Human;
class Si;

class Game
{
    private:
        void startGameVsSi();
        void startGameVsHuman();
        bool chooseShipPlacement();


    public:
        Player* player1;
        Player* player2;

        Game(int gamemode);
        ~Game();



};
class Board
{
    private:
        char map[10][10];
        vector<Ship> ships;
        void printShips();
        friend class Si;
    public:
        Board();
        void printMap();
        bool autoPlaceShip(int size);
        bool checkShipPlace(int randomX, int randomY, int isVertical, int size);
        void addShipToBoard(const Ship& ship);
        bool receiveAttack(int x, int y);
        bool areAllShipsSunk() const;
};

class Player
{
    public:
        string name;
        bool manualShipPlacement(Board& board);
        bool autoShipPlacement(Board& board);
        virtual bool makeMove(Board& enemyBoard)=0;
        bool isValidCoordinate(const string& position);

};
class Human : public Player
{
    bool makeMove(Board& enemyBoard) override;



};
class Si : public Player
{
    bool makeMove(Board& enemyBoard) override;


};
class Ship
{
    private:
        int size;
        int position[2];
        int isVertical;
    	vector<bool> hits;
	public:
		void setPositionX(int x);
        void setPositionY(int y);
        bool getIsVertical() const;
        int getPositionX() const;
        int getPositionY() const;
        int getSizeOfShip() const;
        Ship(Board& board, int size);

        bool isHitted(int x, int y);
        bool isSunk() const;

};
Game::Game(int gamemode)
{
    // gra przeciwko SI
    if(gamemode == 1)
    {
        player1 = new Human();
        player2 = new Si();
        startGameVsSi();

    }
    // gra na dwoch graczy
    else if(gamemode == 2)
    {
        player1 = new Human();
        player2 = new Human();
    }
}
Game::~Game()
{
    delete player1;
    delete player2;
}
void Game::startGameVsSi()
{
    cout<<"grasz z SI\n";

    //tworzenie mapy czlowieka
    Board humanBoard;

    //manualne wstawianie statkow
    if(chooseShipPlacement())
    {
        cout<<"w trakcie tworzenia"<<endl;
    }
    //automatyczne wstawianie statkow
    else
    {
        Board humanBoard;

        player1->autoShipPlacement(humanBoard);
        cout<<"plansza gracza: \n";
        humanBoard.printMap();

        cout<<endl;

    }
    //tworzenie mapy Si
    Board siBoard;
    player2->autoShipPlacement(siBoard);
    cout<<"statki si postawione pomyslnie!\n";

    cout<<"plansza si: \n";
    siBoard.printMap();

  	while(true)
  	{
    	player1->makeMove(siBoard);
    	if(siBoard.areAllShipsSunk())
		{
			break;
		}
		player2->makeMove(humanBoard);

	}
	cout<<endl;
	cout<<endl;
	cout<<"koniec gry!"<<endl;
	cout<<"plansze: "<<endl;
	cout<<endl;
	cout<<"plansza gracza: "<<endl;
	humanBoard.printMap();
	cout<<endl;
	cout<<"plansza komputera: "<<endl;
	siBoard.printMap();
}
void Game::startGameVsHuman()
{
    cout<<"Opcja w trakcie tworzenia"<<endl;
    system("cls");
}
bool Game::chooseShipPlacement()
{
	return false;
	//reszta funkcji stworzona do dalszej rozbudowy programu
	/*
	cout<<"Czy chcesz ustawic samodzielnie statki? t/n"<<endl;
    char placeChoice;
    while (true)
    {
         if (cin>>placeChoice)
         {
            if (placeChoice == 't' || placeChoice == 'n')
            {
                break;
            }
            cout << "musisz podac t lub n!\n";

         }
         else
         {
            cout << "To nie jest litera!\n";
            cin.clear(); // Wyczysc flage bledu
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Zignoruj reszte danych
         }
    }
    if(placeChoice == 't')
	{
	    return true;
	}
	else if(placeChoice == 'n')
	{

	    return false;
	}
    */

}
bool Human::makeMove(Board& enemyBoard)
{

	string atackPosition = "";


	while(true)
	{
		cout<<"wybierz pozycje: [A-I][1-9]"<<endl;
		cin>>atackPosition;
		atackPosition[0] = toupper(atackPosition[0]); //zmiana pierwszej wspolrzednej na duza litere
		if(isValidCoordinate(atackPosition))
		{
			if(enemyBoard.receiveAttack(atackPosition[0], atackPosition[1]))
			{
				enemyBoard.printMap();
				return true;
			}
			else
			{
				enemyBoard.printMap();
				return false;
			}

		}
		else
		{
			cout<<"bledna wspolrzedna"<<endl;
		}

	}

    return false;
}
bool Player::isValidCoordinate(const string& position)
{
	if(position.size() == 2)
		{
			for(int i=0;i<9;i++)
			{
				char letter = i + 'A';
				if(position[0] == letter)
				{
					if(isdigit(position[1]))
					{

						return true;
					}
				}
			}
		}
	return false;
}
bool Si::makeMove(Board& enemyBoard)
{
    string attackPosition = "";

    while (true)
    {

        char xChar = 'A' + rand() % 9; // A-I
        char yChar = '1' + rand() % 9; // 1-9

        attackPosition = "";
        attackPosition += xChar;
        attackPosition += yChar;

        if (isValidCoordinate(attackPosition))
        {

            int x = xChar - '@';
            int y = yChar - '0';


            if (enemyBoard.map[y][x] == '.' || enemyBoard.map[y][x] == 'o')
            {
                cout << "SI atakuje pozycje: " << attackPosition << endl;
                enemyBoard.receiveAttack(xChar, yChar);
                enemyBoard.printMap();
                return true;
            }
        }
    }

    return false;
}
Ship::Ship(Board& board, int size) :hits(size, false)
{


    this -> size = size;
    int randomX, randomY;


        //losowanie czy statek bedzie wstawiony pionowo czy poziomo
        isVertical = (rand() %2)==0;

        do
        {
            randomX = rand() %9 + 1; //losowanie pozycji x i sprawdzenie czy nie wychodzi poza granice
            if(randomX + size > 9 && size>1)
            {
                randomX = randomX - size; //jeœli wychodzi to statek "przesuwamy" bli¿ej

            }
            randomY = rand() %9 + 1; //losowanie pozycji y i sprawdzenie czy nie wychodzi poza granice
            if(randomY + size > 9 && size>1)
            {
                randomY = randomY - size; //jeœli wychodzi to statek "przesuwamy" bli¿ej
            }

            //sprawdzamy kolizje z innymi statkami


            }while(!board.checkShipPlace(randomX,randomY,isVertical, size));
            position[0] = randomX;
            position[1] = randomY;
            board.addShipToBoard(*this);



}

void Ship::setPositionX(int x){position[0] = x;}

void Ship::setPositionY(int y){position[1] = y;}

int Ship::getPositionX() const{return position[0];}

int Ship::getPositionY()const{return position[1];}

bool Ship::getIsVertical()const{return isVertical;}

int Ship::getSizeOfShip() const{return size;}

bool Ship::isHitted(int x, int y)
{

	for(int i=0;i<getSizeOfShip();i++)
	{
		if(getIsVertical())
	    {
	        if(x == getPositionX() && y == getPositionY() + i)
	        {
	        	hits[i] = true;
	            return true;
			}
	    }
    	else
		{
	        if(x == getPositionX() + i && y == getPositionY())
	        {
	        	hits[i] = true;
	            return true;
	        }

	    }
	}
	return false;
}
bool Ship::isSunk() const
{
	for(int i=0;i<hits.size();i++)
	{
		if(hits[i] != true)
			return false;
	}
	return true;
}
Board::Board()
{
    //tworzenie podstawy mapy z wspólrzednymi na rogach
     map[0][0] = '/';

		for(int i=1; i<10; i++)
		{
			char numbers = i + '0';
			map[0][i] = numbers;
			char letters = i + '@';
			map[i][0] = letters;

			for(int j=1;j<10;j++)
			{
				map[i][j] = '.';
			}
		}

}

void Board::printMap()
{
    for(int i=0; i<10; i++)
	{
		for(int j=0;j<10;j++)
		{
			cout<<map[i][j]<<" ";
		}
		cout<<endl;
	}
}

bool Board::checkShipPlace(int randomX, int randomY, int isVertical, int size)
{

   if(isVertical)  // Statek poziomy
    {
        // Sprawdzamy prostokat: szerokosc 3 pola (x-1, x, x+1) x wysokosc (size+2)
        for(int i = -1; i <= 1; i++)  // Poziome sasiedztwo
        {
            for(int j = -1; j <= size; j++)  // Pionowe sasiedztwo
            {
                int checkX = randomX + i;
                int checkY = randomY + j;


                if(map[checkY][checkX] == 'o')
                {
                    return false;
                }

            }
        }
    }
    else  // Statek pionowy
    {
        // Sprawdzamy prostok¹t: szerokoœæ (size+2) x wysokoœæ 3 pola (y-1, y, y+1)
        for(int i = -1; i <= size; i++)  // Poziome s¹siedztwo
        {
            for(int j = -1; j <= 1; j++)  // Pionowe s¹siedztwo
            {
                int checkX = randomX + i;
                int checkY = randomY + j;


                if(map[checkY][checkX] == 'o')
                {
                    return false;
                }

            }
        }
    }

    return true;
}

void Board::addShipToBoard(const Ship& ship)
{

    ships.push_back(ship);
    if(ship.getIsVertical())
    {
        for(int i=0;i<ship.getSizeOfShip();i++)
        {
            map[ship.getPositionY()+ i][ship.getPositionX()] = 'o';
        }
    }
    else
    {
        for(int i=0;i<ship.getSizeOfShip();i++)
        {
            map[ship.getPositionY()][ship.getPositionX() + i] = 'o';
        }

    }

}

void Board::printShips()
{
    cout<<"wspolrzedne statkow: "<<endl;
    for(size_t i = 0;i<ships.size();i++)
    {
        cout<<ships[i].getSizeOfShip()<<" - polowy: "<<map[ships[i].getPositionY()][0]<<ships[i].getPositionX();
        if(ships[i].getIsVertical())
        {

            cout<<" pionowo"<<endl;
        }
        else
        {
            cout<<" poziomo"<<endl;
        }
    }
    cout<<endl;
}

bool Board::receiveAttack(int x, int y)
{
	x = x - 64; //przeliczamy spowrrotem z kodu ASCII na liczby aby sprawdzić współrzędne na mapie
	y = y - 48;

	for (Ship& ship : ships)
	{
        if (ship.isHitted(y, x))
		{
            map[x][y] = 'x';  // oznaczamy trafienie
            if (ship.isSunk())
				{
                	cout << "Zatopiono statek!\n";
            	}
			else
				{
            		cout << "Trafiony!\n";
            	}

            return true;
        }
    }

    map[x][y] = '-'; // pudło
    cout << "Pudlo!\n";
    return false;
}

bool Board::areAllShipsSunk() const {
    for (const Ship& ship : ships) {
        if (!ship.isSunk()) {
            return false;
        }
    }
    return true;
}


bool Player::autoShipPlacement(Board& board)
{

    cout<<"automatyczne wstawianie statkow\n";

    for(int i=4;i>0;i--)
    {
        Ship s1(board,i);
    }

	return true;
}

bool Player::manualShipPlacement(Board& board)
{
	//Kod do dalszego rozwoju programu - reczne wstawianie statkow na planszy
	/*
    cout<<"Wygenerowano mape, musisz teraz wstawic swoje statki:\n";
    cout<<"Do wyboru masz 4 rodzaje statkow:\n";
    cout<<"4 - polowy statek [1szt]\n";
    cout<<"3 - polowy statek [1szt]\n";
    cout<<"2 - polowy statek [1szt]\n";
    cout<<"1 - polowy statek [1szt]\n";
    cout<<endl;

    cout<<"mapa: \n";
    cout<<endl;
    board.printMap();
    cout<<endl;
    cout<<"statki wstawiasz za pomoca podania wspolrzednych miejsca na planszy \n";
    cout<<"wybierasz pomiedzy postawieniem statku poziomo lub pionowo: \n";
    for(int i=4; i>0 ; i--)
    {
        cout<<"wstaw statek "<<i<<" polowy\n";
    }
    */
    return false;
}



//kod do dalszego rozwoju programu - funkcja wyboru trybu gry
/*
int selectGameMode()
{
    int gameMode;
    while (true)
     {
         if (cin >> gameMode)
         {
            if (gameMode >= 1 && gameMode <= 2)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Liczba musi byc 1 lub 2!\n";
         }
         else
         {
            cout << "To nie jest liczba!\n";
            cin.clear(); // Wyczyœæ flagê b³êdu
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Zignoruj resztê danych
         }
     }

    return gameMode;
}

*/



int main(int argc, char** argv) {

     srand(time(0));

	 cout<<"witamy w grze w statki!\n";

	 //kod do dalszego rozwoju programu
	 /*
	 cout<<"Wybierz tryb gry: \n";
	 cout<<"[1] graj przeciwko SI\n";
	 cout<<"[2] gra dla dwoch graczy\n";


	 int gameMode = selectGameMode();
	*/
	int gameMode = 1;

	 switch(gameMode)
	 {
	 	case 1:
	 		{
	 		   Game g1(1);
			}
	 		break;
		case 2:
	 		{
	 		    cout<<"w trakcie tworzenia\n";
	 		}
	 		break;
	 	default:
	 		cout<<"nie ma takiej opcji\n";
	 		return 1;
	 }




	return 0;
}
