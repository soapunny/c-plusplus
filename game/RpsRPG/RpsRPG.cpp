#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
#include <Windows.h>
using namespace std;

//실습
//영웅은 절차적
// [I made this game without functions and classes]
// 1) 텍스트로 진행되는 게임
// 2) 게임이 시작되면 영웅의 이름과 난이도를 입력
// 3) 난이도에 따라서 영웅의 HP/ 만나게 되는 몬스터의 숫자가 변동
// 4) 몬스터를 잡으면 랜덤하게 돈을 획득한다.(0~100)
// 5) 몬스터를 잡으면 던전을 계속 탐험할지 상점을 들를지 결정한다.
// 6) 상점에 들르면 돈을 소모해서 HP를 회복할 수 있다.
// 6_1) 전체회복, +10, +20, +30 차등회복
// 7) HP가 0이 되면 게임오버
// 8) 몬스터를 모두 잡으면 게임클리어

// 0) 가위바위보 순서도
// 0) 가위바위보(비기면 승패가 정해질 때까지 반복) - 선행
// 0_1) 졌을 때만 영웅 HP가 줄어든다.
// 0_2) 몬스터는 한번 지면 바로 죽음

struct GameMap {
	static const int MAP_SIZE = 15;
	static const char GRASS_CHAR = '#';
	static const char CHARAC_CHAR = 'O';
	static const char BOSS_CHAR = 'B';

	char mapBoard[GameMap::MAP_SIZE][GameMap::MAP_SIZE];
};

struct GamePlayer {
	static const int MAX_HP = 100;
	static const int INIT_MONEY = 100;
	string playerName = "";
	int playerHP = MAX_HP;
	int playerMoney = INIT_MONEY;
	int playerX = 0;
	int playerY = 0;
};

struct BossMonster {
	static const int MIN_LEVEL = 1;
	static const int MAX_LEVEL = 5;
	string monsterName;
	int monsterLevel = 1;
	int monsterDamage;
	int bossX;
	int bossY;
	bool isAlive = true;

	void setDifficulty(int monsterLevel) {
		if (monsterLevel < 1 || monsterLevel > MAX_LEVEL + 5) {
			cout << "Type a proper level(1~5)!!" << endl;
			return;
		}
		monsterDamage = monsterLevel * 10;
		monsterName = "BOSS" + to_string(monsterLevel);
		this->monsterLevel = monsterLevel;
	}
};

struct RPSGame {
	static const int SCISSORS_NUM = 0;
	static const int ROCK_NUM = 1;
	static const int PAPER_NUM = 2;
};

struct GameStage {
	static const int INIT_STAGE = 1;
	static const int MAX_STAGE = 5;
	int currentStage;
};

struct BossGroup {
	static const int BOSS_CNT = 3;
	BossMonster bossMon[BOSS_CNT];
};

struct HealerShop {
	static const int MAX_HEAL = 99999999;
	static const int HEAL_10 = 10;
	static const int HEAL_20 = 20;
	static const int HEAL_30 = 30;

	static const int MAX_HEAL_PRICE = 50;
	static const int HEAL_10_PRICE = 10;
	static const int HEAL_20_PRICE = 20;
	static const int HEAL_30_PRICE = 30;
};

struct SleepTime {
	static const int RPS_COUNTDOWN = 300;
};


