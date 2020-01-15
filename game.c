#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ==============================================
// max predator number
#define MAXPREDATOR 30
// max prey number
#define MAXPREY 60
// 0 - without spawn
#define PREYSPAWNFREQUENCY 5
// 0 - spawn one predator per frame
// -1 - without spawn
#define PREDATORFOODTOSPAWN 2
// -1 - without death
#define PREYYEARSTODIE -2
#define PREDATORYEARSTODIE -2
#define PLAYERYEARSTODIE -2
// start creatures number
#define STARTPREY 6
#define STARTPREDATOR 2
// field size
#define FIELDWIDTH 40
#define FIELDHEIGHT 20
// symbols
#define PREYCHAR '-'
#define PREDATORCHAR '<'
#define FIELDEMPTYCHAR ' '
#define FIELDBORDERCHAR '#'
#define PLAYERCHAR '*'
// ===============================================
// colors
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct {
  int id;
  int x;
  int y;
  int food;
  int year;
} t_creature;

// creatures initialization
void creaturesInit(t_creature *creature, int maxCreature, int creatureFood, int creatureYears);
// primary creature spawn
int spawnCreature(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreature);
// field cleaDring and creating
void makeField(char field[FIELDHEIGHT][FIELDWIDTH]);
// placing creatures on the field
void printCreatures(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreature, char creatureChar);
// field printing
void printField(char field[FIELDHEIGHT][FIELDWIDTH], int score, int steps);
// prey moving
void movePrey(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature);
// predator moving
void movePredator(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature);
// player moving
void movePlayer(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, char dir);
// creature birth
int birthCreature(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, t_creature *Mother);
// satiety check
int checkFood(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreatures, int creatureFood, int creatureYears);
// creature death
void death(t_creature *creature, int maxCreature, int creatureYears);
// take for user
char getch(); 
// save score
int saveScore(int score, int steps);
// read score
int readScore(int score, int steps);

