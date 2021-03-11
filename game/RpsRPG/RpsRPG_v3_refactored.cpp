#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>
using namespace std;

/*
	���� ������2
	�� ����

	����ũ�� -> ���̵��� ���� ũ�Ⱑ �ٲ�
	Ÿ���� ������ �迭�� ������

	enum �ڷ���
		���ӵ� ������ �����͸� ǥ���� �� ����ϴ� �ڷ���

	enum [�ڷ����̸�] {���1, ���2, ...};
*/

enum MAPSIZE { BEGINNER_MAPSIZE = 10, INTERMIDIATE_MAPSIZE = 15, PROFESSIONAL_MAPSIZE = 20 };
enum TILETYPE { FOREST, SWAMP, EARTH };
enum DIFFICULTY { BEGINNER_LEVEL = 1, INTERMIDIATE_LEVEL, PROFESSIONAL_LEVEL };
enum BOSSCNT { BEGINNER_CNT = 3, INTERMIDIATE_CNT = 5, PROFESSIONAL_CNT = 7 };
enum RPS { ROCK, PAPER, SISSORS };

struct Hero {
	static const int MAX_LEVEL = 10;
	static const int HERO_SHAPE = 'O';

	string heroName;
	int maxHp;
	int currentHp;
	int currentExp;
	int currentLevel;
	int currentMoney;
	int currentDamage;
	int heroX;
	int heroY;

	int requiredExp = 10;
	void updateRequiredExp() {
		requiredExp = 10 * (currentLevel);
	}
	void updateStat() {
		currentDamage += 10;
		maxHp += 20;
		currentHp = maxHp;
	}
	float getExpToPercentage() {
		return currentExp / (float)requiredExp * 100;
	}
};

struct BossMonster {
	static const char BOSS_SHAPE = '+';

	string bossName;
	bool isAlive;

	int maxHp;
	int monsterHp;
	int treatExp;
	int treatMoney;
	int mosterLevel;
	int monsterType;
	int bossDamage;
	int bossX;
	int bossY;
};

struct BossGroup {
	BossMonster* bossMonsters = nullptr;
	int bossCnt;
};

struct RecoveryPotion {
	string potionName;
	int recoveryAmount;
	int potionPrice;
};

struct PotionShop {
	int hpPotionCnt;
	RecoveryPotion* hpPotions = nullptr;
};

struct FloorTile
{
	static const char FOREST_SHAPE = 'Y';
	static const char SWAMP_SHAPE = 'E';
	static const char EARTH_SHAPE = 'M';

	int tileX;
	int tileY;
	int type;//forest(0), swamp(1), land(0)
	char shape;// forest(Y), swamp(E), land(M)
};

// ����� ���� �Լ�
string getName() {
	string heroName = "";
	cout << "Hero Name : ";
	cin >> heroName;

	return heroName;
}

void initHero(Hero& hero) {
	hero.heroX = 0;
	hero.heroY = 0;
	hero.maxHp = 100;
	hero.currentHp = hero.maxHp;
	hero.currentMoney = 20;
	hero.currentLevel = 1;
	hero.currentExp = 0;
	hero.currentDamage = 50;
}

void printHeroInfo(Hero& gamePlayer) {
	cout << "#######################################" << endl;
	cout << "NAME : " << gamePlayer.heroName << endl;
	cout << "LV   : " << gamePlayer.currentLevel << "(" << gamePlayer.getExpToPercentage() << "%)" << endl;
	cout << "HP   : " << gamePlayer.currentHp << "/" << gamePlayer.maxHp << endl;
	cout << "MONEY: " << gamePlayer.currentMoney << endl;
	cout << "#######################################" << endl;
}

void putHero(char** gameMap, Hero& hero) {
	gameMap[hero.heroY][hero.heroX] = Hero::HERO_SHAPE;
}