int main() {
	srand((unsigned)time(NULL));
	GamePlayer gamePlayer;
	BossGroup bossGroup;
	GameStage gameStage;
	GameMap gameMap;
	bool keepPlaying = true;

	//Player Register
	cout << "Your Name: ";
	cin >> gamePlayer.playerName;
	gamePlayer.playerHP = GamePlayer::MAX_HP;
	gamePlayer.playerMoney = GamePlayer::INIT_MONEY;
	gamePlayer.playerX = 0;
	gamePlayer.playerY = 0;

	//Difficulty Setting
	int difficultyLevel = BossMonster::MIN_LEVEL;
	do {
		cout << "Difficulty(1~5): ";
		cin >> difficultyLevel;
	} while (difficultyLevel < BossMonster::MIN_LEVEL || difficultyLevel > BossMonster::MAX_LEVEL);
	gameStage.currentStage = GameStage::INIT_STAGE;

	//Create Boss Monsters
	for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
		//current stage level ~ (0 ~ difficultyLevel-1)+current stage level - 1
		bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + gameStage.currentStage);
	}

	//Map Renewal
	for (int i = 0; i < GameMap::MAP_SIZE; i++) {//Cover the whole map with grass
		for (int j = 0; j < GameMap::MAP_SIZE; j++) {
			gameMap.mapBoard[i][j] = GameMap::GRASS_CHAR;
		}
	}

	//Position the boss monsters on the map
	for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
		bossGroup.bossMon[i].bossX = rand() % GameMap::MAP_SIZE;
		bossGroup.bossMon[i].bossY = rand() % GameMap::MAP_SIZE;
		if (bossGroup.bossMon[i].bossX == 0 && bossGroup.bossMon[i].bossY == 0) {//When it's the start point
			i--;//repeat!!
		}
		else if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] == GameMap::BOSS_CHAR) {//When there is another boss monster on the point
			i--;
		}
		else {
			gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
		}
	}


	while (keepPlaying) {

		//Print the player information
		cout << "########################" << endl;
		cout << "STAGE " << gameStage.currentStage << endl;
		cout << "########################" << endl;
		cout << "NAME  : " << gamePlayer.playerName << endl;
		cout << "HP    : " << gamePlayer.playerHP << endl;
		cout << "MONEY : " << gamePlayer.playerMoney << endl;
		cout << "########################" << endl << endl;

		//Update the player location
		gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::CHARAC_CHAR;

		//Print the map
		for (int i = 0; i < GameMap::MAP_SIZE; i++) {
			for (int j = 0; j < GameMap::MAP_SIZE; j++) {
				cout << gameMap.mapBoard[i][j] << " ";
			}
			cout << endl;
		}

		//Character Move
		char userInput = _getch();
		switch (userInput) {
		case 'w':case 'W': gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::GRASS_CHAR; //Delete the last location of the player
			if (gamePlayer.playerY != 0) gamePlayer.playerY -= 1;
			break;
		case 'a':case 'A': gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::GRASS_CHAR;
			if (gamePlayer.playerX != 0) gamePlayer.playerX -= 1;
			break;
		case 's':case 'S': gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::GRASS_CHAR;
			if (gamePlayer.playerY < GameMap::MAP_SIZE - 1) gamePlayer.playerY += 1;
			break;
		case 'd':case 'D': gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::GRASS_CHAR;
			if (gamePlayer.playerX < GameMap::MAP_SIZE - 1) gamePlayer.playerX += 1;
			break;
		case 27: cout << "Good Bye..." << endl;
			keepPlaying = false;
			break;
		}

		if (!keepPlaying) {//switch 문에서 ESC를 누를 시 게임 종료
			break;
		}

		//보스를 마주치는 경우 계산
		bool isBoss = false;
		int bossIdx = 0;
		for (int i = 0; i < BossGroup::BOSS_CNT; i++) {//보스들 중 하나와 마주쳤는지 확인
			if (bossGroup.bossMon[i].isAlive && bossGroup.bossMon[i].bossX == gamePlayer.playerX && bossGroup.bossMon[i].bossY == gamePlayer.playerY) {
				isBoss = true;
				bossIdx = i;//몇 번째 보스인지 저장
			}
		}

		//보스와 마추쳤을 때
		if (isBoss) {
			system("cls");
			cout << "Boss Name : " << bossGroup.bossMon[bossIdx].monsterName << endl;
			while (true) {

				//가위 바위 보
				int userResult = 0;
				int comResult = rand() % 3;

				cout << "SCISSORS(0) ROCK(1) PAPER(2): ";
				cin >> userResult;

				//카운트 다운
				for (int i = 3; i > 0; i--) {
					cout << i << " ";
					Sleep(SleepTime::RPS_COUNTDOWN);
				}
				cout << endl;

				cout << "[" << gamePlayer.playerName << " : " << (userResult == RPSGame::SCISSORS_NUM ? "SCISSORS" : ((userResult == RPSGame::ROCK_NUM) ? "ROCK" : "PAPER")) << "] [";
				cout << bossGroup.bossMon[bossIdx].monsterName << " : " << (comResult == RPSGame::SCISSORS_NUM ? "SCISSORS" : ((comResult == RPSGame::ROCK_NUM) ? "ROCK" : "PAPER")) << "]" << endl;

				if (comResult == userResult) {
					//Draw
					continue;
				}
				else if (userResult - comResult == 1 || userResult - comResult == -2) {
					//User Win
					int treatMoney = rand() % 101;
					gamePlayer.playerMoney += treatMoney;//유저 돈 획득
					bossGroup.bossMon[bossIdx].isAlive = false;
					cout << endl << gamePlayer.playerName << " Won!!" << endl;
					cout << "You got $" << treatMoney << ", Total Money: $" << gamePlayer.playerMoney << endl;

					//모든 보스가 죽었을 시 다음 스테이지로
					bool hasCleared = true;
					for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
						if (bossGroup.bossMon[i].isAlive) {
							hasCleared = false;
						}
					}
					if (hasCleared) {//이번 스테이지를 클리어 했으면
						gameStage.currentStage += 1;
						if (gameStage.currentStage > gameStage.MAX_STAGE) {//모든 스테이지를 클리어 했으면
							cout << "Congratuation, You've cleared all stages!!" << endl;
							keepPlaying = false;
							break;
						}
						//캐릭터, 보스몹 및 맵 초기화
						//캐릭터 위치 초기화
						gamePlayer.playerX = 0;
						gamePlayer.playerY = 0;

						//보스 몬스터 위치 초기화
						for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
							//현재 스테이지 ~ (0~현재 레벨-1)+stage 까지의 보스몬스터 출현
							bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + gameStage.currentStage);
							bossGroup.bossMon[i].isAlive = true; //보스몹 부활!!
						}

						//Map Renewal
						for (int i = 0; i < GameMap::MAP_SIZE; i++) {//Cover the whole map with grass
							for (int j = 0; j < GameMap::MAP_SIZE; j++) {
								gameMap.mapBoard[i][j] = GameMap::GRASS_CHAR;
							}
						}

						//Position the boss monsters on the map
						for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
							bossGroup.bossMon[i].bossX = rand() % GameMap::MAP_SIZE;
							bossGroup.bossMon[i].bossY = rand() % GameMap::MAP_SIZE;
							if (bossGroup.bossMon[i].bossX == 0 && bossGroup.bossMon[i].bossY == 0) {//When it's the start point
								i--;//repeat!!
							}
							else if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] == GameMap::BOSS_CHAR) {//When there is another boss monster on the point
								i--;
							}
							else {
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
							}
						}

						for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //보스몬스터 깔기
						}

					}
					break;
				}
				else {
					//유저가 졌을 경우
					gamePlayer.playerHP -= bossGroup.bossMon[bossIdx].monsterDamage;
					gamePlayer.playerX = 0; gamePlayer.playerY = 0;//플레이어 초기위치로 돌아가기
					cout << gamePlayer.playerName << " Lost!!" << endl;
					if (gamePlayer.playerHP <= 0) {
						cout << "GAME OVER" << endl;
						keepPlaying = false;
					}
					break;
				}
			}
			if (!keepPlaying)
				break;
			char shopYn;
			cout << "Do you want to visit the Healer's shop?(y/n)";
			cin >> shopYn;
			shopYn = tolower(shopYn);
			if (shopYn == 'y') {
				int userSelect = 0;
				system("cls");
				cout << "########################" << endl;
				cout << "Money: " << gamePlayer.playerMoney << " / HP:" << gamePlayer.playerHP << endl;
				cout << "########################" << endl;
				cout << " 1. Full Recover($50)" << endl;
				cout << " 2. +10($10)" << endl;
				cout << " 3. +20($20)" << endl;
				cout << " 4. +30($30)" << endl;
				cout << "########################" << endl;
				do {
					cin >> userSelect;
				} while (userSelect < 1 || userSelect > 4);

				switch (userSelect) {
				case 1: cout << "You HP has been fully recovered!!" << endl;
					gamePlayer.playerHP = GamePlayer::MAX_HP;
					gamePlayer.playerMoney -= HealerShop::MAX_HEAL_PRICE;
					cout << "You spent $" << HealerShop::MAX_HEAL_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
					break;
				case 2: cout << "Your HP has been recovered by 10!!" << endl;
					gamePlayer.playerHP += 10;
					gamePlayer.playerMoney -= HealerShop::HEAL_10_PRICE;
					cout << "You spent $" << HealerShop::HEAL_10_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
					break;
				case 3: cout << "Your HP has been recovered by 20!!" << endl;
					gamePlayer.playerHP += 20;
					gamePlayer.playerMoney -= HealerShop::HEAL_20_PRICE;
					cout << "You spent $" << HealerShop::HEAL_20_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
					break;
				case 4: cout << "Your HP has been recovered by 30!!" << endl;
					gamePlayer.playerHP += 30;
					gamePlayer.playerMoney -= HealerShop::HEAL_30_PRICE;
					cout << "You spent $" << HealerShop::HEAL_30_PRICE << ", Your Money: $" << gamePlayer.playerMoney << endl;
					break;
				}
				if (gamePlayer.playerHP > GamePlayer::MAX_HP) {
					gamePlayer.playerHP = GamePlayer::MAX_HP;
				}
				cout << "Input Any key To Keep Playing..." << endl; _getch();
			}

			//Update the boss locations(if the player lose, the boss will stay here)
			for (int i = 0; i < BossGroup::BOSS_CNT; i++) {
				if (bossGroup.bossMon[i].isAlive)//Position the bosses who are alive
					gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //보스몬스터 깔기
			}
		}

		system("cls");

	}


	return 0;
}