int main(void) {
  int seccounter = 0, randMother, r, score=0, steps=0;
  char temp='w';
  srand(time(NULL));
  char field[FIELDHEIGHT][FIELDWIDTH];
  t_creature prey[MAXPREY];
  t_creature predator[MAXPREDATOR];
  t_creature player[1];
  creaturesInit(prey, MAXPREY, PREDATORFOODTOSPAWN, PREYYEARSTODIE);
  creaturesInit(predator, MAXPREDATOR, PREDATORFOODTOSPAWN, PREDATORYEARSTODIE);
  creaturesInit(player, 1, 0, PLAYERYEARSTODIE);
  
  // spawn in different cells
  for(int i = 0; i < STARTPREY; i++) {
    r = spawnCreature(field, prey, MAXPREY);
    if(r == 0) {
      i--;
    } 
  }
  for(int i = 0; i < STARTPREDATOR; i++) {
    r = spawnCreature(field, predator, MAXPREDATOR);
    if(r == 0) {
      i--;
    } 
  }
  
  for(int i = 0; i < 1; i++) {
    r = spawnCreature(field, player, 1);
    if(r == 0) {
      i--;
    }
  }

  while(1) {
	system("clear");
    // move prey
    if(temp=='e'){
		readScore(score,steps);
	}
    for(int i = 0; i < MAXPREY; i++) {
      if(prey[i].id != -1) {
        movePrey(field, &prey[i]);
        makeField(field);
        printCreatures(field, prey, MAXPREY, PREYCHAR);
        printCreatures(field, predator, MAXPREDATOR, PREDATORCHAR);
        printCreatures(field, player, 1, PLAYERCHAR);
      }
    }
    // move predator
    for(int i = 0; i < MAXPREDATOR; i++) {
      if(predator[i].id != -1) {
        movePredator(field, &predator[i]);
        for(int j = 0; j < MAXPREY; j++) {
          if((prey[j].x == predator[i].x) && (prey[j].y == predator[i].y)) {
            prey[j].id = -1;
            predator[i].food++;
          }
        }
		if((player[0].x == predator[i].x) && (player[0].y == predator[i].y)) {
            player[0].id = -1;
            predator[i].food++;
        }
        makeField(field);
        printCreatures(field, prey, MAXPREY, PREYCHAR);
        printCreatures(field, predator, MAXPREDATOR, PREDATORCHAR);
        printCreatures(field, player, 1, PLAYERCHAR);
        
      }
    }
    // birth prey
    if(PREYSPAWNFREQUENCY > 0) {
      if(seccounter >= PREYSPAWNFREQUENCY) {
        /*for(int i = 0; i < MAXPREY; i++) {
          if(prey[i].id == -1) {
            randMother = i;
            i = MAXPREY;
          }
        }
        while(prey[randMother].id == -1) {
          randMother = rand() % (MAXPREY-1) + 0;
        }*/
        r = checkFood(field, prey, MAXPREY, -1, PREYYEARSTODIE);
        if(r == 1) {
          seccounter = 0;
        }
      }
      seccounter++;
    }
    // birth predator
    checkFood(field, predator, MAXPREDATOR, PREDATORFOODTOSPAWN, PREDATORYEARSTODIE);
    makeField(field);
    printCreatures(field, prey, MAXPREY, PREYCHAR);
    printCreatures(field, predator, MAXPREDATOR, PREDATORCHAR);
    printCreatures(field, player, 1, PLAYERCHAR);
    // death
    if(PREDATORYEARSTODIE >= 0) {
      death(predator, MAXPREDATOR, PREDATORYEARSTODIE);
    }
    if(PREYYEARSTODIE >= 0) {
      death(prey, MAXPREY, PREYYEARSTODIE);
    }
    printField(field,score,steps);
    temp=getch();
    if(temp=='e'){
		readScore(score,steps);
	}
    if(temp=='q'){
		printf("\nDo you want save the score?\n (y/n)");
		temp=getch();
		if(temp=='y')
		saveScore(score,steps);
		return 0;
	}
	if(player[0].id == -1){
		saveScore(score,steps);
		printf("\nGame Over\n");
		return 0;
	}

	movePlayer(field, &player[0],temp);
	if(temp=='w' || temp=='s' || temp=='a' || temp=='d') steps++;
	//movePlayer(field, &player[0],temp);
	for(int j = 0; j < MAXPREY; j++) {
        if((player[0].x == prey[j].x) && (player[0].y == prey[j].y)) {
			prey[j].id = -1;
            player[0].food++;
            if(temp=='w' || temp=='s' || temp=='a' || temp=='d') score++;
          }
        }
    // print, sleep, clear

  }
  return 0;
}

void death(t_creature *creature, int maxCreature, int creatureYears) {
  for(int i = 0; i < maxCreature; i++) {
    if(creature[i].id != -1) {
      if(creature[i].year >= creatureYears) {
        creature[i].id = -1;
      } else {
        creature[i].year++;
      }
    }
  }
}

int checkFood(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreatures, int creatureFood, int creatureYears) {
  for(int i = 0; i < maxCreatures; i++) {
    if((creature[i].id != -1) && (creature[i].food >= creatureFood)) {
      int j, r;
      for(j = 0; (creature[j].id != -1) && (j < maxCreatures); j++);
      if((j == maxCreatures) && (creature[j].id != -1)) {
        return 0;
      }
      creature[j].id = j;
      r = birthCreature(field, &creature[j], &creature[i]);
      if(r == 1) {
        if(creatureYears < 0) {
          creature[j].year = -1;
        } else {
          creature[j].year = 0;
        }
        if(creatureFood < 0) {
          creature[j].food = -1;
        } else {
          creature[j].food = 0;
          creature[i].food -= creatureFood;
        }
        return 1;
      } else if (r == 0) {
        creature[j].id = -1;
      }
    }
  }
  return 0;
}

