#include <iostream>
#include <ncurses.h>
#include <vector>
#include <time.h>

using namespace std;

enum Type{
    EMPTY, 
    MY_TANK,
    ENEMY_TANK,
    GUN,
    GLASS,
    WOOD,
    CONCRETE,
    BORDER
};
class Pixel{
private:
    int x, y;
    char symbol;
    Type type;
public:
    Pixel()=default;
    Pixel(int x, int y){
        this->x = x;
        this->y = y;
    }
    Pixel(int x, int y, char symbol){
        Pixel(x, y);
        this->symbol = symbol;
    }

    Pixel operator+(Pixel& vector);
    void setXY(int x, int y);
    void setXY(Pixel vector);
    void setX(int x);
    void setY(int y);
    void setSymbol(char symbol);
    char getSymbol()const;
    void setType(Type type);
    Type getType()const;
    int getX()const;
    int getY()const;
};

Pixel Pixel::operator+(Pixel& vector){
    Pixel p(*this);
    p.x = p.x + vector.getX();
    p.y = p.y + vector.getY();
    return p;
}

void Pixel::setXY(int x, int y){
    this->x = x;
    this->y = y;
}
void Pixel::setXY(Pixel vector){
    this->x = vector.getX();
    this->y = vector.getY();
}
void Pixel::setX(int x){
    this->x = x;
}
void Pixel::setY(int y){
    this->y = y;
}
void Pixel::setSymbol(char symbol){
    this->symbol = symbol;
}
char Pixel::getSymbol()const{
    return symbol;
}
void Pixel::setType(Type type){
    this->type = type;
}
Type Pixel::getType()const{
    return type;
}
int Pixel::getX()const{
    return x;
}
int Pixel::getY()const{
    return y;
}

class Map{
private:
    Pixel** map;
public:
    Map(){
        createMap();
    }
    void createBorder();
    void createMap();
    void clearMap();
    void showMap();
    Pixel**& getMap();
};
void Map::createBorder() {
    for(int i = 0; i < 45; i++){
        for(int j = 0; j < 45; j++){
            if(i == 0 || j == 0 || i == 44 || j == 44){
                Pixel p(i, j,'#');
                map[i][j] = p;
                map[i][j].setType(BORDER);
            }
        }
    }
}
void Map::createMap(){
    map = new Pixel*[45];
    for(int i = 0 ; i < 45; ++i){
        map[i] = new Pixel[45];
    }
    createBorder();
    for(int i = 1; i < 44; i++){
        for(int j = 1; j < 44; j++){
            Pixel p(i, j,' ');
            map[i][j] = p;
            map[i][j].setType(BORDER);
        }
    }
}
void Map::clearMap(){
    for(int i = 1; i < 44; i++){
        for(int j = 1; j < 44; j++){
            Pixel p(i, j,' ');
            map[i][j] = p;
            map[i][j].setType(EMPTY);
        }
    }
}
void Map::showMap(){
    createBorder();
    for(int i = 0; i < 45; i++){
        for(int j = 0; j < 45; j++){
            if(map[i][j].getType() == EMPTY){
                attron(COLOR_PAIR(1));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(1));
            } else if(map[i][j].getType() == MY_TANK){
                attron(COLOR_PAIR(2));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(2));
            } else if(map[i][j].getType() == ENEMY_TANK){
                attron(COLOR_PAIR(3));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(3));
            } else if(map[i][j].getType() == GLASS){
                attron(COLOR_PAIR(4));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(4));
            } else if(map[i][j].getType() == CONCRETE){
                attron(COLOR_PAIR(5));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(5));
            } else if(map[i][j].getType() == WOOD){
                attron(COLOR_PAIR(6));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(6));
            } else if(map[i][j].getType() == GUN){
                attron(COLOR_PAIR(7));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(7));
            } else if(map[i][j].getType() == BORDER){
                attron(COLOR_PAIR(8));
                printw(" %c", map[i][j].getSymbol());
                attroff(COLOR_PAIR(8));
            } else {
                printw(" %c", map[i][j].getSymbol());
            }
        }
        printw("\n");
    }
    //refresh();
}
Pixel**& Map::getMap(){
    return map;
}



enum Direction{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};


