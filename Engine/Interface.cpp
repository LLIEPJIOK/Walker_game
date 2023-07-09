#include "Interface.h"

std::string** Interface::field = 0;
bool Interface::flag = 0;
bool Interface::was_attack = 0;
Turn Interface::turn{};

void Interface::clear()
{
	for (int i = 0; i < db->get_rows() + 2; ++i)
		delete[]field[i];
	delete[]field;
	field = 0;
}

void Interface::choose_action(const bool& is_begin)
{
	int command = 0;
	was_attack = 0;
	while (command != 1)
	{
		show_map(0);
		std::cin >> command;
#pragma warning ���������
		switch (command)
		{
		case 2:
		{
			command_attack();
			break;
		}
		case 3:
		{
			command_potions();
			break;
		}
		case 4:
		{
			command_equip();
			break;
		}
		}
	}
}

void Interface::command_attack()
{
	if (!flag)
		std::cout << "�� �� ������ ��������� �� ������ ����!\n";
	else if (was_attack)
		std::cout << "�� ��� ��������� � ���� ����\n";
	else
		was_attack = turn.attack();
	system("pause");
}

void Interface::command_potions()
{
	if (!flag)
		std::cout << "�� �� ������ ������������ ����� �� ������ ����!\n";
	else
		turn.potions();
	system("pause");
}

void Interface::command_equip()
{
	if (!flag)
		std::cout << "�� �� ������ ������������� �� ������ ����!\n";
	else
		turn.equip();
	system("pause");
}

int Interface::number_of_payers()
{

	int kol;
	std::cin >> kol;
#pragma warning processing
	return kol;
}

std::string Interface::players_names()
{
	std::cout << "������� ���\n";
	std::string name;
	std::cin >> name;
	return name;
}

bool Interface::win()
{
	if (seq->size() == 1)
	{
		system("cls");

		return 1;
	}
	return 0;
}

void Interface::begin()
{
	create_matrix();
	while (1)
	{
		if (win())
		{
			clear();
			return;
		}
		for (const auto& i : *seq)
		{
			turn.set_player(i.second);
			choose_action(1);
			turn.go();
			choose_action(0);
		}
		db->generate_item();
		flag = 1;
	}
}

void Interface::create_matrix()
{
    int** matrix = db->get_matrix();
    field = new std::string * [db->get_rows() + 2];
    for (int i = 0; i < db->get_rows() + 2; ++i)
    {
        field[i] = new std::string[db->get_cols() + 2];
        for (int j = 0; j < db->get_cols() + 2; ++j)
            field[i][j] = std::to_string(matrix[i][j]);
    }
    for (const auto& i : *seq)
    {
        int x = i.second->get_x();
        int y = i.second->get_y();
        if (isdigit(field[y][x][0]))
            field[y][x] = 'P' + std::to_string(i.second->get_id());
        else
            field[y][x] += 'P' + std::to_string(i.second->get_id());
    }
}

void Interface::show_characteristics()
{
    std::cout << "������:\n";
    int counter = 0;
    for (const auto& i : *seq)
        std::cout << ++counter << ". " << i.second->get_name() << " (P" << i.second->get_id() << "): HP - " <<
        i.second->get_characteristics().at("HP") << ", ATK - " << i.second->get_characteristics().at("ATK") <<
        ", RNG - " << i.second->get_characteristics().at("RNG") << '\n';
}

void Interface::show_map(const int& rolls)
{
    system("cls");
    for (int i = 0; i < db->get_rows() + 2; ++i)
    {
        for (int j = 0; j < db->get_cols() + 2; ++j)
            std::cout << std::setw(6) << field[i][j] << ' ';
        std::cout << '\n';
    }
    show_characteristics();
	if (rolls)
		std::cout << "���������� ������ - " << rolls << '\n';
}

void Interface::selection(Player* player, const int& x1, const int& y1, const int& x2, const int& y2)
{
	if (db->get_matrix()[y2][x2] > 1)
	{
		std::string equipment_id = std::to_string(db->get_matrix()[y2][x2]);
#pragma wraning ����� �������� ��������
		std::cout << "\n*****������� ��������*****\n";
		db->minus_item();
		player->add_item(equipment_id);
		system("color 04");
		std::cout << "---- " << player->get_name() << " (P" << player->get_id() << ") ----\n";
		system("pause");
		system("color 07");
		db->get_matrix()[y2][x2] = 1;
	}
}

char Interface::check_direction(Player* player, std::string& ways)
{
	char choice;
	if (std::count(ways.begin(), ways.end(), ' ') != 1)
	{
		std::cout << "\n---- " << player->get_name() << " (P" << player->get_id() << ") ----\n" <<
			"�������� �����������:\n" << ways << '\n';
		std::cin >> choice;
	}
	else
	{
		ways.pop_back();
		choice = ways[0];
	}
	choice = toupper(choice);
#pragma warning ���������
	return choice;
}