bool moveHero(Hero& hero, int mapSize) {
	char userKey = _getch();
	switch (userKey) {
	case 'w':case 'W':
		if (hero.heroY > 0) hero.heroY--;
		break;
	case 's':case 'S':
		if (hero.heroY < mapSize - 1) hero.heroY++;
		break;
	case 'a':case 'A':
		if (hero.heroX > 0) hero.heroX--;
		break;
	case 'd':case 'D':
		if (hero.heroX < mapSize - 1) hero.heroX++;
		break;
	case 27://ESC Ű�� ������
		return false;
	}
}

// ���̵�, �� ũ�� ����
int getDifficulty() {
	int difficulty = 0;
	cout << "Select difficulty for this game(1:EASY, 2:NORMAL, 3:HARD) : ";
	cin >> difficulty;

	return difficulty;
}

int getMapSize(int difficultyLevel) {
	switch (difficultyLevel) {
	case DIFFICULTY::BEGINNER_LEVEL:
		return MAPSIZE::BEGINNER_MAPSIZE;
	case DIFFICULTY::INTERMIDIATE_LEVEL:
		return MAPSIZE::INTERMIDIATE_MAPSIZE;
	case DIFFICULTY::PROFESSIONAL_LEVEL:
		return MAPSIZE::PROFESSIONAL_MAPSIZE;
	}
}

char** createGameMap(int mapSize) {
	char** gameMap = new char* [mapSize];
	for (int i = 0; i < mapSize; i++)
		gameMap[i] = new char[mapSize];

	return gameMap;
}

void deleteGameMap(char** gameMap, int mapSize) {
	for (int i = 0; i < mapSize; i++) {
		delete[] gameMap[i];
	}
	delete[] gameMap;
}

void printGameMap(char** gameMap, int mapSize) {
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			cout << gameMap[i][j] << " ";
		}
		cout << endl;
	}
}


//FloorTile ���� �Լ�
FloorTile** createFloorTile(int currentMapSize) {
	FloorTile** gameMap = new FloorTile * [currentMapSize];
	for (int i = 0; i < currentMapSize; i++) {
		gameMap[i] = new FloorTile[currentMapSize];
	}

	return gameMap;
}

void deleteFloorTile(FloorTile** gameMap, int currentMapSize) {
	for (int i = 0; i < currentMapSize; i++) {
		delete[] gameMap[i];
	}
	delete[] gameMap;
}

void shuffleFloorTile(FloorTile** gameMap, int currentMapSize) {
	for (int i = 0; i < currentMapSize; i++) {
		for (int j = 0; j < currentMapSize; j++) {
			gameMap[i][j].tileX = j;
			gameMap[i][j].tileY = i;
			gameMap[i][j].type = rand() % 3;

			switch (gameMap[i][j].type) {
			case TILETYPE::EARTH: gameMap[i][j].shape = FloorTile::EARTH_SHAPE;
				break;
			case TILETYPE::FOREST: gameMap[i][j].shape = FloorTile::FOREST_SHAPE;
				break;
			case TILETYPE::SWAMP: gameMap[i][j].shape = FloorTile::SWAMP_SHAPE;
				break;
			}
		}
	}
}

void putTiles(char** gameMap, FloorTile** floorTile, int mapSize) {
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			gameMap[i][j] = floorTile[i][j].shape;
		}
	}
}

//���� ���� ���� �Լ�
void createBoss(BossGroup& bossGroup) {
	bossGroup.bossMonsters = new BossMonster[bossGroup.bossCnt];
}

void deleteBoss(BossGroup& bossGroup) {
	delete[] bossGroup.bossMonsters;
}