class Area{
private:
    int area[4];
public:
    Area()=default;
    Area(int ltX, int ltY, int rdX, int rdY){
        area[0] = ltX;
        area[1] = ltY;
        area[2] = rdX;
        area[3] = rdY;
    }

    int* getArea(){
        return area;
    }

    int getRight() const{
        return area[3];
    }
    int getLeft() const{
        return area[1];
    }
    int getTop() const{
        return area[0];
    }
    int getBottom() const{
        return area[2];
    }
};
class Bullet;
class Tank;
class MyTank;
class EnemyTank;
class Barrier;
class Game{
private:
    MyTank* myTank;
    int speed = 0;
    Map *newMap;
    vector<pair<char*,int>> highScoreMap;
    vector<Tank*> tankArray;
    vector<Barrier*> barrierArray;
    Bullet* bulletArray;
    int score;
    int countOfBullet = 0;
    int countOfBarriersNow = 0;
    int countOfTanksNow = 0;
    bool isActiveGame = false;
public:
    Game(){
        newMap = new Map();
        createBarriers(43);
        createTanks(3);
        createBullet(100);
        score = 0;
        isActiveGame = true;
    }
    Map* getMap();
    vector<pair<char*,int>> getHighScoreMap();
    void setHighScoreMap(vector<pair<char*,int>>& newScoreMap);
    void update();
    void render();
    MyTank* getMyTank();
    bool isCross(Area a, Area b);
    Tank* crossTank(Area a, Tank *except);
    Barrier* crossBarrier(Area a);
    bool crossFireMyTank(Tank* tank);
    void createTanks(int count);
    void createBarriers(int count);
    void createBullet(int count);
    Bullet& getBullet();
    void setScore(int s);
    int getScore() const;
    int getCountOfBarriersNow() const;
    void setCountOfBarriersNow(int c);
    int getCountOfTanksNow() const;
    void setCountOfTanksNow(int c);
    void printInformation() const;
    void setActiveGame(bool b);
    bool getActiveGame();
};
enum BulletType{
    MACHINE_GUN,
    ROCKET,
    LASER
};
class Bullet {
private:
    Game* m_game;
    BulletType type = MACHINE_GUN;
    Pixel position;
    Pixel dirVector[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    Direction dir;
    bool use = false;
public:
    Bullet()=default;
    Bullet(Game* g){
        setGame(g);
    }
    Area areaBullet = {1,1,43,43};
    void setGame(Game* g);
    void setBullet(Pixel pos, Direction dir);
    void move();
    void show(Pixel**& map);
    bool isUsed();
    void setType(BulletType t);
    BulletType getType() const;
    Direction getDirection() const;
    void setDirection(Direction d);
};
class Tank{
protected:
    Game* m_game;
    Pixel position;
    Pixel sPosition;
    float speed = 0;
    Pixel dirVector[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int proof = 3;
    Direction dir;
    Direction dirOld;
public:
    Tank()=default;
    Tank(int startX, int startY, Game* g){
        sPosition.setXY(startX, startY);
        position.setXY(sPosition);
        setGame(g);
    }
    Area area = {3, 3, 41, 41};
    bool isHoriz();
    Area getArea();
    Pixel getPosition() const;
    virtual void move();
    virtual void show(Pixel**& map);
    virtual void setStartProof();
    int getProof() const;
    void setProof(int p);
    Pixel getDirection() const;
    virtual void setToStart();
    void setGame(Game* game);
    Direction getDir() const;
};
Direction Tank::getDir() const {
    return dir;
}
Pixel Tank::getDirection() const {
    return dirVector[dir];
}
Pixel Tank::getPosition() const{
    return position;
}
void Tank::move(){

}
void Tank::show(Pixel**& map){

}
class EnemyTank : public Tank{
public:
    EnemyTank()=default;
    EnemyTank(int startX, int startY, Game* g) : Tank(startX, startY, g){
        dir = UP;
        dirOld = UP;
    }
    void setToStart() override;
    void setStartProof()override;
    void move()override;
    void show(Pixel**& map)override;
};

class MyTank : public Tank{
private:
    int countOfLife = 3;
public:
    MyTank()=default;
    MyTank(int startX, int startY, Game* g) : Tank(startX, startY, g){
        dir = UP;
        dirOld = UP;
    }
    int getCountOfLife() const;
    void setCountOfLife(int c);
    bool isAlive();
    void setStartProof() override;
    void move()override;
    void show(Pixel**& map)override;
};
bool MyTank::isAlive() {
    return countOfLife > 0;
}
int MyTank::getCountOfLife() const{
    return countOfLife;
}
void MyTank::setCountOfLife(int c){
    countOfLife = c;
}
int Game::getCountOfTanksNow() const{
    return countOfTanksNow;
}
void Game::setCountOfTanksNow(int c){
    countOfTanksNow = c;
}
int Tank::getProof() const{
    return proof;
}
void Tank::setProof(int p){
    proof = p;
}
void Tank::setStartProof(){

}
void Tank::setGame(Game* game){
    m_game = game;
}
void Tank::setToStart(){
    position.setXY(sPosition);
    Tank* tank = m_game->crossTank(getArea(), this);
    if(tank){
        m_game->setScore(m_game->getScore() + 10);
        m_game->setCountOfTanksNow(m_game->getCountOfTanksNow() + 1);
        tank->setStartProof();
        tank->setToStart();
    }
}
bool Tank::isHoriz(){
    return (dir == RIGHT || dir == LEFT);
}
Area Tank::getArea(){
    Area a(position.getX() - 1, position.getY() - 1, position.getX() + 1, position.getY() + 1);
    return a;
}
void MyTank::setStartProof() {
    if(m_game->getScore() < 120){
        proof = 3;
    } else if(m_game->getScore() < 420){
        proof = 9;
    } else {
        proof = 15;
    }
}
void MyTank::show(Pixel**& map){
    Pixel tmpT = dirVector[dir];
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            map[position.getX() - j][position.getY() + i].setType(MY_TANK);
        }
    }
    if(isHoriz()){
        for(int i = -1; i < 2; map[position.getX() - 1][position.getY() + i].setSymbol('*'), map[position.getX() + 1][position.getY() + i].setSymbol('*'), ++i);
        map[position.getX() + tmpT.getX()][position.getY() + tmpT.getY()].setSymbol('-');
        map[position.getX() - tmpT.getX()][position.getY() - tmpT.getY()].setSymbol('|');
    } else {
        for(int i = -1; i < 2; map[position.getX() + i][position.getY() - 1].setSymbol('*'), map[position.getX() + i][position.getY() + 1].setSymbol('*'), ++i);
        map[position.getX() + tmpT.getX()][position.getY() + tmpT.getY()].setSymbol('|');
        map[position.getX() - tmpT.getX()][position.getY() - tmpT.getY()].setSymbol('-');
    }
    map[position.getX()][position.getY()].setSymbol('+');
}
void MyTank::move(){
    char input[5] = {'w','s','a','d','q'};
    char button;
    button = getch();
    for(int i = 0; i < 4; ++i){
        if(button == input[i]){
            dir = (Direction)i;
            speed = 1;
            break;
        }
    }
    Pixel tmpT = dirVector[dir];
    MyTank old = *this;

    if(dir != dirOld){
    } else {
        if(speed == 1){
            position.setXY(position + tmpT);
            if(!m_game->isCross(area, getArea()) ||
               (m_game->crossTank(getArea(), this) != nullptr) ||
               (m_game->crossBarrier(getArea()) != nullptr)){
                *this = old;
            }
        }
    }

    dirOld = dir;
    if(button == 32){
        m_game->getBullet().setBullet(position + tmpT + tmpT, dir);
    }
    speed = 0;
    if(button == 'q'){
        m_game->setActiveGame(false);
    }
}
void EnemyTank::setStartProof() {
    if(m_game->getScore() < 80){
        proof = 3;
    } else if(m_game->getScore() < 200){
        proof = 9;
    } else {
        proof = 15;
    }
}
void EnemyTank::setToStart() {
    do{
        position.setXY(2+rand()%40, 2+rand()%40);
    }while(m_game->crossTank(getArea(), this) != nullptr || m_game->crossBarrier(getArea()) != nullptr || !m_game->isCross(area, getArea()));
}
void EnemyTank::show(Pixel**& map){
    Pixel tmpT = dirVector[dir];
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            map[position.getX() + i][position.getY() + j].setType(ENEMY_TANK);
        }
    }
    if(isHoriz()){
        for(int i = -1; i < 2; map[position.getX() - 1][position.getY() + i].setSymbol('*'), map[position.getX() + 1][position.getY() + i].setSymbol('*'), ++i);
        map[position.getX() + tmpT.getX()][position.getY() + tmpT.getY()].setSymbol('-');
        map[position.getX() - tmpT.getX()][position.getY() - tmpT.getY()].setSymbol('|');
    } else {
        for(int i = -1; i < 2; map[position.getX() + i][position.getY() - 1].setSymbol('*'), map[position.getX() + i][position.getY() + 1].setSymbol('*'), ++i);
        map[position.getX() + tmpT.getX()][position.getY() + tmpT.getY()].setSymbol('|');
        map[position.getX() - tmpT.getX()][position.getY() - tmpT.getY()].setSymbol('-');
    }
    map[position.getX()][position.getY()].setSymbol('+');
}
void EnemyTank::move(){
    int random = 0 + rand() % 4;
    EnemyTank old = *this;
    Pixel tmpT = dirVector[dir];
    position.setXY(position + tmpT);
    if(!m_game->isCross(area, getArea()) ||
       (m_game->crossTank(getArea(), this) != nullptr) ||
       (m_game->crossBarrier(getArea()) != nullptr)){
        *this = old;
        dir = (Direction)random;
    }
    if(m_game->crossFireMyTank(this)){
        m_game->getBullet().setBullet(position + tmpT + tmpT, dir);
    }
}

