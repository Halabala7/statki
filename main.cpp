#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <limits>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

class Board;

class Player
{


};
class Ship
{
    private:
        int size;
        int position[2];
        int isVertical;
        vector <bool> hits;
    public:

        Ship(Board& board, int size);
        void setPositionX(int x);
        void setPositionY(int y);
        bool getIsVertical() const;
        int getPositionX() const;
        int getPositionY() const;
        int getSizeOfShip() const;
        bool isHitted(int x, int y);
        bool isSunk();

};

class Board
{
    private:
        char map[10][10];
        vector<Ship> ships;
    public:
        Board();
        void printMap();
        bool autoPlaceShip(int size);
        bool checkShipPlace(int randomX, int randomY, int isVertical, int size);
        void addShipToBoard(const Ship& ship);
        void printShips();
};
Ship::Ship(Board& board, int size)
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
                randomX = randomX - size; //je�li wychodzi to statek "przesuwamy" bli�ej

            }
            randomY = rand() %9 + 1; //losowanie pozycji y i sprawdzenie czy nie wychodzi poza granice
            if(randomY + size > 9 && size>1)
            {
                randomY = randomY - size; //je�li wychodzi to statek "przesuwamy" bli�ej
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

Board::Board()
{
    //tworzenie podstawy mapy z wsp�rzednymi na rogach
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
        // Sprawdzamy prostok�t: szeroko�� 3 pola (x-1, x, x+1) x wysoko�� (size+2)
        for(int i = -1; i <= 1; i++)  // Poziome s�siedztwo
        {
            for(int j = -1; j <= size; j++)  // Pionowe s�siedztwo
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
        // Sprawdzamy prostok�t: szeroko�� (size+2) x wysoko�� 3 pola (y-1, y, y+1)
        for(int i = -1; i <= size; i++)  // Poziome s�siedztwo
        {
            for(int j = -1; j <= 1; j++)  // Pionowe s�siedztwo
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

void autoShipPlacement(Board& board)
{
    system("cls");
    cout<<"automatyczne wstawianie statkow\n";

    for(int i=4;i>0;i--)
    {
        Ship s1(board,i);
    }

}
void manualShipPlacement(Board& board)
{
    system("cls");
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
}
int selectGameMode()
{
    int gameMode;
    while (true)
     {
         if (cin >> gameMode)
         {
            if (gameMode >= 1 && gameMode <= 2)
            {
                break;
            }
            cout << "Liczba musi byc 1 lub 2!\n";
         }
         else
         {
            cout << "To nie jest liczba!\n";
            cin.clear(); // Wyczy�� flag� b��du
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Zignoruj reszt� danych
         }
     }

    return gameMode;
}
void playVsSi()
{
    system("cls");
    cout<<"wybrales gre z SI\n";
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
            cin.clear(); // Wyczy�� flag� b��du
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Zignoruj reszt� danych
         }
    }
    Board playerBoard;
    if(placeChoice == 't')
    {
        manualShipPlacement(playerBoard);
    }
    else
    {
        autoShipPlacement(playerBoard);
        cout<<"plansza gracza: \n";
        playerBoard.printMap();

        cout<<endl;



    }
    Board siBoard;
    autoShipPlacement(siBoard);
    cout<<"statki si postawione pomyslnie!\n";

    cout<<"plansza si: \n";
    siBoard.printMap();
}
void playVsPlayer()
{
    cout<<"wybrales gre na dwie osoby\n";
}

int main(int argc, char** argv) {

     srand(time(0));
	 cout<<"witamy w grze w statki!\n";
	 cout<<"Wybierz tryb gry: \n";
	 cout<<"[1] graj przeciwko SI\n";
	 cout<<"[2] gra dla dwoch graczy\n";


	 int gameMode = selectGameMode();


	 switch(gameMode)
	 {
	 	case 1:
	 		{
	 		    playVsSi();
			}
	 		break;
		case 2:
	 		{
	 		    playVsPlayer();
	 		}
	 		break;
	 	default:
	 		cout<<"nie ma takiej opcji\n";
	 		return 1;
	 }




	return 0;
}