int birthCreature(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, t_creature *Mother) {
  int dir;
  dir = rand() % 4 + 1;
  if((dir == 1) && (Mother->x+1 != FIELDWIDTH-1) && (field[Mother->y][Mother->x+1] != PREYCHAR) && (field[Mother->y][Mother->x+1] != PREDATORCHAR)) {
    creature->x = Mother->x+1;
    creature->y = Mother->y;
    return 1;
  } else if((dir == 2) && (Mother->x-1 != 0) && (field[Mother->y][Mother->x-1] != PREYCHAR) && (field[Mother->y][Mother->x-1] != PREDATORCHAR)) {
    creature->x = Mother->x-1;
    creature->y = Mother->y;
    return 1;
  } else if((dir == 3) && (Mother->y+1 != FIELDHEIGHT-1) && (field[Mother->y+1][Mother->x] != PREYCHAR) && (field[Mother->y+1][Mother->x] != PREDATORCHAR)) {
    creature->x = Mother->x;
    creature->y = Mother->y+1;
    return 1;
  } else if((dir == 4) && (Mother->y-1 != 0) && (field[Mother->y-1][Mother->x] != PREYCHAR) && (field[Mother->y-1][Mother->x] != PREDATORCHAR)) {
    creature->x = Mother->x;
    creature->y = Mother->y-1;
    return 1;
  }
  return 0;
}

void movePredator(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature) {
  int dir;
  dir = rand() % 4 + 1;
  if((dir == 1) && (creature->x+1 != FIELDWIDTH-1) && (field[creature->y][creature->x+1] != PREDATORCHAR)) {
    creature->x++;
  } else if((dir == 2) && (creature->x-1 != 0) && (field[creature->y][creature->x-1] != PREDATORCHAR)) {
    creature->x--;
  } else if((dir == 3) && (creature->y+1 != FIELDHEIGHT-1)  && (field[creature->y+1][creature->x] != PREDATORCHAR)) {
    creature->y++;
  } else if((dir == 4) && (creature->y-1 != 0) && (field[creature->y-1][creature->x] != PREDATORCHAR)) {
    creature->y--;
  }
}

void movePrey(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature) {
  int dir;
  dir = rand() % 4 + 1;
  if((dir == 1) && (creature->x+1 != FIELDWIDTH-1) && (field[creature->y][creature->x+1] != PREYCHAR) && (field[creature->y][creature->x+1] != PREDATORCHAR ) && (field[creature->y][creature->x+1] != PLAYERCHAR)) {
    creature->x++;
  } else if((dir == 2) && (creature->x-1 != 0) && (field[creature->y][creature->x-1] != PREYCHAR) && (field[creature->y][creature->x-1] != PREDATORCHAR) && (field[creature->y][creature->x+1] != PLAYERCHAR)) {
    creature->x--;
  } else if((dir == 3) && (creature->y+1 != FIELDHEIGHT-1) && (field[creature->y+1][creature->x] != PREYCHAR) && (field[creature->y+1][creature->x] != PREDATORCHAR) && (field[creature->y][creature->x+1] != PLAYERCHAR)) {
    creature->y++;
  } else if((dir == 4) && (creature->y-1 != 0) && (field[creature->y-1][creature->x] != PREYCHAR) && (field[creature->y-1][creature->x] != PREDATORCHAR) && (field[creature->y][creature->x+1] != PLAYERCHAR)) {
    creature->y--;
  }
}

void movePlayer(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, char dir) {
  if((dir == 'd') && (creature->x+1 != FIELDWIDTH-1) && (field[creature->y][creature->x+1] != PREDATORCHAR)) {
    creature->x++;
  } else if((dir == 'a') && (creature->x-1 != 0) && (field[creature->y][creature->x-1] != PREDATORCHAR)) {
    creature->x--;
  } else if((dir == 's') && (creature->y+1 != FIELDHEIGHT-1)  && (field[creature->y+1][creature->x] != PREDATORCHAR)) {
    creature->y++;
  } else if((dir == 'w') && (creature->y-1 != 0) && (field[creature->y-1][creature->x] != PREDATORCHAR)) {
    creature->y--;
  }
}