enum BarrierType{
    GLASS_WALL,
    CONCRETE_WALL,
    WOODEN_WALL
};

class Barrier{
private:
    Area area;
    bool use;
    BarrierType type = CONCRETE_WALL;
    int proof = 9;
public:
    Barrier()=default;
    Barrier(int px, int py){
        use = false;
        type = CONCRETE_WALL;
        setPos(px, py);
    }
    bool isUsed();
    void show(Pixel**& map);
    void setPos(int px, int py);
    Area getArea();
    void setType(BarrierType t);
    BarrierType getType() const;
    void setUsed(bool u);
    int getProof() const;
    void setProof(int p);
};

Direction Bullet::getDirection() const{
    return dir;
}
void Bullet::setDirection(Direction d){
    dir = d;
}

void Bullet::setType(BulletType t){
    type = t;
}
BulletType Bullet::getType() const{
    return type;
}
void Bullet::setGame(Game* g){
    m_game = g;
}
void Bullet::move(){
    if(!use) return;
    position = position + dirVector[dir];
    Area a = {position.getX(), position.getY(),position.getX(), position.getY()};
    if(!m_game->isCross(a, areaBullet)){
        use = false;
    }
    Barrier* barrier = m_game->crossBarrier(a);
    if(barrier){
        use = false;
        if(getType() == MACHINE_GUN){
            barrier->setProof(barrier->getProof() - 1);
        } else if(getType() == ROCKET){
            barrier->setProof(barrier->getProof() - 3);
        } else {
            barrier->setProof(barrier->getProof() - 500);
        }
        if(barrier->getProof() <= 0){
            barrier->setUsed(false);
            m_game->setCountOfBarriersNow(m_game->getCountOfBarriersNow() - 1);
        }
    }
    Tank* tank = m_game->crossTank(a, nullptr);
    if(tank){
        use = false;
        if(getType() == MACHINE_GUN){
            tank->setProof(tank->getProof() - 1);
        } else if(getType() == ROCKET){
            tank->setProof(tank->getProof() - 3);
        } else {
            tank->setProof(tank->getProof() - 500);
        }
        if(tank->getProof() <= 0){
            if(!m_game->isCross(tank->getArea(), m_game->getMyTank()->getArea())){
                m_game->setScore(m_game->getScore() + 10);
                m_game->setCountOfTanksNow(m_game->getCountOfTanksNow() + 1);
            } else {
                m_game->getMyTank()->setCountOfLife(m_game->getMyTank()->getCountOfLife() - 1);
            }
            tank->setStartProof();
            tank->setToStart();
        }
    }
}
bool Bullet::isUsed(){
    return use;
}
void Bullet::setBullet(Pixel pos, Direction d) {
    if(m_game->getScore() < 100){
        setType(MACHINE_GUN);
    } else if(m_game->getScore() < 300){
        setType(ROCKET);
    } else {
        setType(LASER);
    }
    position.setXY(pos);
    dir = d;
    use = true;
}
void Bullet::show(Pixel**& map){
    if(!use){
        return;
    }
    map[position.getX()][position.getY()].setType(GUN);
    if(getType() == MACHINE_GUN){
        map[position.getX()][position.getY()].setSymbol('*');
    } else if(getType() == ROCKET){
        map[position.getX()][position.getY()].setSymbol('r');
    } else {
        if(getDirection() == LEFT || getDirection() == RIGHT){
            map[position.getX()][position.getY()].setSymbol('-');
        } else {
            map[position.getX()][position.getY()].setSymbol('|');
        }
    }
}