void setBossType(BossGroup& bossGroup, FloorTile** floorTile) {
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		//Ÿ�Լ���
		int type = floorTile[bossGroup.bossMonsters[i].bossY][bossGroup.bossMonsters[i].bossX].type;
		switch (type) {
		case TILETYPE::EARTH:
			bossGroup.bossMonsters[i].bossName = "Sandshrew(�𷡵���)";
			break;
		case TILETYPE::FOREST:
			bossGroup.bossMonsters[i].bossName = "Bulbasaur(�̻��ؾ�)";
			break;
		case TILETYPE::SWAMP:
			bossGroup.bossMonsters[i].bossName = "Grimer(������)";
			break;
		}
	}
}
void shuffleBoss(BossGroup& bossGroup, int mapSize) {

	for (int i = 0; i < bossGroup.bossCnt; i++) {
		bossGroup.bossMonsters[i].bossX = rand() % mapSize;
		bossGroup.bossMonsters[i].bossY = rand() % mapSize;

		if (bossGroup.bossMonsters[i].bossX == 0 && bossGroup.bossMonsters[i].bossY == 0) {
			i--; continue;
		}

		for (int j = 0; j < i; j++) {
			if (bossGroup.bossMonsters[i].bossX == bossGroup.bossMonsters[j].bossX && bossGroup.bossMonsters[i].bossY == bossGroup.bossMonsters[j].bossY) {
				i--;
				break;
			}
		}
	}
}

void setBossCnt(BossGroup& bossGroup, int difficultyLevel) {
	if (difficultyLevel == DIFFICULTY::BEGINNER_LEVEL)
	{
		bossGroup.bossCnt = BOSSCNT::BEGINNER_CNT;
	}
	else if (difficultyLevel == DIFFICULTY::INTERMIDIATE_LEVEL)
	{
		bossGroup.bossCnt = BOSSCNT::INTERMIDIATE_CNT;
	}
	else if (difficultyLevel == DIFFICULTY::PROFESSIONAL_LEVEL)
	{
		bossGroup.bossCnt = BOSSCNT::PROFESSIONAL_CNT;
	}
}

void initBoss(BossGroup& bossGroup, int difficultyLevel) {

	//���̵��� ���� ���� �ɷ�ġ ���
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		bossGroup.bossMonsters[i].bossName = "";
		bossGroup.bossMonsters[i].maxHp = 100 * difficultyLevel;
		bossGroup.bossMonsters[i].monsterHp = bossGroup.bossMonsters[i].maxHp;
		bossGroup.bossMonsters[i].mosterLevel = difficultyLevel * 3;
		bossGroup.bossMonsters[i].treatExp = (rand() % difficultyLevel + 1) * 5;
		bossGroup.bossMonsters[i].treatMoney = (rand() % difficultyLevel + 1) * 10;
		bossGroup.bossMonsters[i].bossDamage = (rand() % difficultyLevel + 1) * 5;
		bossGroup.bossMonsters[i].isAlive = true;
	}
}

void putBosses(char** gameMap, BossGroup& bossGroup) {
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (bossGroup.bossMonsters[i].isAlive)
			gameMap[bossGroup.bossMonsters[i].bossY][bossGroup.bossMonsters[i].bossX] = BossMonster::BOSS_SHAPE;
	}
}

