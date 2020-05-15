/* Игра, в которой нужно сражаться с монстрами. Цель игры — собрать максимум золота, 
прежде чем умереть или достигнуть 20 уровня.*/

#include <iostream>
#include <string>    //для std::string
#include <array>     //для std::array
#include <cstdlib>   //для rand() и srand()
#include <ctime>     //для time()
#include <Windows.h> //для кириллицы

using std::cout; using std::cin;

// класс Существо - родительский класс для монстров и пользователей
class Creature
{
protected:
	std::string m_name;  // имя
	char m_symbol;       // символ
	int m_health;        // количество здоровья
	int m_force;         // сила - количество урона, которое можно нанести за 1 раз
	int m_gold;          // количество золота

public:
	// конструктор класса
	Creature(std::string name = "some creature", char symbol = 'x', int health = 0, int force = 0, int gold = 0) :
		m_name(name), m_symbol(symbol), m_health(health), m_force(force), m_gold(gold)
	{
	}

	// набор геттеров по одному на каждую переменную-член
	std::string getName() const
	{
		return m_name;
	}
	char getSymbol() const
	{
		return m_symbol;
	}
	int getHealth() const
	{
		return m_health;
	}
	int getForce() const
	{
		return m_force;
	}
	int getGold() const
	{
		return m_gold;
	}

	// метод для уменьшения здоровья Creature на указанное целочисленное значение
	void reduceHealth(int attack)
	{
		m_health -= attack;
	}
	// возвращает true, если здоровье Creature равно 0 или меньше
	bool isDead()
	{
		return (m_health <= 0);
	}
	// добавляет золото Creature-у
	void addGold(int salary)
	{
		m_gold += salary;
	}
};

// класс Player, который наследует Creature
class Player : public Creature
{
private:
	unsigned short m_level;                       // уровень
	static const unsigned short level_limit = 20; // максимальный уровень
public:
	// конструктор класса
	Player(std::string name = "Player's name", char symbol = '@', int health = 10, int force = 1, int gold = 0, unsigned short level = 1):
		Creature(name, symbol, health, force, gold), m_level(level)
	{}
	// метод, который увеличивает уровень Player-а, его здоровье и его урон на 1
	void levelUp()
	{
		++m_level;
		++m_force;
		++m_health;
	}
	// геттер для члена level
	unsigned short getLevel()
	{
		return m_level;
	}
	// возвращает true, если Player достиг 20 уровня
	bool hasWon()
	{
		return (m_level >= level_limit);
	}
};

// возвращает имя пользователя, спрашивая его
const std::string invitation()
{
	cout << "Представьтесь, пожалуйста, как к Вам обращаться: ";
	std::string users_name = "Вася";
	std::getline(cin, users_name);

	return users_name;
}

// функция генерирует рандомное число между min и max по списанному алгоритму
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем генерацию значения из диапазона
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

// класс Monster также наследует Creature и у него нет собственных переменных-членов. 
class Monster : public Creature
{
public:
	// перечисление Type, которое содержит 3 перечислителя, которые обозначают типы монстров : DRAGON, ORC и SLIME
	enum Type
	{
		DRAGON, ORC, SLIME /*слизь, лизун*/, MAX_TYPES
	};

	// структура данных монстра
	struct MonsterData
	{
		std::string name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	// массив из данных для монстров (общий для класса), инициализируется вне класса
	static MonsterData monsterData[MAX_TYPES];

	// конструктор класса, создающий монстра по его типу. По умолчанию - лизун
	Monster(Type type = SLIME) :
		Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{}
	
	//метод генерирует случайное число от 0 до MAX_TYPES-1 и возвращает определённый тип монстра 
	static Type getRandomMonster()
	{
		return static_cast<Type>(getRandomNumber(0, MAX_TYPES - 1));
	}
};

// определение таблицы с данными для монстров. Делаем вне класса, т.к. эта таблица статическая, не для объекта:
Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "Дракон", 'Д', 20, 4, 100 },
	{ "Орк", 'О', 4, 2, 25 },
	{ "Лизун", 'Л', 1, 1, 10 }
};