void Barrier::setType(BarrierType t){
    type = t;
    if(type == CONCRETE_WALL){
        setProof(6);
    } else if(type == WOODEN_WALL){
        setProof(3);
    } else if(type == GLASS_WALL){
        setProof(1);
    }
}
BarrierType Barrier::getType() const{
    return type;
}
void Barrier::setUsed(bool u){
    use = u;
}
bool Barrier::isUsed(){
    return use;
}
int Barrier::getProof() const{
    return proof;
}
void Barrier::setProof(int p){
    proof = p;
}
void Barrier::show(Pixel**& map){
    if(!use){return;}
    for(int i = area.getTop(); i <= area.getBottom(); ++i){
        for(int j = area.getLeft(); j <= area.getRight(); ++j){
            if(type == CONCRETE_WALL){
                map[i][j].setType(CONCRETE);
                map[i][j].setSymbol('#');
            } else if(type == GLASS_WALL) {
                map[i][j].setType(GLASS);
                map[i][j].setSymbol('G');
            } else if(type == WOODEN_WALL){
                map[i][j].setType(WOOD);
                map[i][j].setSymbol('W');
            }
        }
    }
}
void Barrier::setPos(int px, int py){
    Area a = {px-1, py-1, px+1, py+1};
    area = a;
    use = true;
}
Area Barrier::getArea(){return area;}