void moveBoss(BossGroup& bossGroup, Hero& gamePlayer, int mapSize) {
	//ĳ���Ϳ� ������ �������� üũ
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (bossGroup.bossMonsters[i].isAlive && bossGroup.bossMonsters[i].bossX == gamePlayer.heroX && bossGroup.bossMonsters[i].bossY == gamePlayer.heroY) {
			return;
		}
	}

	//ĳ���Ϳ� ������ �ʾ�����
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (!bossGroup.bossMonsters[i].isAlive)// ������ ������� ������
			continue;//�������� �ʴ´�.

		int xBy = rand() % 3 - 1; // -1 0 1
		int yBy = ((xBy == -1 || xBy == 1) ? 0 : (rand() % 2) == 0 ? -1 : 1);

		bossGroup.bossMonsters[i].bossX += xBy;
		bossGroup.bossMonsters[i].bossY += yBy;

		//������ �������ϴ� ���⿡ ���� ������
		if (bossGroup.bossMonsters[i].bossX < 0 || bossGroup.bossMonsters[i].bossY < 0
			|| bossGroup.bossMonsters[i].bossX > mapSize - 1
			|| bossGroup.bossMonsters[i].bossY > mapSize - 1) {

			bossGroup.bossMonsters[i].bossX -= xBy;
			bossGroup.bossMonsters[i].bossY -= yBy;
			i--; continue;//�ٽ� ������
		}
		//�̵��Ϸ��� ��ġ�� �ٸ� ������ ������ �������� �ʴ´�.
		for (int j = 0; j < i; j++) {
			if (bossGroup.bossMonsters[j].isAlive
				&& bossGroup.bossMonsters[i].bossX == bossGroup.bossMonsters[j].bossX
				&& bossGroup.bossMonsters[i].bossY == bossGroup.bossMonsters[j].bossY) {

				bossGroup.bossMonsters[i].bossX -= xBy;
				bossGroup.bossMonsters[i].bossY -= yBy;
				break;
			}
		}
	}
}

//���ǻ�
void printPotionShop(Hero& gamePlayer, PotionShop& potionShop) {
	system("cls");
	cout << "#######################################" << endl;
	cout << "HeroHP   : " << gamePlayer.currentHp << "/" << gamePlayer.maxHp << endl;
	cout << "HeroMoney: " << gamePlayer.currentMoney << endl;
	cout << "#######################################" << endl;
	for (int i = 0; i < potionShop.hpPotionCnt; i++) {
		cout << "Potion[" << i + 1 << "] : " << potionShop.hpPotions[i].potionName << "\tRecovery : +" << potionShop.hpPotions[i].recoveryAmount << "\tPrice : $" << potionShop.hpPotions[i].potionPrice << endl;
	}
	cout << endl;
}

void visitPotionShop(Hero& gamePlayer) {
	PotionShop potionShop;
	potionShop.hpPotionCnt = 4;
	potionShop.hpPotions = new RecoveryPotion[potionShop.hpPotionCnt];
	int potionNum = 0;
	int potionCnt = 0;
	int recoveryAmount = 0;

	potionShop.hpPotions[0].potionName = "HP_10_RECOVERY_POTION";
	potionShop.hpPotions[0].recoveryAmount = 10;
	potionShop.hpPotions[0].potionPrice = 10;

	potionShop.hpPotions[1].potionName = "HP_20_RECOVERY_POTION";
	potionShop.hpPotions[1].recoveryAmount = 20;
	potionShop.hpPotions[1].potionPrice = 20;

	potionShop.hpPotions[2].potionName = "HP_30_RECOVERY_POTION";
	potionShop.hpPotions[2].recoveryAmount = 30;
	potionShop.hpPotions[2].potionPrice = 30;

	potionShop.hpPotions[3].potionName = "FULL_HP_RECOVERY_POTION";
	potionShop.hpPotions[3].recoveryAmount = 99999999;
	potionShop.hpPotions[3].potionPrice = 80;

	//���� �� ���
	printPotionShop(gamePlayer, potionShop);

	//���� ��ȣ�� ���� �޾Ƽ�
	do {
		cout << "Potion number, count(leave:0 0) : ";
		cin >> potionNum >> potionCnt;

		if (potionNum == 0 && potionCnt == 0) {//���� ������
			cout << "Good Bye~~" << endl;
			return;
		}
		else if (potionNum<1 || potionNum>potionShop.hpPotionCnt)
			continue;//�ƹ��͵� ���� ����
		else if (potionShop.hpPotions[potionNum - 1].potionPrice * potionCnt > gamePlayer.currentMoney) {//���� �ݾ� ����
			cout << "You don't have enough money!!" << endl;
		}
		else//���� �Է�
			break;
	} while (true);


	recoveryAmount = potionShop.hpPotions[potionNum - 1].recoveryAmount * potionCnt;
	if ((recoveryAmount + gamePlayer.currentHp) > gamePlayer.maxHp) {// �ִ� ü�� �ʰ� ���� �ʰ� ȸ���� ����
		recoveryAmount = gamePlayer.maxHp - gamePlayer.currentHp;
	}

	gamePlayer.currentHp += recoveryAmount;
	gamePlayer.currentMoney -= potionShop.hpPotions[potionNum - 1].potionPrice * potionCnt;

	//ȭ�� �� �� �� ���
	printPotionShop(gamePlayer, potionShop);

	cout << "Your HP got recovered by " << recoveryAmount << endl;
	cout << "You spent $" << potionShop.hpPotions[potionNum - 1].potionPrice * potionCnt << endl;
	delete[] potionShop.hpPotions;

	cout << "Press any key to keep playing" << endl;
	_getch();
}

