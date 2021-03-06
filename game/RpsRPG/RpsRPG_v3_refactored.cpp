#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>
using namespace std;

/*
	용사는 절차적2
	맵 구성

	고정크기 -> 난이도에 따라 크기가 바뀜
	타일을 이차원 배열로 만들자

	enum 자료형
		연속된 형태의 데이터를 표현할 때 사용하는 자료형

	enum [자료형이름] {멤버1, 멤버2, ...};
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

// 사용자 관련 함수
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
	case 27://ESC 키를 누르면
		return false;
	}
}

// 난이도, 맵 크기 설정
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


//FloorTile 관련 함수
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

//보스 몬스터 관련 함수
void createBoss(BossGroup& bossGroup) {
	bossGroup.bossMonsters = new BossMonster[bossGroup.bossCnt];
}

void deleteBoss(BossGroup& bossGroup) {
	delete[] bossGroup.bossMonsters;
}

void setBossType(BossGroup& bossGroup, FloorTile** floorTile) {
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		//타입설정
		int type = floorTile[bossGroup.bossMonsters[i].bossY][bossGroup.bossMonsters[i].bossX].type;
		switch (type) {
		case TILETYPE::EARTH:
			bossGroup.bossMonsters[i].bossName = "Sandshrew(모래두지)";
			break;
		case TILETYPE::FOREST:
			bossGroup.bossMonsters[i].bossName = "Bulbasaur(이상해씨)";
			break;
		case TILETYPE::SWAMP:
			bossGroup.bossMonsters[i].bossName = "Grimer(질퍽이)";
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

	//난이도에 따라 보스 능력치 배분
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
	//캐릭터와 보스와 만났는지 체크
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (bossGroup.bossMonsters[i].isAlive && bossGroup.bossMonsters[i].bossX == gamePlayer.heroX && bossGroup.bossMonsters[i].bossY == gamePlayer.heroY) {
			return;
		}
	}

	//캐릭터와 만나지 않았으면
	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (!bossGroup.bossMonsters[i].isAlive)// 보스가 살아있지 않으면
			continue;//움직이지 않는다.

		int xBy = rand() % 3 - 1; // -1 0 1
		int yBy = ((xBy == -1 || xBy == 1) ? 0 : (rand() % 2) == 0 ? -1 : 1);

		bossGroup.bossMonsters[i].bossX += xBy;
		bossGroup.bossMonsters[i].bossY += yBy;

		//보스가 가려고하는 방향에 벽이 있으면
		if (bossGroup.bossMonsters[i].bossX < 0 || bossGroup.bossMonsters[i].bossY < 0
			|| bossGroup.bossMonsters[i].bossX > mapSize - 1
			|| bossGroup.bossMonsters[i].bossY > mapSize - 1) {

			bossGroup.bossMonsters[i].bossX -= xBy;
			bossGroup.bossMonsters[i].bossY -= yBy;
			i--; continue;//다시 돌리기
		}
		//이동하려는 위치에 다른 보스가 있으면 움직이지 않는다.
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

//포션삽
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

	//포션 샵 출력
	printPotionShop(gamePlayer, potionShop);

	//포션 번호와 개수 받아서
	do {
		cout << "Potion number, count(leave:0 0) : ";
		cin >> potionNum >> potionCnt;

		if (potionNum == 0 && potionCnt == 0) {//상점 나가기
			cout << "Good Bye~~" << endl;
			return;
		}
		else if (potionNum<1 || potionNum>potionShop.hpPotionCnt)
			continue;//아무것도 하지 않음
		else if (potionShop.hpPotions[potionNum - 1].potionPrice * potionCnt > gamePlayer.currentMoney) {//소지 금액 부족
			cout << "You don't have enough money!!" << endl;
		}
		else//정상 입력
			break;
	} while (true);


	recoveryAmount = potionShop.hpPotions[potionNum - 1].recoveryAmount * potionCnt;
	if ((recoveryAmount + gamePlayer.currentHp) > gamePlayer.maxHp) {// 최대 체력 초과 하지 않게 회복량 조절
		recoveryAmount = gamePlayer.maxHp - gamePlayer.currentHp;
	}

	gamePlayer.currentHp += recoveryAmount;
	gamePlayer.currentMoney -= potionShop.hpPotions[potionNum - 1].potionPrice * potionCnt;

	//화면 한 번 더 출력
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

//가위바위보 게임
bool playRPS(BossGroup& bossGroup, Hero& gamePlayer) {
	bool defeatBoss = true;

	for (int i = 0; i < bossGroup.bossCnt; i++) {
		if (bossGroup.bossMonsters[i].isAlive && bossGroup.bossMonsters[i].bossX == gamePlayer.heroX && bossGroup.bossMonsters[i].bossY == gamePlayer.heroY) {//둘이 만났으면

			bool keepFighting = true;
			int playerInput = 0;
			int bossInput = 0;

			while (keepFighting) {
				//화면 출력
				printRPS(bossGroup, gamePlayer, i);

				//사용자 입력
				cout << "ROCK(0)/PAPER(1)/SCISSORS(2) : ";
				do {
					cin >> playerInput;
				} while (playerInput < RPS::ROCK || playerInput > RPS::SISSORS);

				//보스 입력
				bossInput = rand() % 3;

				//사용자 / 보스 가위바위보 출력
				cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
				cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;

				if (playerInput == bossInput) {//비겼을 경우

				}
				else if (playerInput - bossInput == 1 || playerInput - bossInput == -2) {// 이겼을 경우
					bossGroup.bossMonsters[i].monsterHp -= gamePlayer.currentDamage;// 보스 체력 감소

					if (bossGroup.bossMonsters[i].monsterHp <= 0) {//보스가 죽었을 경우
						//보스 사망 처리
						bossGroup.bossMonsters[i].monsterHp = 0;
						bossGroup.bossMonsters[i].isAlive = false;

						//갱신된 화면 출력
						printRPS(bossGroup, gamePlayer, i);
						cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;

						//모든 보스가 죽었는지 확인
						bool clearStage = true;
						for (int j = 0; j < bossGroup.bossCnt; j++) {
							if (bossGroup.bossMonsters[j].isAlive)
								clearStage = false;
						}
						if (clearStage) {//모든 보스를 죽였으면 게임 종료
							cout << "congratuation!! You cleared this game!!" << endl;
							_getch();
							return false;
						}

						//경험치 & 돈 얻기
						gamePlayer.currentMoney += bossGroup.bossMonsters[i].treatMoney;
						gamePlayer.currentExp += bossGroup.bossMonsters[i].treatExp;
						cout << "You got $" << bossGroup.bossMonsters[i].treatMoney << ", now you have $" << gamePlayer.currentMoney << " in total" << endl;
						cout << "You got " << bossGroup.bossMonsters[i].treatExp << "EXP" << endl;

						if (gamePlayer.currentExp >= gamePlayer.requiredExp) {//레벨업!!
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
				else {// 졌을 경우
					gamePlayer.currentHp -= bossGroup.bossMonsters[i].bossDamage;

					if (gamePlayer.currentHp <= 0) {//유저가 죽었을 경우
						gamePlayer.currentHp = 0;

						//갱신된 화면 출력
						printRPS(bossGroup, gamePlayer, i);
						cout << "YOU : " << (playerInput == RPS::ROCK ? "ROCK" : (playerInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << "BOSS: " << (bossInput == RPS::ROCK ? "ROCK" : (bossInput == RPS::PAPER ? "PAPER" : "SCISSORS")) << endl;
						cout << endl;
						cout << "Game Over" << endl;

						cout << "Press any key to keep playing" << endl;
						_getch();
						keepFighting = false;//전투 종료
						return false;//게임 종료(keepPlaying에 false값 전달)
					}
				}
				cout << "Press any key to keep playing" << endl;
				_getch();
			}

			// 가위바위보가 끝나고 상점갈지여부 선택
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
	// 구조체 선언
	Hero gamePlayer;
	BossGroup bossGroup;
	bool keepPlaying = true;
	bool isTurn = true;

	// 히어로 이름 받기
	gamePlayer.heroName = getName();

	// 히어로 정보 초기화
	initHero(gamePlayer);

	// 난이도 입력받기
	int difficultyLevel = getDifficulty();

	// 맵 크기 받기
	int currentMapSize = getMapSize(difficultyLevel);

	// 바닥타일 생성
	FloorTile** floorTile = createFloorTile(currentMapSize);

	// 바탁타일 섞기
	shuffleFloorTile(floorTile, currentMapSize);

	// 게임 맵 생성
	char** gameMap = createGameMap(currentMapSize);

	// 보스 숫자 설정
	setBossCnt(bossGroup, difficultyLevel);

	// 보스 생성
	createBoss(bossGroup);

	// 보스 초기화
	initBoss(bossGroup, difficultyLevel);

	// 보스 위치 섞기
	shuffleBoss(bossGroup, currentMapSize);

	//보스 속성 설정
	setBossType(bossGroup, floorTile);


	while (keepPlaying) {
		system("cls");

		// 캐릭터 정보 출력
		printHeroInfo(gamePlayer);

		// 바닥재 놓기
		putTiles(gameMap, floorTile, currentMapSize);

		// 보스몬스터 놓기
		putBosses(gameMap, bossGroup);

		// 플레이어 놓기
		putHero(gameMap, gamePlayer);

		// 게임맵 출력
		printGameMap(gameMap, currentMapSize);

		// 캐릭터 움직임 알고리즘
		keepPlaying = moveHero(gamePlayer, currentMapSize);

		if (!keepPlaying) {
			cout << "Exit Game!!" << endl;
			break;
		}

		// 보스 움직임 알고리즘
		if (isTurn = !isTurn) {
			//보스 이동
			moveBoss(bossGroup, gamePlayer, currentMapSize);
			//보스 속성 설정
			setBossType(bossGroup, floorTile);
		}

		// 만났을 경우 가위바위보
		keepPlaying = playRPS(bossGroup, gamePlayer);
	}


	//동적할당된 메모리 해제
	deleteGameMap(gameMap, currentMapSize);
	deleteFloorTile(floorTile, currentMapSize);
	deleteBoss(bossGroup);
	return 0;
}