MyTank* Game::getMyTank(){
    return myTank;
}

Map* Game::getMap(){
    return newMap;
}
void Game::setActiveGame(bool b){
    isActiveGame = b;
}
bool Game::getActiveGame(){
    return isActiveGame;
}
void Game::setScore(int s){
    score = s;
}
int Game::getScore() const{
    return score;
}
vector<pair<char*,int>> Game::getHighScoreMap(){
    return highScoreMap;
}

void Game::setHighScoreMap(vector<pair<char*,int>>& newScoreMap){
    highScoreMap.clear();
    for(auto i : newScoreMap){
        highScoreMap.push_back(i);
    }
}
void Game::printInformation() const{
    printw("Score: %d\n", getScore());
    printw("Armor: %d\n", myTank->getProof());
    printw("Count of lifes: %d\n", myTank->getCountOfLife());
    printw("Barrier count: %d\n", getCountOfBarriersNow());
    printw("Dead enemytank count: %d\n", getCountOfTanksNow());
    printw("\n");
}
int Game::getCountOfBarriersNow() const{
    return countOfBarriersNow;
}
void Game::setCountOfBarriersNow(int c){
    countOfBarriersNow = c;
}
void Game::update(){
    //tankArray[0]->move();
    myTank->move();
    if(speed % 200 == 0){
        for(int i = 0; i < countOfBullet; bulletArray[i++].move());
    }
    if(speed % 400 == 0){
        for(int i = 1; i < tankArray.size(); tankArray[i++]->move());
    }
    speed++;
}
void Game::render(){
    newMap->clearMap();
    for(int i = 0; i < countOfBullet; bulletArray[i++].show(newMap->getMap()));
    for(auto i : barrierArray){
        i->show(newMap->getMap());
    }
    for(auto i : tankArray){
        i->show(newMap->getMap());
    }
    newMap->showMap();
    printInformation();
}
bool Game::isCross(Area a, Area b){
    return (a.getRight() >= b.getLeft()) && (a.getLeft() <= b.getRight())
           && (a.getBottom() >= b.getTop()) && (a.getTop() <= b.getBottom());
}
Tank* Game::crossTank(Area a, Tank *except){
    for(auto i : tankArray){
        if(except != i &&
           (isCross(a, i->getArea()))){
            return i;
        }
    }
    return nullptr;
}