void printRPS(BossGroup& bossGroup, Hero& gamePlayer, int idx) {
	system("cls");
	cout << "#######################################" << endl;
	cout << "BossName : " << bossGroup.bossMonsters[idx].bossName << endl;
	cout << "#######################################" << endl;
	cout << "HeroHP   : " << gamePlayer.currentHp << "/" << gamePlayer.maxHp << endl;
	cout << "BossHP   : " << bossGroup.bossMonsters[idx].monsterHp << "/" << bossGroup.bossMonsters[idx].maxHp << endl;
	cout << "#######################################" << endl;
}

//���������� ����
bool playRPS(BossGroup& bossGroup, Hero& gamePlayer) {
	bool defeatBoss = true;

	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (bossGroup.bossMonsters[i].isAlive && bossGroup.bossMonsters[i].bossX == gamePlayer.heroX && bossGroup.bossMonsters[i].bossY == gamePlayer.heroY) {//���� ��������

			bool keepFighting = true;
			int playerInput = 0;
			int bossInput = 0;

			while (keepFighting) {
				//ȭ�� ���
				printRPS(bossGroup, gamePlayer, i);

				//����� �Է�
				cout << "ROCK(0)/PAPER(1)/SCISSORS(2) : ";
				do {
					cin >> playerInput;
				} while (playerInput < RPS::ROCK || playerInput > RPS::SISSORS);

				//���� �Է�
				bossInput = rand() % 3;

				//����� / ���� ���������� ���
				cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
				cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;

				if (playerInput == bossInput) {//����� ���

				}
				else if (playerInput - bossInput == 1 || playerInput - bossInput == -2) {// �̰��� ���
					bossGroup.bossMonsters[i].monsterHp -= gamePlayer.currentDamage;// ���� ü�� ����

					if (bossGroup.bossMonsters[i].monsterHp <= 0) {//������ �׾��� ���
						//���� ��� ó��
						bossGroup.bossMonsters[i].monsterHp = 0;
						bossGroup.bossMonsters[i].isAlive = false;

						//���ŵ� ȭ�� ���
						printRPS(bossGroup, gamePlayer, i);
						cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;

						//��� ������ �׾����� Ȯ��
						bool clearStage = true;
						for (int j = 0; j < bossGroup.bossCnt; j++) {
							if (bossGroup.bossMonsters[j].isAlive)
								clearStage = false;
						}
						if (clearStage) {//��� ������ �׿����� ���� ����
							cout << "congratuation!! You cleared this game!!" << endl;
							_getch();
							return false;
						}

						//����ġ & �� ���
						gamePlayer.currentMoney += bossGroup.bossMonsters[i].treatMoney;
						gamePlayer.currentExp += bossGroup.bossMonsters[i].treatExp;
						cout << "You got $" << bossGroup.bossMonsters[i].treatMoney << ", now you have $" << gamePlayer.currentMoney << " in total" << endl;
						cout << "You got " << bossGroup.bossMonsters[i].treatExp << "EXP" << endl;

						if (gamePlayer.currentExp >= gamePlayer.requiredExp) {//������!!
							gamePlayer.currentExp -= gamePlayer.requiredExp;
							gamePlayer.currentLevel++;
							gamePlayer.updateRequiredExp();
							gamePlayer.updateStat();
							cout << "Level up!!! " << gamePlayer.currentLevel - 1 << " > " << gamePlayer.currentLevel << endl;
						}

						keepFighting = false;
						defeatBoss = true;
					}
				}
				else {// ���� ���
					gamePlayer.currentHp -= bossGroup.bossMonsters[i].bossDamage;

					if (gamePlayer.currentHp <= 0) {//������ �׾��� ���
						gamePlayer.currentHp = 0;

						//���ŵ� ȭ�� ���
						printRPS(bossGroup, gamePlayer, i);
						cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << endl;
						cout << "Game Over" << endl;

						cout << "Press any key to keep playing" << endl;
						_getch();
						keepFighting = false;//���� ����
						return false;//���� ����(keepPlaying�� false�� ����)
					}
				}
				cout << "Press any key to keep playing" << endl;
				_getch();
			}

			// ������������ ������ ������������ ����
			char userSelect = 'n';
			do {
				cout << "Do you want to go to the potion shop?(y/n) : ";
				cin >> userSelect;
				tolower(userSelect);
			} while (userSelect != 'y' && userSelect != 'n');
			if (userSelect == 'y')
				visitPotionShop(gamePlayer);
		}
	}
}



