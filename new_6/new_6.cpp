#include "stdafx.h"
#include "string.h"

typedef struct PointOfCharacter 
{
	double x;
	double y;
	double z;
}Point;

typedef struct MonsterOfCosGame
{
	// 怪物的各种角色信息
	int ID; // 怪物ID 1为怪物 2为NPC 3为玩家角色
	char name[12];  // 怪物名字 长度最多6个汉字
	int level; // 怪物等级
	Point Location; // 怪物 x-y-z 坐标
}Monster;

Monster MonsterArr[10];

void input()
{
	// 此函数为MonsterArr数组赋值
	MonsterArr[0].ID = 1;
	strcpy(MonsterArr[0].name, "超核神兵");
	MonsterArr[0].level = 6;
	MonsterArr[0].Location.x = 12.1;
	MonsterArr[0].Location.y = 11.3;
	MonsterArr[0].Location.z = 10.0;

	MonsterArr[1].ID = 1;
	strcpy(MonsterArr[1].name, "爱因斯坦");
	MonsterArr[1].level = 10;
	MonsterArr[1].Location.x = 17.5;
	MonsterArr[1].Location.y = 65.3;
	MonsterArr[1].Location.z = 10.0;

	MonsterArr[2].ID = 2;
	strcpy(MonsterArr[2].name, "商店老板");
	MonsterArr[2].level = 1;
	MonsterArr[2].Location.x = 10.1;
	MonsterArr[2].Location.y = 11.2;
	MonsterArr[2].Location.z = 10.0;

	MonsterArr[3].ID = 2;
	strcpy(MonsterArr[3].name, "酒馆");
	MonsterArr[3].level = 1;
	MonsterArr[3].Location.x = 52.5;
	MonsterArr[3].Location.y = 75.3;
	MonsterArr[3].Location.z = 10.0;

	MonsterArr[4].ID = 3;
	strcpy(MonsterArr[4].name, "玩家1");
	MonsterArr[4].level = 3;
	MonsterArr[4].Location.x = 10.1;
	MonsterArr[4].Location.y = 11.2;
	MonsterArr[4].Location.z = 10.0;

	MonsterArr[5].ID = 3;
	strcpy(MonsterArr[5].name, "玩家2");
	MonsterArr[5].level = 2;
	MonsterArr[5].Location.x = 52.5;
	MonsterArr[5].Location.y = 75.3;
	MonsterArr[5].Location.z = 10.0;

	MonsterArr[6].ID = 1;
	strcpy(MonsterArr[6].name, "玉皇大帝");
	MonsterArr[6].level = 10;
	MonsterArr[6].Location.x = 12.1;
	MonsterArr[6].Location.y = 11.3;
	MonsterArr[6].Location.z = 10.0;

	MonsterArr[7].ID = 1;
	strcpy(MonsterArr[7].name, "齐天大圣");
	MonsterArr[7].level = 10;
	MonsterArr[7].Location.x = 12.1;
	MonsterArr[7].Location.y = 11.3;
	MonsterArr[7].Location.z = 10.0;

	MonsterArr[8].ID = 1;
	strcpy(MonsterArr[8].name, "波塞冬");
	MonsterArr[8].level = 8;
	MonsterArr[8].Location.x = 12.1;
	MonsterArr[8].Location.y = 11.3;
	MonsterArr[8].Location.z = 10.0;

	MonsterArr[9].ID = 1;
	strcpy(MonsterArr[9].name, "雅典娜");
	MonsterArr[9].level = 9;
	MonsterArr[9].Location.x = 12.1;
	MonsterArr[9].Location.y = 11.3;
	MonsterArr[9].Location.z = 10.0;
}

void output(int id)
{
	// 此函数通过怪物ID打印怪物的所有信息
	for (int num = 0; num < 10; num++)
	{
		if (MonsterArr[num].ID == id)
		{
			printf("怪物名称: %s\n怪物等级: %d\n怪物坐标: x:%.1f  y:%.1f  z:%.1f\n\n", \
				MonsterArr[num].name, \
				MonsterArr[num].level, \
				MonsterArr[num].Location.x, \
				MonsterArr[num].Location.y, \
				MonsterArr[num].Location.z);
		}
	}
}

int main(int argc, char* argv[])
{
	input();
	output(1);  // 参数代表要打印谁的信息  1为怪物 2为NPC 3为玩家角色
	getchar();
	return 0;
}