bool Game::crossFireMyTank(Tank* tank){
    Area* a;
    Direction tmpD;
    if(tank->getDir() == LEFT){
        a = new Area(tank->getPosition().getX(), 2, tank->getPosition().getX(), tank->getPosition().getY() - 1);
        if(isCross(*a, myTank->getArea())) {
            tmpD = LEFT;
            delete a;
        } else{
            delete a;
            return false;
        }
    } else if(tank->getDir() == RIGHT){
        a = new Area(tank->getPosition().getX(), tank->getPosition().getY() + 1, tank->getPosition().getX(), 42);
        if(isCross(*a, myTank->getArea())) {
            tmpD = RIGHT;
            delete a;
        } else{
            delete a;
            return false;
        }
    } else if(tank->getDir() == UP){
        a = new Area(2, tank->getPosition().getY(), tank->getPosition().getX() - 1, tank->getPosition().getY());
        if(isCross(*a, myTank->getArea())) {
            tmpD = UP;
            delete a;
        } else{
            delete a;
            return false;
        }
    } else {
        a = new Area(tank->getPosition().getX() + 1, tank->getPosition().getY(), 42, tank->getPosition().getY());
        if(isCross(*a, myTank->getArea())) {
            tmpD = DOWN;
            delete a;
        } else{
            delete a;
            return false;
        }
    }
    if(tmpD == LEFT){
        a = new Area(myTank->getPosition().getX(), myTank->getPosition().getY() + 2, tank->getPosition().getX(), tank->getPosition().getY() - 2);
    } else if(tmpD == RIGHT){
        a = new Area(tank->getPosition().getX(), tank->getPosition().getY() + 2, myTank->getPosition().getX(), myTank->getPosition().getY() - 2);
    } else if(tmpD == UP){
        a = new Area(myTank->getPosition().getX() + 2, myTank->getPosition().getY(), tank->getPosition().getX() - 2, tank->getPosition().getY());
    } else {
        a = new Area(tank->getPosition().getX() + 2, tank->getPosition().getY(), myTank->getPosition().getY() - 2, myTank->getPosition().getY());
    }
    for(auto i : barrierArray){
        if(isCross(*a, i->getArea()) && i->getType() != GLASS_WALL){
            delete a;
            return false;
        }
    }
    for(auto i : tankArray){
        if(isCross(*a, i->getArea())){
            delete a;
            return false;
        }
    }
    delete a;
    return true;
}

Barrier* Game::crossBarrier(Area a){
    for(auto i : barrierArray){
        if(i->isUsed() &&
           isCross(a, i->getArea())){
            return i;
        }
    }
    return nullptr;
}

