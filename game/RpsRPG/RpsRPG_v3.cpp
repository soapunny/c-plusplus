//#include <iostream>
//#include <string>
//#include <ctime>
//#include <conio.h>
//#include <Windows.h>
//using namespace std;
//
////실습
////영웅은 절차적
//// 1) 텍스트로 진행되는 게임
//// 2) 게임이 시작되면 영웅의 이름과 난이도를 입력
//// 3) 난이도에 따라서 영웅의 HP/ 만나게 되는 몬스터의 숫자가 변동
//// 4) 몬스터를 잡으면 랜덤하게 돈을 획득한다.(0~100)
//// 5) 몬스터를 잡으면 던전을 계속 탐험할지 상점을 들를지 결정한다.
//// 6) 상점에 들르면 돈을 소모해서 HP를 회복할 수 있다.
//// 6_1) 전체회복, +10, +20, +30 차등회복
//// 7) HP가 0이 되면 게임오버
//// 8) 몬스터를 모두 잡으면 게임클리어
//
//// 0) 가위바위보 순서도
//// 0) 가위바위보(비기면 승패가 정해질 때까지 반복) - 선행
//// 0_1) 졌을 때만 영웅 HP가 줄어든다.
//// 0_2) 몬스터는 한번 지면 바로 죽음
//
////#####v2 업데이트#####
////1. 난이도에 따라 맵의 크기가 변경된다.
////2. 용사가 두 번 이동할 때, 적도 한 번 이동한다.
//
////#####v3 업데이트#####
////배열 / 구조체 / 함수를 최대한 활용해보자.
////맵의 구성 : 타일로 구성된다.
////타일에는 종류가 있다. (숲, 늪, 땅)마다 만나는 몬스터가 다르다.
////타일		: x좌표, y좌표, 타입, 모양
////플레이어	: 이름, 최대HP(레벨에 따라 증가), HP, 경험치, 레벨, 소유골드
////몬스터	: 이름, 최대HP, HP, 획득할 수 있는 경험치 / 골드
////물약		: 이름, 가격, 회복량
////상점		: 물약 종류, 수량
//
//struct BossMonster {
//	static const int BEGINNER_LEVEL = 1;
//	static const int INTERMIDIATE_LEVEL = 2;
//	static const int PROFESSIONAL_LEVEL = 3;
//	static const int TOTAL_WAIT_TIME = 2;
//
//	int current_wait_time = 0;
//	string monsterName;
//	int maxHp = 100;
//	int currentHp = 100;
//	int treatMoney = 10; //획득할 수 있는 돈
//	int treatEXP = 10; //획득할 수 있는 경험치
//	int monsterLevel = 1;
//	int monsterDamage;
//	int bossX;
//	int bossY;
//	bool isAlive = true;
//
//	void setDifficulty(int monsterLevel) {
//		if (monsterLevel < BEGINNER_LEVEL || monsterLevel > PROFESSIONAL_LEVEL + 5) {
//			cout << "Type a proper level(1~5)!!" << endl;
//			return;
//		}
//		monsterDamage = monsterLevel * 10;
//		treatEXP = monsterLevel * 10;
//		treatMoney = monsterLevel * 10 + rand() % 50;
//		monsterName = "BOSS" + to_string(monsterLevel);
//		this->monsterLevel = monsterLevel;
//	}
//};
//
//struct BossGroup {
//	static const int BEGINNER_BOSS_CNT = 3;
//	static const int INTERMIDIATE_BOSS_CNT = 5;
//	static const int PROFESSIONAL_BOSS_CNT = 7;
//
//	int currentBossCnt;
//	BossMonster* bossMon = nullptr;
//
//	void deleteBossMon() {
//		delete[] bossMon;//bossMon memory
//	}
//};
//struct FloorTile {
//	static const char GRASS_CHAR = '#';
//	static const char SWAMP_CHAR = '@';
//	static const char SAND_CHAR = '$';
//	static const int GRASS_TYPE = 0;
//	static const int SWAMP_TYPE = 1;
//	static const int SAND_TYPE = 2;
//
//	int tileX;
//	int tileY;
//	int tileType;
//	char tileShape;
//
//	void setTileShape(int tileType) {
//		switch (tileType) {
//		case 0: tileShape = GRASS_CHAR;
//			break;
//		case 1: tileShape = SWAMP_CHAR;
//			break;
//		case 2: tileShape = SAND_CHAR;
//			break;
//		}
//	}
//};
//
//struct GameMap {
//	static const int BEGINNER_MAP_SIZE = 10;
//	static const int INTERMIDIATE_MAP_SIZE = 15;
//	static const int PROFESSIONAL_MAP_SIZE = 20;
//	int currentMapSize;
//
//	static const char CHARAC_CHAR = 'O';
//	static const char BOSS_CHAR = 'B';
//	char** mapBoard = nullptr;
//	FloorTile** floorTiles = nullptr;
//
//
//	void deleteMapBoard() {
//		for (int i = 0; i < currentMapSize; i++) {
//			delete[] mapBoard[i];
//		}
//		delete[] mapBoard;//mapBoard memory
//	}
//
//	void deleteTiles() {
//		for (int i = 0; i < currentMapSize; i++) {
//			delete[] floorTiles[i];
//		}
//		delete[] floorTiles;
//	}
//};
//
//struct GameInfo {
//	void printGameInfo() {
//		cout << "w:up, s:down, a:left, d:right ESC:quit" << endl;
//	}
//};
//// 미구현
////struct ExperienceTable {
////	static const int MAX_LEVEL = 10;
////	int levelUpAmount[MAX_LEVEL - 1];
////
////	void initTable() {
////		int tmpNum = 10;
////		for (int i = 0; i < sizeof(levelUpAmount) / sizeof(int); i++) {
////			tmpNum += i * tmpNum;
////			levelUpAmount[i] = tmpNum;
////		}
////	}
////
////	//경험치 획득 후 경험치와 레벨 계산 시 활용
////	void updateExperiencePoint(GamePlayer* playerPtr) {
////		int expSum = 0;
////		for (int i = 0; i < sizeof(levelUpAmount) / sizeof(int); i++) {
////			if (playerPtr->experiencePoint >= levelUpAmount[i]) {
////				playerPtr->currentLevel = i + 2;//set player's level;
////			}
////			else {
////				if (i > 0)
////					playerPtr->experienceRate = (float)playerPtr->experiencePoint / levelUpAmount[i]-levelUpAmount[i-1];
////				else {
////					playerPtr->experienceRate = (float)playerPtr->experiencePoint / levelUpAmount[i];
////				}
////				break;
////			}
////		}
////	}
////};
//
//struct GamePlayer {
//	string playerName = "";
//	int maxHp = 100;
//	int currentHp = maxHp;
//	int experiencePoint = 0;
//	float experienceRate = 0.0f;
//	int currentLevel = 1;
//
//	static const int INIT_MONEY = 100;
//	int playerMoney = INIT_MONEY;
//	int playerX = 0;
//	int playerY = 0;
//
//};
//
//struct MoveTo {
//	static const int UP_WARD = 0;
//	static const int LEFT_WARD = 1;
//	static const int DOWN_WARD = 2;
//	static const int RIGHT_WARD = 3;
//};
//
//
//
//struct RPSGame {
//	static const int SCISSORS_NUM = 0;
//	static const int ROCK_NUM = 1;
//	static const int PAPER_NUM = 2;
//};
//
//struct GameStage {
//	static const int INIT_STAGE = 1;
//	static const int MAX_STAGE = 5;
//	int currentStage;
//};
//
////=====About HealerShop=====
//struct RecoveryPortion {
//	string portionName;
//	int portionPrice;
//	int recoveryAmount;
//};
//
//struct HealerShop {
//	static const int MAX_HEAL = 99999999;
//	static const int HEAL_10 = 10;
//	static const int HEAL_20 = 20;
//	static const int HEAL_30 = 30;
//
//	RecoveryPortion* hp10Portion = nullptr;
//	int hp10PortionCnt = 0;
//
//	RecoveryPortion* hp20Portion = nullptr;
//	int hp20PortionCnt = 0;
//
//	RecoveryPortion* hp30Portion = nullptr;
//	int hp30PortionCnt = 0;
//
//	RecoveryPortion* fullRecoveryPortion = nullptr;
//	int fullRecoveryPortionCnt = 0;
//
//	static const int MAX_HEAL_PRICE = 50;
//	static const int HEAL_10_PRICE = 10;
//	static const int HEAL_20_PRICE = 20;
//	static const int HEAL_30_PRICE = 30;
//};
//
////Etc function
//struct SleepTime {
//	static const int RPS_COUNTDOWN = 300;
//};
//
//
////======================================= 함 수 ================================================
//void registerPlayer(GamePlayer& gamePlayer) {
//	cout << "Your Name: ";
//	cin >> gamePlayer.playerName;
//	gamePlayer.playerMoney = GamePlayer::INIT_MONEY;
//	gamePlayer.playerX = 0;
//	gamePlayer.playerY = 0;
//}
//
//void setMapSize(GameMap& gameMap, int difficultyLevel) {
//	switch (difficultyLevel) {
//	case BossMonster::BEGINNER_LEVEL:
//		gameMap.currentMapSize = GameMap::BEGINNER_MAP_SIZE;
//		break;
//	case BossMonster::INTERMIDIATE_LEVEL:
//		gameMap.currentMapSize = GameMap::INTERMIDIATE_MAP_SIZE;
//		break;
//	case BossMonster::PROFESSIONAL_LEVEL:
//		gameMap.currentMapSize = GameMap::PROFESSIONAL_MAP_SIZE;
//		break;
//	}
//}
//void setBossCnt(BossGroup& bossGroup, int difficultyLevel) {
//	switch (difficultyLevel) {
//	case BossMonster::BEGINNER_LEVEL:
//		bossGroup.currentBossCnt = BossGroup::BEGINNER_BOSS_CNT;
//		break;
//	case BossMonster::INTERMIDIATE_LEVEL:
//		bossGroup.currentBossCnt = BossGroup::INTERMIDIATE_BOSS_CNT;
//		break;
//	case BossMonster::PROFESSIONAL_LEVEL:
//		bossGroup.currentBossCnt = BossGroup::PROFESSIONAL_BOSS_CNT;
//		break;
//	}
//}
//void initMap(GameMap& gameMap) {
//	//Heap Memory allocation for mapBoard
//	gameMap.mapBoard = new char* [gameMap.currentMapSize];
//	for (int i = 0; i < gameMap.currentMapSize; i++) {
//		gameMap.mapBoard[i] = new char[gameMap.currentMapSize];
//	}
//}
//void createBossMon(BossGroup& bossGroup, int difficultyLevel, int currentStage) {
//	bossGroup.bossMon = new BossMonster[bossGroup.currentBossCnt];
//	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//		//current stage level ~ (0 ~ difficultyLevel-1)+current stage level - 1
//		bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + currentStage);
//	}
//}
//void arrangeBoss(BossGroup& bossGroup, GameMap& gameMap) {
//	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//		bossGroup.bossMon[i].bossX = rand() % gameMap.currentMapSize;
//		bossGroup.bossMon[i].bossY = rand() % gameMap.currentMapSize;
//		if (bossGroup.bossMon[i].bossX == 0 && bossGroup.bossMon[i].bossY == 0) {//When it's the start point
//			i--;//repeat!!
//		}
//		else if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] == GameMap::BOSS_CHAR) {//When there is another boss monster on the point
//			i--;
//		}
//		else {
//			gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//		}
//	}
//}
//void printStageInfo(GameStage& gameStage) {
//	cout << "########################" << endl;
//	cout << "STAGE " << gameStage.currentStage << endl;
//}
//void printPlayerInfo(GamePlayer& gamePlayer) {
//	//Print the player information
//	cout << "########################" << endl;
//	cout << "NAME  : " << gamePlayer.playerName << endl;
//	cout << "LV    : " << gamePlayer.currentLevel << "(" << gamePlayer.experienceRate << "%)" << endl;
//	cout << "HP    : " << gamePlayer.currentHp << "/" << gamePlayer.maxHp << endl;
//	cout << "MONEY : " << gamePlayer.playerMoney << endl;
//	cout << "########################" << endl << endl;
//}
//void arrangeMap(GameMap& gameMap) {
//	for (int i = 0; i < gameMap.currentMapSize; i++) {//Cover the whole map with grass
//		for (int j = 0; j < gameMap.currentMapSize; j++) {
//			gameMap.mapBoard[i][j] = gameMap.floorTiles[i][j].tileShape;
//		}
//	}
//}
//void updateBossLocation(const BossGroup& bossGroup, GameMap& gameMap) {
//	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//		if (bossGroup.bossMon[i].isAlive)
//			gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //보스몬스터 깔기
//	}
//}
//void updatePlayerLocation(const GamePlayer& gamePlayer, GameMap& gameMap) {
//	gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::CHARAC_CHAR;
//}
//void printMap(GameMap& gameMap) {
//	for (int i = 0; i < gameMap.currentMapSize; i++) {
//		for (int j = 0; j < gameMap.currentMapSize; j++) {
//			cout << gameMap.mapBoard[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//bool moveCharacter(int currentMapSize, GamePlayer& gamePlayer) {
//	bool keepPlaying = true;
//	char userInput = _getch();
//
//	switch (userInput) {
//	case 'w':case 'W': if (gamePlayer.playerY != 0) gamePlayer.playerY -= 1;
//		break;
//	case 'a':case 'A': if (gamePlayer.playerX != 0) gamePlayer.playerX -= 1;
//		break;
//	case 's':case 'S': if (gamePlayer.playerY < currentMapSize - 1) gamePlayer.playerY += 1;
//		break;
//	case 'd':case 'D': if (gamePlayer.playerX < currentMapSize - 1) gamePlayer.playerX += 1;
//		break;
//	case 27: cout << "Good Bye..." << endl;
//		keepPlaying = false;
//		break;
//	}
//
//	return keepPlaying;
//}
//void moveBossMon(GameMap& gameMap, BossGroup& bossGroup) {
//	if ((bossGroup.bossMon->current_wait_time += 1) == BossMonster::TOTAL_WAIT_TIME) {
//		bossGroup.bossMon->current_wait_time = 0;
//
//		for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//			if (bossGroup.bossMon[i].isAlive) {
//				int whereTo = rand() % 4;
//				switch (whereTo) {
//				case MoveTo::UP_WARD:
//					if (bossGroup.bossMon[i].bossY != 0) {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY - 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossY -= 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//					else {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY + 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossY++;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//				case MoveTo::LEFT_WARD:
//					if (bossGroup.bossMon[i].bossX != 0) {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX - 1] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossX -= 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//					else {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX + 1] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossX++;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//				case MoveTo::DOWN_WARD:
//					if (bossGroup.bossMon[i].bossY != gameMap.currentMapSize - 1) {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY + 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossY += 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//					else {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY - 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossY -= 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//				case MoveTo::RIGHT_WARD:
//					if (bossGroup.bossMon[i].bossX != gameMap.currentMapSize - 1) {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX + 1] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossX += 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//					else {
//						if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX - 1] != GameMap::BOSS_CHAR) {
//							bossGroup.bossMon[i].bossX -= 1;
//							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
//							break;
//						}
//					}
//					break;
//				}
//			}
//		}
//	}
//}
//
//void createFloorTiles(GameMap& gameMap) {
//	//floorTiles 동적할당
//	gameMap.floorTiles = new FloorTile * [gameMap.currentMapSize];
//	for (int i = 0; i < gameMap.currentMapSize; i++) {
//		gameMap.floorTiles[i] = new FloorTile[gameMap.currentMapSize];
//	}
//}
//
//void shuffleTiles(GameMap& gameMap) {//타일 랜덤 배분
//	for (int i = 0; i < gameMap.currentMapSize; i++) {
//		for (int j = 0; j < gameMap.currentMapSize; j++) {
//			gameMap.floorTiles[i][j].tileX = i;
//			gameMap.floorTiles[i][j].tileY = j;
//			gameMap.floorTiles[i][j].tileType = rand() % 3;
//			gameMap.floorTiles[i][j].setTileShape(gameMap.floorTiles[i][j].tileType);
//		}
//	}
//}
//
//void showHealerShop(GamePlayer& gamePlayer) {
//	char shopYn;
//	cout << "Do you want to visit the Healer's shop?(y/n)";
//	cin >> shopYn;
//	shopYn = tolower(shopYn);
//	if (shopYn == 'y') {
//		int userSelect = 0;
//		system("cls");
//		cout << "########################" << endl;
//		cout << "Money: " << gamePlayer.playerMoney << " / HP:" << gamePlayer.currentHp << endl;
//		cout << "########################" << endl;
//		cout << " 1. Full Recover($50)" << endl;
//		cout << " 2. +10($10)" << endl;
//		cout << " 3. +20($20)" << endl;
//		cout << " 4. +30($30)" << endl;
//		cout << "########################" << endl;
//		do {
//			cin >> userSelect;
//		} while (userSelect < 1 || userSelect > 4);
//
//		switch (userSelect) {
//		case 1: cout << "You HP has been fully recovered!!" << endl;
//			gamePlayer.currentHp = gamePlayer.maxHp;
//			gamePlayer.playerMoney -= HealerShop::MAX_HEAL_PRICE;
//			cout << "You spent $" << HealerShop::MAX_HEAL_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
//			break;
//		case 2: cout << "Your HP has been recovered by 10!!" << endl;
//			gamePlayer.currentHp += 10;
//			gamePlayer.playerMoney -= HealerShop::HEAL_10_PRICE;
//			cout << "You spent $" << HealerShop::HEAL_10_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
//			break;
//		case 3: cout << "Your HP has been recovered by 20!!" << endl;
//			gamePlayer.currentHp += 20;
//			gamePlayer.playerMoney -= HealerShop::HEAL_20_PRICE;
//			cout << "You spent $" << HealerShop::HEAL_20_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
//			break;
//		case 4: cout << "Your HP has been recovered by 30!!" << endl;
//			gamePlayer.currentHp += 30;
//			gamePlayer.playerMoney -= HealerShop::HEAL_30_PRICE;
//			cout << "You spent $" << HealerShop::HEAL_30_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
//			break;
//		}
//		if (gamePlayer.currentHp > gamePlayer.maxHp) {
//			gamePlayer.currentHp = gamePlayer.maxHp;
//		}
//		cout << "Input Any key To Keep Playing..." << endl; _getch();
//	}
//}
//void updateBossLocation(BossGroup& bossGroup, GameMap& gameMap) {
//	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//		if (bossGroup.bossMon[i].isAlive)//Position the bosses who are alive
//			gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //보스몬스터 깔기
//	}
//}
////=====================================  Main 함 수 ================================================
//int main() {
//	srand((unsigned)time(NULL));
//	GamePlayer gamePlayer;
//	BossGroup bossGroup;
//	GameStage gameStage;
//	GameMap gameMap;
//	bool keepPlaying = true;
//
//	//Player Register
//	registerPlayer(gamePlayer);
//
//	//Difficulty Setting
//	int difficultyLevel = BossMonster::BEGINNER_LEVEL;
//	do {
//		cout << "Difficulty(1~3): ";
//		cin >> difficultyLevel;
//	} while (difficultyLevel < BossMonster::BEGINNER_LEVEL || difficultyLevel > BossMonster::PROFESSIONAL_LEVEL);
//	gameStage.currentStage = GameStage::INIT_STAGE;
//
//	//Map size setting
//	setMapSize(gameMap, difficultyLevel);
//
//	//Boss count setting
//	setBossCnt(bossGroup, difficultyLevel);
//
//	//Heap Memory allocation for mapBoard
//	initMap(gameMap);
//
//	//Heap Memory allocation for floorTiles
//	createFloorTiles(gameMap);
//
//	//put tiles on the map
//	shuffleTiles(gameMap);
//
//	//Create Boss Monsters
//	createBossMon(bossGroup, difficultyLevel, gameStage.currentStage);
//
//	//Position the boss monsters on the map
//	arrangeBoss(bossGroup, gameMap);
//
//
//	while (keepPlaying) {
//
//		printStageInfo(gameStage);
//		printPlayerInfo(gamePlayer);
//
//		//Map Renewal
//		arrangeMap(gameMap);
//
//		//Update the boss location
//		updateBossLocation(bossGroup, gameMap);
//
//		//Update the player location
//		updatePlayerLocation(gamePlayer, gameMap);
//
//		//Print the map
//		printMap(gameMap);
//
//		//Character Move
//		keepPlaying = moveCharacter(gameMap.currentMapSize, gamePlayer);
//
//		if (!keepPlaying) {//switch 문에서 ESC를 누를 시 게임 종료
//			break;
//		}
//
//		//Boss Move
//		moveBossMon(gameMap, bossGroup);
//
//
//		//보스를 마주치는 경우 계산
//		bool isBoss = false;
//		int bossIdx = 0;
//		for (int i = 0; i < bossGroup.currentBossCnt; i++) {//보스들 중 하나와 마주쳤는지 확인
//			if (bossGroup.bossMon[i].isAlive && bossGroup.bossMon[i].bossX == gamePlayer.playerX && bossGroup.bossMon[i].bossY == gamePlayer.playerY) {
//				isBoss = true;
//				bossIdx = i;//몇 번째 보스인지 저장
//			}
//		}
//
//		//보스와 마추쳤을 때
//		if (isBoss) {
//			system("cls");
//			cout << "Boss Name : " << bossGroup.bossMon[bossIdx].monsterName << endl;
//			while (true) {
//
//				//가위 바위 보
//				int userResult = 0;
//				int comResult = rand() % 3;
//
//				cout << "SCISSORS(0) ROCK(1) PAPER(2): ";
//				cin >> userResult;
//
//				//카운트 다운
//				for (int i = 3; i > 0; i--) {
//					cout << i << " ";
//					Sleep(SleepTime::RPS_COUNTDOWN);
//				}
//				cout << endl;
//
//				cout << "[" << gamePlayer.playerName << " : " << (userResult == RPSGame::SCISSORS_NUM ? "SCISSORS" : ((userResult == RPSGame::ROCK_NUM) ? "ROCK" : "PAPER")) << "] [";
//				cout << bossGroup.bossMon[bossIdx].monsterName << " : " << (comResult == RPSGame::SCISSORS_NUM ? "SCISSORS" : ((comResult == RPSGame::ROCK_NUM) ? "ROCK" : "PAPER")) << "]" << endl;
//
//				if (comResult == userResult) {
//					//Draw
//					continue;
//				}
//				else if (userResult - comResult == 1 || userResult - comResult == -2) {
//					//User Win
//					gamePlayer.playerMoney += bossGroup.bossMon[bossIdx].treatMoney;//유저 돈 획득
//					bossGroup.bossMon[bossIdx].isAlive = false;
//					cout << endl << gamePlayer.playerName << " Won!!" << endl;
//					cout << "You got $" << bossGroup.bossMon[bossIdx].treatMoney << ", Total Money: $" << gamePlayer.playerMoney << endl;
//
//					if (gamePlayer.experiencePoint / 100 < (gamePlayer.experiencePoint + bossGroup.bossMon[bossIdx].treatEXP) / 100) {
//						gamePlayer.currentLevel++;
//						gamePlayer.experiencePoint += bossGroup.bossMon[bossIdx].treatEXP;//유저 경험치 획득
//						cout << "You got " << bossGroup.bossMon[bossIdx].treatEXP << "EXP" << endl;
//						cout << "LEVEL UP!! " << gamePlayer.currentLevel - 1 << " > " << gamePlayer.currentLevel << endl;
//						gamePlayer.maxHp += 20;
//						gamePlayer.currentHp = gamePlayer.maxHp;
//					}
//					else {
//						gamePlayer.experiencePoint += bossGroup.bossMon[bossIdx].treatEXP;//유저 경험치 획득
//						cout << "You got " << bossGroup.bossMon[bossIdx].treatEXP << "EXP" << endl;
//					}
//					gamePlayer.experienceRate = gamePlayer.experiencePoint % 100;
//
//					//모든 보스가 죽었을 시 다음 스테이지로
//					bool hasCleared = true;
//					for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//						if (bossGroup.bossMon[i].isAlive) {
//							hasCleared = false;
//						}
//					}
//					if (hasCleared) {//이번 스테이지를 클리어 했으면
//						gameStage.currentStage += 1;
//						if (gameStage.currentStage > gameStage.MAX_STAGE) {//모든 스테이지를 클리어 했으면
//							cout << "Congratuation, You've cleared all stages!!" << endl;
//							keepPlaying = false;
//							break;
//						}
//						//캐릭터, 보스몹 및 맵 초기화
//						//캐릭터 위치 초기화
//						gamePlayer.playerX = 0;
//						gamePlayer.playerY = 0;
//
//						//보스 몬스터 위치 초기화
//						for (int i = 0; i < bossGroup.currentBossCnt; i++) {
//							//현재 스테이지 ~ (0~현재 레벨-1)+stage 까지의 보스몬스터 출현
//							bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + gameStage.currentStage);
//							bossGroup.bossMon[i].isAlive = true; //보스몹 부활!!
//						}
//
//						//Map Renewal
//						shuffleTiles(gameMap);
//
//						//Position the boss monsters on the map
//						arrangeBoss(bossGroup, gameMap);
//
//					}
//					break;
//				}
//				else {
//					//유저가 졌을 경우
//					gamePlayer.currentHp -= bossGroup.bossMon[bossIdx].monsterDamage;
//					gamePlayer.playerX = 0; gamePlayer.playerY = 0;//플레이어 초기위치로 돌아가기
//					cout << gamePlayer.playerName << " Lost!!" << endl;
//					if (gamePlayer.currentHp <= 0) {
//						cout << "GAME OVER" << endl;
//						keepPlaying = false;
//					}
//					break;
//				}
//			}
//			if (!keepPlaying)
//				break;
//
//			showHealerShop(gamePlayer);
//
//
//			//Update the boss locations(if the player lose, the boss will stay here)
//			updateBossLocation(bossGroup, gameMap);
//		}
//		system("cls");
//	}
//
//	//Delete allocated heap memory
//	gameMap.deleteTiles();
//	gameMap.deleteMapBoard();
//	bossGroup.deleteBossMon();
//
//	return 0;
//}