int main()
{
	srand(time(NULL));
	// ����ü ����
	Hero gamePlayer;
	BossGroup bossGroup;
	bool keepPlaying = true;
	bool isTurn = true;

	// ����� �̸� �ޱ�
	gamePlayer.heroName = getName();

	// ����� ���� �ʱ�ȭ
	initHero(gamePlayer);

	// ���̵� �Է¹ޱ�
	int difficultyLevel = getDifficulty();

	// �� ũ�� �ޱ�
	int currentMapSize = getMapSize(difficultyLevel);

	// �ٴ�Ÿ�� ����
	FloorTile** floorTile = createFloorTile(currentMapSize);

	// ��ŹŸ�� ����
	shuffleFloorTile(floorTile, currentMapSize);

	// ���� �� ����
	char** gameMap = createGameMap(currentMapSize);

	// ���� ���� ����
	setBossCnt(bossGroup, difficultyLevel);

	// ���� ����
	createBoss(bossGroup);

	// ���� �ʱ�ȭ
	initBoss(bossGroup, difficultyLevel);

	// ���� ��ġ ����
	shuffleBoss(bossGroup, currentMapSize);

	//���� �Ӽ� ����
	setBossType(bossGroup, floorTile);


	while (keepPlaying) {
		system("cls");

		// ĳ���� ���� ���
		printHeroInfo(gamePlayer);

		// �ٴ��� ����
		putTiles(gameMap, floorTile, currentMapSize);

		// �������� ����
		putBosses(gameMap, bossGroup);

		// �÷��̾� ����
		putHero(gameMap, gamePlayer);

		// ���Ӹ� ���
		printGameMap(gameMap, currentMapSize);

		// ĳ���� ������ �˰�����
		keepPlaying = moveHero(gamePlayer, currentMapSize);

		if (!keepPlaying) {
			cout << "Exit Game!!" << endl;
			break;
		}

		// ���� ������ �˰�����
		if (isTurn = !isTurn) {
			//���� �̵�
			moveBoss(bossGroup, gamePlayer, currentMapSize);
			//���� �Ӽ� ����
			setBossType(bossGroup, floorTile);
		}

		// ������ ��� ����������
		keepPlaying = playRPS(bossGroup, gamePlayer);
	}


	//�����Ҵ�� �޸� ����
	deleteGameMap(gameMap, currentMapSize);
	deleteFloorTile(floorTile, currentMapSize);
	deleteBoss(bossGroup);
	return 0;
}