void Game::createTanks(int count){
    tankArray.push_back(new MyTank(42,22, this));
    myTank = dynamic_cast<MyTank*>(*(tankArray.begin() + 0));
    tankArray.push_back(new EnemyTank(2,9, this));
    tankArray.push_back(new EnemyTank(2,13, this));
    // tankArray.push_back(new EnemyTank(2,17, this));
    // tankArray.push_back(new EnemyTank(2,21, this));
    // tankArray.push_back(new EnemyTank(2,24, this));
    // tankArray.push_back(new EnemyTank(2,27, this));
    // tankArray.push_back(new EnemyTank(2,31, this));
    // tankArray.push_back(new EnemyTank(2,35, this));
}
void Game::createBarriers(int count){
    countOfBarriersNow = count;
    int random = 0 + rand()%3;
    if(random == 0){
        //FIT
        barrierArray.push_back(new Barrier(7,10));
        barrierArray.push_back(new Barrier(7,13));
        barrierArray.push_back(new Barrier(7,16));
        barrierArray.push_back(new Barrier(7,22));
        barrierArray.push_back(new Barrier(7,28));
        barrierArray.push_back(new Barrier(7,31));
        barrierArray.push_back(new Barrier(7,34));

        barrierArray.push_back(new Barrier(10,10));
        barrierArray.push_back(new Barrier(10,22));
        barrierArray.push_back(new Barrier(10,31));

        barrierArray.push_back(new Barrier(13,10));
        barrierArray.push_back(new Barrier(13,13));
        barrierArray.push_back(new Barrier(13,22));
        barrierArray.push_back(new Barrier(13,31));

        barrierArray.push_back(new Barrier(16,10));
        barrierArray.push_back(new Barrier(16,22));
        barrierArray.push_back(new Barrier(16,31));

        barrierArray.push_back(new Barrier(19,10));
        barrierArray.push_back(new Barrier(19,22));
        barrierArray.push_back(new Barrier(19,31));
    } else if(random == 1){
        //FEL
        barrierArray.push_back(new Barrier(7,7));
        barrierArray.push_back(new Barrier(7,10));
        barrierArray.push_back(new Barrier(7,13));
        barrierArray.push_back(new Barrier(7,19));
        barrierArray.push_back(new Barrier(7,22));
        barrierArray.push_back(new Barrier(7,25));
        barrierArray.push_back(new Barrier(7,31));

        barrierArray.push_back(new Barrier(10,7));
        barrierArray.push_back(new Barrier(10,19));
        barrierArray.push_back(new Barrier(10,31));

        barrierArray.push_back(new Barrier(13,7));
        barrierArray.push_back(new Barrier(13,10));
        barrierArray.push_back(new Barrier(13,19));
        barrierArray.push_back(new Barrier(13,22));
        barrierArray.push_back(new Barrier(13,31));

        barrierArray.push_back(new Barrier(16,7));
        barrierArray.push_back(new Barrier(16,19));
        barrierArray.push_back(new Barrier(16,31));
        barrierArray.push_back(new Barrier(16,37));

        barrierArray.push_back(new Barrier(19,7));
        barrierArray.push_back(new Barrier(19,19));
        barrierArray.push_back(new Barrier(19,22));
        barrierArray.push_back(new Barrier(19,25));
        barrierArray.push_back(new Barrier(19,31));
        barrierArray.push_back(new Barrier(19,34));
        barrierArray.push_back(new Barrier(19,37));
    } else if(random == 2){
        //FJFI
        barrierArray.push_back(new Barrier(7,4));
        barrierArray.push_back(new Barrier(7,7));
        barrierArray.push_back(new Barrier(7,10));
        barrierArray.push_back(new Barrier(7,16));
        barrierArray.push_back(new Barrier(7,19));
        barrierArray.push_back(new Barrier(7,22));
        barrierArray.push_back(new Barrier(7,28));
        barrierArray.push_back(new Barrier(7,31));
        barrierArray.push_back(new Barrier(7,34));
        barrierArray.push_back(new Barrier(7,40));

        barrierArray.push_back(new Barrier(10,4));
        barrierArray.push_back(new Barrier(10,16));
        barrierArray.push_back(new Barrier(10,22));
        barrierArray.push_back(new Barrier(10,28));
        barrierArray.push_back(new Barrier(10,40));

        barrierArray.push_back(new Barrier(13,4));
        barrierArray.push_back(new Barrier(13,7));
        barrierArray.push_back(new Barrier(13,22));
        barrierArray.push_back(new Barrier(13,28));
        barrierArray.push_back(new Barrier(13,31));
        barrierArray.push_back(new Barrier(13,40));

        barrierArray.push_back(new Barrier(16,4));
        barrierArray.push_back(new Barrier(16,22));
        barrierArray.push_back(new Barrier(16,28));
        barrierArray.push_back(new Barrier(16,40));

        barrierArray.push_back(new Barrier(19,4));
        barrierArray.push_back(new Barrier(19,16));
        barrierArray.push_back(new Barrier(19,19));
        barrierArray.push_back(new Barrier(19,28));
        barrierArray.push_back(new Barrier(19,40));
    }
    //CTU
    barrierArray.push_back(new Barrier(25,10));
    barrierArray.push_back(new Barrier(25,19));
    barrierArray.push_back(new Barrier(25,22));
    barrierArray.push_back(new Barrier(25,25));
    barrierArray.push_back(new Barrier(25,31));
    barrierArray.push_back(new Barrier(25,37));

    barrierArray.push_back(new Barrier(28,7));
    barrierArray.push_back(new Barrier(28,13));
    barrierArray.push_back(new Barrier(28,22));
    barrierArray.push_back(new Barrier(28,31));
    barrierArray.push_back(new Barrier(28,37));

    barrierArray.push_back(new Barrier(31,7));
    barrierArray.push_back(new Barrier(31,22));
    barrierArray.push_back(new Barrier(31,31));
    barrierArray.push_back(new Barrier(31,37));

    barrierArray.push_back(new Barrier(34,7));
    barrierArray.push_back(new Barrier(34,13));
    barrierArray.push_back(new Barrier(34,22));
    barrierArray.push_back(new Barrier(34,31));
    barrierArray.push_back(new Barrier(34,37));

    barrierArray.push_back(new Barrier(37,10));
    barrierArray.push_back(new Barrier(37,22));
    barrierArray.push_back(new Barrier(37,34));
    for(auto i : barrierArray){
        i->setType(BarrierType(0+rand()%3));
        //i->setType(GLASS_WALL);
    }
}
void Game::createBullet(int count){
    countOfBullet = count;
    bulletArray = new Bullet[count];
    for(int i = 0; i < countOfBullet; i++){
        bulletArray[i].setGame(this);
    }

}

