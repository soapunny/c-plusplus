#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
#include <Windows.h>
using namespace std;

//�ǽ�
//������ ������
// [I made this game without functions and classes]
// 1) �ؽ�Ʈ�� ����Ǵ� ����
// 2) ������ ���۵Ǹ� ������ �̸��� ���̵��� �Է�
// 3) ���̵��� ���� ������ HP/ ������ �Ǵ� ������ ���ڰ� ����
// 4) ���͸� ������ �����ϰ� ���� ȹ���Ѵ�.(0~100)
// 5) ���͸� ������ ������ ��� Ž������ ������ �鸦�� �����Ѵ�.
// 6) ������ �鸣�� ���� �Ҹ��ؼ� HP�� ȸ���� �� �ִ�.
// 6_1) ��üȸ��, +10, +20, +30 ����ȸ��
// 7) HP�� 0�� �Ǹ� ���ӿ���
// 8) ���͸� ��� ������ ����Ŭ����

// 0) ���������� ������
// 0) ����������(���� ���а� ������ ������ �ݺ�) - ����
// 0_1) ���� ���� ���� HP�� �پ���.
// 0_2) ���ʹ� �ѹ� ���� �ٷ� ����

//#####v2 ������Ʈ#####
//1. ���̵��� ���� ���� ũ�Ⱑ ����ȴ�.
//2. ��簡 �� �� �̵��� ��, ���� �� �� �̵��Ѵ�.

struct GameMap {
	static const int BEGINNER_MAP_SIZE = 10;
	static const int INTERMIDIATE_MAP_SIZE = 15;
	static const int PROFESSIONAL_MAP_SIZE = 20;
	int currentMapSize;

	static const char GRASS_CHAR = '#';
	static const char CHARAC_CHAR = 'O';
	static const char BOSS_CHAR = 'B';

	char** mapBoard = nullptr;
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

struct MoveTo {
	static const int UP_WARD = 0;
	static const int LEFT_WARD = 1;
	static const int DOWN_WARD = 2;
	static const int RIGHT_WARD = 3;
};

struct BossMonster {
	static const int BEGINNER_LEVEL = 1;
	static const int INTERMIDIATE_LEVEL = 2;
	static const int PROFESSIONAL_LEVEL = 3;
	static const int TOTAL_WAIT_TIME = 2;

	int current_wait_time = 0;
	string monsterName;
	int monsterLevel = 1;
	int monsterDamage;
	int bossX;
	int bossY;
	bool isAlive = true;

	void setDifficulty(int monsterLevel) {
		if (monsterLevel < BEGINNER_LEVEL || monsterLevel > PROFESSIONAL_LEVEL + 5) {
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
	static const int BEGINNER_BOSS_CNT = 3;
	static const int INTERMIDIATE_BOSS_CNT = 5;
	static const int PROFESSIONAL_BOSS_CNT = 7;

	int currentBossCnt;
	BossMonster* bossMon = nullptr;
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
	int difficultyLevel = BossMonster::BEGINNER_LEVEL;
	do {
		cout << "Difficulty(1~3): ";
		cin >> difficultyLevel;
	} while (difficultyLevel < BossMonster::BEGINNER_LEVEL || difficultyLevel > BossMonster::PROFESSIONAL_LEVEL);
	gameStage.currentStage = GameStage::INIT_STAGE;

	//Map size setting
	switch (difficultyLevel) {
	case BossMonster::BEGINNER_LEVEL:
		gameMap.currentMapSize = GameMap::BEGINNER_MAP_SIZE;
		bossGroup.currentBossCnt = BossGroup::BEGINNER_BOSS_CNT;
		break;
	case BossMonster::INTERMIDIATE_LEVEL:
		gameMap.currentMapSize = GameMap::INTERMIDIATE_MAP_SIZE;
		bossGroup.currentBossCnt = BossGroup::INTERMIDIATE_BOSS_CNT;
		break;
	case BossMonster::PROFESSIONAL_LEVEL:
		gameMap.currentMapSize = GameMap::PROFESSIONAL_MAP_SIZE;
		bossGroup.currentBossCnt = BossGroup::PROFESSIONAL_BOSS_CNT;
		break;
	}

	//Heap Memory allocation for mapBoard
	gameMap.mapBoard = new char* [gameMap.currentMapSize];
	for (int i = 0; i < gameMap.currentMapSize; i++) {
		gameMap.mapBoard[i] = new char[gameMap.currentMapSize];
	}


	//Create Boss Monsters
	bossGroup.bossMon = new BossMonster[bossGroup.currentBossCnt];
	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
		//current stage level ~ (0 ~ difficultyLevel-1)+current stage level - 1
		bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + gameStage.currentStage);
	}