// получение ответа от пользователя, что он хочет
char RunOrFight()
{
	char answer = '0';
	cout << "Что будешь делать? Бежать ? - нажми 'б'. Ударить первым - нажми 'у': ";
    cin >> answer;
	while (!((answer == 'б') || (answer == 'у')))
	{
		cout << "Твой ответ не понятен! Бежать - нажми 'б', ударить первым - нажми 'у': ";
		cin >> answer;
	};
	return answer;
}

// нанесение удара игроком монстру
void playerAttack(Player& player, Monster& monster)
{
	cout << "Ты хорошенько прицеливаешься и наносишь монстру " << player.getForce() << " единиц урона! \n";
	monster.reduceHealth(player.getForce());
	if (monster.isDead())
	{
		cout << "Ты победил этого монстра!\n";
		player.levelUp();
		player.addGold(monster.getGold());
		cout << "Теперь у тебя " << player.getHealth() << " единиц здоровья и " << player.getGold() << " единиц золота. К тому же, твоя сила теперь " << player.getForce() << '\n';
	}
	else 
		cout << "Это был неплохой удар, но " << monster.getName() << " ещё жив! \n";
}

// нанесение удара игроком монстру
void monsterAttack(Player& player, Monster& monster)
{
	cout << monster.getName() << " атакует тебя всей своей силой! ";
	player.reduceHealth(monster.getForce());
	if (!player.isDead())
	{
		cout << "У тебя " << player.getHealth() << " единиц здоровья. ";
		cout << "Ты всё ещё жив! \n";
	}
}

int main()
{
	// для кириллицы:
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	/*
	Creature o("orc", 'o', 4, 2, 10);
	o.addGold(5);
	o.reduceHealth(1);
	cout << o.getName() << " обладает здоровьем в " << o.getHealth() << " единиц и носит с собой " << o.getGold() << " единиц золота." << '\n';
	*/

	Player player(invitation());

	cout << '\n' << "Добро пожаловать в Подземелье Монстров, " << player.getName() << "! Посмотрим, насколько тебя хватит!" << '\n';
	cout << "У тебя " << player.getHealth() << " единиц здоровья и " << player.getGold() << " единиц золота." << '\n';

	//аналог randomize с привязкой ко времени запуска:
	srand(static_cast<unsigned int>(time(0)));
	rand();

	/*
	//генерируем 10 рандомных монстров:
	for (int i = 0; i < 10; ++i)
	{
		Monster m(Monster::getRandomMonster()); // создаём монстра рандомного типа
		cout << "Был создан случайный монстр " << m.getName() << " (" << m.getSymbol() << ").\n";
	}
	*/

	//цикл игры. Играем, пока игрок не умрёт или не достигнет максимально возможного уровня
	do 
	{
		// игрок сталкивается со случайно выбранным монстром:
		Monster m(Monster::getRandomMonster()); // создаём монстра рандомного типа
	    cout << "Идёшь по подземелью, и вдруг прямо перед тобой возникает " << m.getName() << "! " << "\n";

		bool run = false; // не сбежали
		unsigned short probability = getRandomNumber(1, 100); // вероятность успешного побега, если больше 50 - будем считать, что сбежать удалось

		do // играем с этим монстром
		{
			if (RunOrFight() == 'б') // игрок хочет бежать
			{
				probability = getRandomNumber(1, 100);
				run = (probability > 50);
				if (run) // удалось сбежать
					cout << "Тебе удалось сбежать! " << "\n";
				else // сбежать не удалось
				{
					cout << "Тебе не удалось сбежать! ";
					monsterAttack(player, m);
				}
			}
			else
			{
				playerAttack(player, m);
				if (!(m.isDead())) monsterAttack(player, m);
			}
		} 
		while ((!run) && (!player.hasWon()) && (!player.isDead()) && (!m.isDead())); // пока не сбежим, не победим, не умрём или пока не умрёт монстр
	}
	while ((!player.hasWon()) && (!player.isDead())); // играем пока жив и пока не победил

	if (player.isDead())
		cout << "\n К сожалению, эти монстры добили тебя! Ты погибаешь на " << player.getLevel() << " уровне, потеряв " << player.getGold() << " золота... Очень жаль! \n";
	if (player.hasWon())
		cout << "\n Вот это да! Ты победил всех монстров, достиг "<< player.getLevel() << " уровня и выбрался из подземелья, забрав с собой " << player.getGold() << " золота. Это было грандиозно! \n";


	return 0;
}