Bullet& Game::getBullet(){
    for(int i = 0; i < countOfBullet; ++i){
        if(!bulletArray[i].isUsed()) return bulletArray[i];
    }
    return bulletArray[0];
}

class GameMenu{
private:
    Game* m_game;
    const char menuPageOne[3][11] = {
            "New Game",
            "High Score",
            "End"
    };
    const char menuPageTwo[3][21] = {
            "Generate battlefield",
            "Load from file",
            "Back"
    };
    unsigned choice = 0;
    bool firstPage = true;
    bool secondPage = true;
    char name[9];
public:
    GameMenu(){
    }
    Game* start(){
        while ( firstPage ){
            secondPage = true;
            clear();
            for ( unsigned i = 0; i < 3; i++ ){
                if ( i == choice )
                    addch('>');
                else
                    addch(' ');
                printw("%s\n", menuPageOne[i]);
            }
            switch ( getch() ) {
                case 'w':
                    if (choice)
                        choice--;
                    break;
                case 's':
                    if (choice != 2)
                        choice++;
                    break;
                case 32:
                    switch (choice) {
                        case 0: {
                            clear();
                            char in = 'k';
                            int j = 0;
                            printw("Write your name: ");
                            while (in != '\n' && j != 8) {
                                in = getch();
                                name[j] = in;
                                j++;
                            }
                            name[j] = '\0';
                            while (secondPage) {
                                clear();
                                for (unsigned i = 0; i < 3; i++) {
                                    if (i == choice)
                                        addch('>');
                                    else
                                        addch(' ');

                                    printw("%s\n", menuPageTwo[i]);
                                }
                                switch (getch()) {
                                    case 'w':
                                        if (choice)
                                            choice--;
                                        break;
                                    case 's':
                                        if (choice != 2)
                                            choice++;
                                        break;
                                    case 32:
                                        secondPage = false;
                                        switch (choice) {
                                            case 0:
                                                m_game = new Game();
                                                secondPage = false;
                                                firstPage = false;
                                                return m_game;
                                                break;
                                            case 1:
                                                m_game = new Game();
                                                secondPage = false;
                                                firstPage = false;
                                                return m_game;
                                                break;
                                            case 2:
                                                secondPage = false;
                                                break;
                                        }
                                }
                            }
                            break;
                        }
                        case 1: {
                            clear();
                            printw("\nScore: %s", name);
                            getch();
                            break;
                        }
                        case 2: {
                            firstPage = false;
                            break;
                        }
                    }
            }
        }
        return m_game;
    }
};

int main(){
    srand(time(NULL));
    initscr();
    curs_set(0);
    char k = 'p';
    GameMenu* newMenu = new GameMenu();
    Game* newGame = newMenu->start();
    if(newGame == nullptr){
        endwin();
        return 0;
    }
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_CYAN, COLOR_CYAN);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    do {
        move(0, 0);
        newGame->update();
        newGame->render();
        if(!newGame->getMyTank()->isAlive()){
            do{
                move(50,0);
                printw("If you want to try again, please enter 'y' otherwise enter 'n'\n");
                refresh();
                k = getch();
            }while(k != 'n' && k != 'y');
            if(k == 'n'){
                newGame->setActiveGame(false);
            } else {
                newGame = new Game();
            }
        }
    } while (newGame->getActiveGame());
    endwin();
    return 0;
}