	//Position the boss monsters on the map
	for (int i = 0; i < bossGroup.currentBossCnt; i++) {
		bossGroup.bossMon[i].bossX = rand() % gameMap.currentMapSize;
		bossGroup.bossMon[i].bossY = rand() % gameMap.currentMapSize;
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

		//Map Renewal
		for (int i = 0; i < gameMap.currentMapSize; i++) {//Cover the whole map with grass
			for (int j = 0; j < gameMap.currentMapSize; j++) {
				gameMap.mapBoard[i][j] = GameMap::GRASS_CHAR;
			}
		}

		//Update the boss location
		for (int i = 0; i < bossGroup.currentBossCnt; i++) {
			if(bossGroup.bossMon[i].isAlive)
				gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //�������� ���
		}

		//Update the player location
		gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::CHARAC_CHAR;

		//Print the map
		for (int i = 0; i < gameMap.currentMapSize; i++) {
			for (int j = 0; j < gameMap.currentMapSize; j++) {
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
			if (gamePlayer.playerY < gameMap.currentMapSize - 1) gamePlayer.playerY += 1;
			break;
		case 'd':case 'D': gameMap.mapBoard[gamePlayer.playerY][gamePlayer.playerX] = GameMap::GRASS_CHAR;
			if (gamePlayer.playerX < gameMap.currentMapSize - 1) gamePlayer.playerX += 1;
			break;
		case 27: cout << "Good Bye..." << endl;
			keepPlaying = false;
			break;
		}

		if (!keepPlaying) {//switch ������ ESC�� ���� �� ���� ����
			break;
		}

		//Boss Move
		if ((bossGroup.bossMon->current_wait_time += 1) == BossMonster::TOTAL_WAIT_TIME) {
			bossGroup.bossMon->current_wait_time = 0;

			for (int i = 0; i < bossGroup.currentBossCnt; i++) {
				if (bossGroup.bossMon[i].isAlive) {
					int whereTo = rand() % 4;
					switch (whereTo) {
					case MoveTo::UP_WARD:
						if (bossGroup.bossMon[i].bossY != 0) {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY - 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossY -= 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
						else {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY + 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossY++;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
					case MoveTo::LEFT_WARD:
						if (bossGroup.bossMon[i].bossX != 0) {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX - 1] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossX -= 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
						else {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX + 1] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossX++;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
					case MoveTo::DOWN_WARD:
						if (bossGroup.bossMon[i].bossY != gameMap.currentMapSize - 1) {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY + 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossY += 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
						else {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY - 1][bossGroup.bossMon[i].bossX] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossY -= 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
					case MoveTo::RIGHT_WARD:
						if (bossGroup.bossMon[i].bossX != gameMap.currentMapSize - 1) {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX + 1] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossX += 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
						else {
							if (gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX - 1] != GameMap::BOSS_CHAR) {
								bossGroup.bossMon[i].bossX -= 1;
								gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR;
								break;
							}
						}
						break;
					}
				}
			}
		}

		//������ ����ġ�� ��� ���
		bool isBoss = false;
		int bossIdx = 0;
		for (int i = 0; i < bossGroup.currentBossCnt; i++) {//������ �� �ϳ��� �����ƴ��� Ȯ��
			if (bossGroup.bossMon[i].isAlive && bossGroup.bossMon[i].bossX == gamePlayer.playerX && bossGroup.bossMon[i].bossY == gamePlayer.playerY) {
				isBoss = true;
				bossIdx = i;//�� ��° �������� ����
			}
		}

		//������ �������� ��
		if (isBoss) {
			system("cls");
			cout << "Boss Name : " << bossGroup.bossMon[bossIdx].monsterName << endl;
			while (true) {

				//���� ���� ��
				int userResult = 0;
				int comResult = rand() % 3;

				cout << "SCISSORS(0) ROCK(1) PAPER(2): ";
				cin >> userResult;

				//ī��Ʈ �ٿ�
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
					gamePlayer.playerMoney += treatMoney;//���� �� ȹ��
					bossGroup.bossMon[bossIdx].isAlive = false;
					cout << endl << gamePlayer.playerName << " Won!!" << endl;
					cout << "You got $" << treatMoney << ", Total Money: $" << gamePlayer.playerMoney << endl;

					//��� ������ �׾��� �� ���� ����������
					bool hasCleared = true;
					for (int i = 0; i < bossGroup.currentBossCnt; i++) {
						if (bossGroup.bossMon[i].isAlive) {
							hasCleared = false;
						}
					}
					if (hasCleared) {//�̹� ���������� Ŭ���� ������
						gameStage.currentStage += 1;
						if (gameStage.currentStage > gameStage.MAX_STAGE) {//��� ���������� Ŭ���� ������
							cout << "Congratuation, You've cleared all stages!!" << endl;
							keepPlaying = false;
							break;
						}
						//ĳ����, ������ �� �� �ʱ�ȭ
						//ĳ���� ��ġ �ʱ�ȭ
						gamePlayer.playerX = 0;
						gamePlayer.playerY = 0;

						//���� ���� ��ġ �ʱ�ȭ
						for (int i = 0; i < bossGroup.currentBossCnt; i++) {
							//���� �������� ~ (0~���� ����-1)+stage ������ �������� ����
							bossGroup.bossMon[i].setDifficulty(rand() % difficultyLevel + gameStage.currentStage);
							bossGroup.bossMon[i].isAlive = true; //������ ��Ȱ!!
						}

						//Map Renewal
						for (int i = 0; i < gameMap.currentMapSize; i++) {//Cover the whole map with grass
							for (int j = 0; j < gameMap.currentMapSize; j++) {
								gameMap.mapBoard[i][j] = GameMap::GRASS_CHAR;
							}
						}

						//Position the boss monsters on the map
						for (int i = 0; i < bossGroup.currentBossCnt; i++) {
							bossGroup.bossMon[i].bossX = rand() % gameMap.currentMapSize;
							bossGroup.bossMon[i].bossY = rand() % gameMap.currentMapSize;
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

						for (int i = 0; i < bossGroup.currentBossCnt; i++) {
							gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //�������� ���
						}

					}
					break;
				}
				else {
					//������ ���� ���
					gamePlayer.playerHP -= bossGroup.bossMon[bossIdx].monsterDamage;
					gamePlayer.playerX = 0; gamePlayer.playerY = 0;//�÷��̾� �ʱ���ġ�� ���ư���
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
			for (int i = 0; i < bossGroup.currentBossCnt; i++) {
				if (bossGroup.bossMon[i].isAlive)//Position the bosses who are alive
					gameMap.mapBoard[bossGroup.bossMon[i].bossY][bossGroup.bossMon[i].bossX] = GameMap::BOSS_CHAR; //�������� ���
			}
		}
		system("cls");
	}

	//Delete allocated heap memory
	for (int i = 0; i < gameMap.currentMapSize; i++) {
		delete[] gameMap.mapBoard[i];
	}
	delete[] gameMap.mapBoard;//mapBoard memory
	delete[] bossGroup.bossMon;//bossMon memory

	return 0;
}