void printField(char field[FIELDHEIGHT][FIELDWIDTH], int score, int steps) {
  for(int i = 0; i < FIELDHEIGHT; i++) {
    for(int j = 0; j < FIELDWIDTH; j++) {
      if(field[i][j] == PREYCHAR) {
        printf("%s", GREEN);
        printf("%c", field[i][j]);
        printf("%s", RESET);
      } else if(field[i][j] == PREDATORCHAR) {
        printf("%s", RED);
        printf("%c", field[i][j]);
        printf("%s", RESET);
      } else if(field[i][j] == PLAYERCHAR) {
        printf("%s", WHITE);
        printf("%c", field[i][j]);
        printf("%s", RESET);
      } else {
        printf("%s", CYAN);
        printf("%c", field[i][j]);
        printf("%s", RESET);
      }
    }
    printf("\n");
  }
    printf("score: %d\nsteps: %d\n",score,steps);
    printf("move - w,s,a,d\n");
    printf("show last score - e\n");
    printf("skip the step - any other key\n");
    printf("quit - q\n");
}

void printCreatures(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreature, char creatureChar) {
  for(int i = 0; i < maxCreature; i++) {
    if(creature[i].id != -1) {
      field[creature[i].y][creature[i].x] = creatureChar;
    }
  }
}

void makeField(char field[FIELDHEIGHT][FIELDWIDTH]) {
  for(int i = 0; i < FIELDHEIGHT; i++) {
    for(int j = 0; j < FIELDWIDTH; j++) {
      field[i][j] = FIELDEMPTYCHAR;
      if((i == 0) || (i == FIELDHEIGHT-1)) {
        field[i][j] = FIELDBORDERCHAR;
      }
      if((j == 0) || (j == FIELDWIDTH-1)) {
        field[i][j] = FIELDBORDERCHAR;
      }
    }
  }
}


int spawnCreature(char field[FIELDHEIGHT][FIELDWIDTH], t_creature *creature, int maxCreature) {
  int tmpX, tmpY;
  for(int i = 0; i < maxCreature; i++) {
    if(creature[i].id == -1) {
      tmpX = rand() % (FIELDWIDTH-2) + 1;
      tmpY = rand() % (FIELDHEIGHT-2) + 1;
      if((field[tmpY][tmpX] != PREYCHAR) && (field[tmpY][tmpX] != PREDATORCHAR) && (field[tmpY][tmpX] != PLAYERCHAR)) {
        creature[i].id = i;
        creature[i].x = tmpX;
        creature[i].y = tmpY;
        return 1;
      } else {
        return 0;
      }
    }
  }
  return 0;
}

void creaturesInit(t_creature *creature, int maxCreature, int creatureFood, int creatureYears) {
  for(int i = 0; i < maxCreature; i++) {
    creature[i].id = -1;
    if(creatureFood < 0) {
      creature[i].food = -2;
    } else {
      creature[i].food = 0;
    }
    if(creatureYears < 0) {
      creature[i].year = -1;
    } else {
      creature[i].year = 0;
    }
  }
}

char getch()
{
    system ("/bin/stty raw");  
    char ret = getchar();
    system ("/bin/stty cooked");
    return ret;
}

int saveScore(int score, int steps){
	FILE *f = fopen("lastScore.txt","a+");
    if (f == NULL)
    {
        printf("Error\n");
        return 0;
    }
    int d = fprintf(f, "score: %-10d steps:%-5d\n", score, steps);
    printf("\nScore Saved\n");
    return 1;
}

int readScore(int score, int steps){
	char temp;
	system("clear");
	char cc[100];
	char * filename = "lastScore.txt";
	FILE *fp;
    if((fp= fopen(filename, "r"))==NULL)
    {
        perror("Error occured while opening file");
        return 0;
    }
    //before the end read for 256 bites
    while((fgets(cc, 100, fp))!=NULL)
    {
        printf("\n%s", cc);
    }
    temp=getch();
    return 1;
}