std::string Interface::choose_hand(Player* player)
{
	int choice;
	std::cout << "�������� ���� ��� ��������������:\n\t1) " <<
		player->get_equiped_weaponary()->at("main_hand")->get_name() << "\n\t2) " <<
		player->get_equiped_weaponary()->at("non_dominant_hand")->get_name() <<
		"\n\t3) �����\n";
	std::cin >> choice;
#pragma warning ���������
	switch (choice)
	{
	case 1:
		return "main_hand";
	case 2:
		return "non_dominant_hand";
	case 3:
		return "exit";
	default:
		assert(0);
	}
}

std::string Interface::choose_ring(Player* player)
{
	int choice;
	std::cout << "�������� ������ ��� ��������������:\n\t1) " <<
		player->get_equiped_jewellery()->at("first_ring")->get_name() << "\n\t2) " <<
		player->get_equiped_jewellery()->at("second_ring")->get_name() <<
		"\n\t3) �����\n";
	std::cin >> choice;
#pragma warning ���������
	switch (choice)
	{
	case 1:
		return "first_hand";
	case 2:
		return "second_ring";
	case 3:
		return "exit";
	default:
		assert(0);
	}
}

Player* Interface::check_attack(Player* player)
{
	std::vector <std::pair <int, std::string> > vect = player->check_range(player->get_characteristics().at("RNG"));
	if (vect.empty())
	{
		std::cout << "�� �� ������ ������ ����������\n";
		return 0;
	}
	std::cout << "�������� ������ ��� �����:\n";
	for (int i = 0; i < vect.size(); i++)
		std::cout << i + 1 << ") " << vect[i].second << " (P" << vect[i].first << ")\n";
	std::cout << vect.size() + 1 << ") �����\n";
	int numb;
	std::cin >> numb;
#pragma warining ���������
	if (numb == vect.size() + 1)
		return 0;
	Player* pl = seq->begin()->second;
	for (const auto& i : *seq)
		if (i.second->get_id() == vect[numb - 1].first)
		{
			pl = i.second;
			break;
		}
	std::cout << "�� ��������� " << pl->get_name() << '\n';
	return pl;
}

std::multiset<Potion*>::iterator Interface::check_potions(Player* player)
{
	if (!player->get_potions()->size())
	{
		std::cout << "� ��� ��� �����\n";
		return player->get_potions()->end();
	}
	std::cout << "�������� �����:\n";
	int counter = 0;
	for (const auto& i : *player->get_potions())
		std::cout << ++counter << ") " << i->get_name() << std::endl;
	std::cout << ++counter << ") �����\n";
	int numb;
#pragma warning ���������
	std::cin >> numb;
	if (numb == counter)
		return player->get_potions()->end();
	auto it = player->get_potions()->begin();
	for (int i = 1; i < numb; ++i)
		++it;
	std::cout << "�� ������������ " << (*it)->get_name() << '\n';
	return it;
}

std::pair<int, Equipment*> Interface::check_equip(Player* player)
{

	int choice;
	std::cin >> choice;
#pragma warning ���������
	if (choice == 1)
	{
		std::cout << "������:\n";
		int counter = 0;
		for (const auto& i : *player->get_weaponary())
				std::cout << '\t' << ++counter << ") " << i->get_name() << '\n';
		std::cout << '\t' << ++counter << ") �����\n";
		int variant;
		std::cin >> variant;
#pragma warning ���������
		if (variant == counter)
			return { 0, nullptr };
		auto it = player->get_weaponary()->begin();
		std::advance(it, variant - 1);
		return { 1, *it };
	}
	if (choice == 2)
	{
		std::cout << "�����:\n";
		int counter = 0;
		for (const auto& i : *player->get_armourment())
				std::cout << '\t' << ++counter << ") " << i->get_name() << '\n';
		std::cout << '\t' << ++counter << ") �����\n";
		int variant;
		std::cin >> variant;
#pragma warning ���������
		if (variant == counter)
			return { 0, nullptr };
		auto it = player->get_armourment()->begin();
		std::advance(it, variant - 1);
		return { 2, *it};
	}
	if (choice == 3)
	{
		std::cout << "�������������:\n";
		int counter = 0;
		for (const auto& i : *player->get_jewellery())
			std::cout << '\t' << ++counter << ") " << i->get_name() << '\n';
		std::cout << '\t' << ++counter << ") �����\n";
		int variant;
		std::cin >> variant;
#pragma warning ���������
		if (variant == counter)
			return { 0, nullptr };
		auto it = player->get_jewellery()->begin();
		std::advance(it, variant - 1);
		return { 3, *it };
	}
	return { 0, nullptr};
}

std::string** Interface::get_field()
{
	return field;
}
