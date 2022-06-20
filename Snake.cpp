#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<curses.h>//getch()函数 需要在编译的时候加上-lncurses
//游戏的一些参数设置
enum {boundary=50,winmark=10};
struct  node//将蛇的身体当成图中的点组成
{
    int index[2];//当前点的位置,第一个是横坐标，第二个是纵坐标
    node * next;//当前蛇身的后一个点的位置
};
unsigned int mark;//分数
bool GAMEOVER = true;//游戏是否结束
bool WIN = false;//游戏是否获胜
enum direction{Up,Left,Right,Down};
//果实
int fruit[2];
//刷新果实
void fresh_fruit(){
    srand((unsigned int)time(NULL));
    fruit[0] = rand()%44+5;
    usleep(10000);
    fruit[1] = rand()%44+5;
}
//蛇
class Snake{
    private:
        node *head;//蛇头的位置
        int num;//蛇的长度
        direction dir=Up;//初始化方向
        // node *tail;//蛇尾
        // int tail_num[2];//蛇尾的坐标
    public:
        Snake();
        bool is_snake(int a[])const;//判断是不是蛇的点
        void Move();//移动
        void turn_head(direction D);//转向
        void tomove();//除了蛇头以外的蛇身移动
        bool to_up();//向上
        bool to_down();//向下
        bool to_left();//向左
        bool to_right();//向右
        void long_snake();//蛇身变长
        ~Snake();
};
Snake::Snake(){
    head=new node;
    head->index[0]=boundary/2;
    head->index[1]=boundary/2;
    head->next=nullptr;
    // tail=&head;
}
Snake::~Snake(){
    node *p=head;
    while(p){
        node *q=p->next;
        delete p;
        p=q;
    }
}
//移动
void Snake::Move(){
    if(dir==Up){
        to_up();
        usleep(100000);
    }
    else if(dir==Down){
        to_down();
        usleep(100000);
    }
    else if(dir==Left){
        to_left();
        usleep(100000);
    }
    else if(dir==Right){
        to_right();
        usleep(100000);
    }
}
void Snake::tomove(){
    node *p= head->next;
    int temp[2]={head->index[0],head->index[1]};
    while(p){
        // if(p->next==nullptr){
        //     tail[0]=p->index[0];
        //     tail[1]=p->index[1];
        // }
        int i[2]={p->index[0],p->index[1]};
        p->index[0]=temp[0];
        p->index[1]=temp[1];
        temp[0]=i[0];
        temp[1]=i[1];
        p=p->next;
        
    }
}
bool Snake::is_snake(int a[])const{
    const node * p= head;
    while(p){
        if(p->index[0]==a[0] && p->index[1]==a[1]){
            return true;
            break;
        }
        p=p->next;
    }
    return false;
}

bool Snake::to_up(){
    if(head->index[0]==fruit[0]&& (head->index[1]-1)==fruit[1] ){
        node *p=new node;
        p->index[0]=fruit[0];
        p->index[1]=fruit[1];
        p->next=head;
        head=p;
        fresh_fruit();
        ++mark;
        if(mark>=winmark){
            WIN=true;
            GAMEOVER=true;
        }
    }
    else{
        tomove();
        --head->index[1];
    }
    if(head->index[1]<=0){
        GAMEOVER=true;
        return false;
    }
    return true;
}
bool Snake::to_down(){
    if(head->index[0]==fruit[0]&& (head->index[1]+1)==fruit[1] ){
        node *p=new node;
        p->index[0]=fruit[0];
        p->index[1]=fruit[1];
        p->next=head;
        head=p;
        fresh_fruit();
        ++mark;
        if(mark>=winmark){
            WIN=true;
            GAMEOVER=true;
        }
    }
    else{
        tomove();
        ++head->index[1];
    }
    if(head->index[1]>=boundary){
        GAMEOVER=true;
        return false;
    }
    return true;
}
bool Snake::to_left(){
    if(head->index[1]==fruit[1]&& (head->index[0]-1)==fruit[0] ){
        node *p=new node;
        p->index[0]=fruit[0];
        p->index[1]=fruit[1];
        p->next=head;
        head=p;
        fresh_fruit();
        ++mark;
        if(mark>=winmark){
            WIN=true;
            GAMEOVER=true;
        }
    }
    else{
        tomove();
        --head->index[0];
    }
    if(head->index[0]<=0){
        GAMEOVER=true;
        return false;
    }
    return true;
}
bool Snake::to_right(){
    if(head->index[1]==fruit[1]&& (head->index[0]+1)==fruit[0] ){
        node *p=new node;
        p->index[0]=fruit[0];
        p->index[1]=fruit[1];
        p->next=head;
        head=p;
        fresh_fruit();
        ++mark;
        if(mark>=winmark){
            WIN=true;
        }
    }
    else{
        tomove();
        ++head->index[0];
    }
    if(head->index[0]>=boundary){
        GAMEOVER=true;
        return false;
    }
    return true;
}

void Snake::turn_head(direction D){
    if((D+dir)==3){
        return;
    }
    dir=D;
}
//************************游戏本体************************************
class Game{
    private:
        char game_interface[boundary][boundary];
        Snake snk;//蛇
    public:
        void Init_game();//初始化
        void operate_snake();//操作蛇
        void Move_snake();//移动蛇
        // bool gameover()const;
        // bool win()const;
        void show()const;
        void refresh_snake();
};
//初始化游戏，初始化界面和蛇
void Game::Init_game(){
    refresh_snake();
    fresh_fruit();
    WIN=false;
    GAMEOVER=false;
}

//操作蛇上下左右
void Game::operate_snake(){
    int ch;
    while(1){
        ch=getch();
        switch (ch)
        {
        case KEY_UP://向上
            snk.turn_head(Up);
            break;
        case KEY_LEFT://向左
            snk.turn_head(Left);
            break;
        case KEY_RIGHT://向右
            snk.turn_head(Right);
            break;
        case KEY_DOWN://向下
            snk.turn_head(Down);
            break;
        }
    }
}
void Game::Move_snake(){
    snk.Move();
}
//刷新蛇的位置
void Game::refresh_snake(){
     for(int i=0; i<boundary; i++){
        if(i==0||i==boundary-1){
            for(int j=0; j<boundary; j++){
                game_interface[i][j]='#';
            }
        }
        else{
            for(int j=0; j<boundary; j++){
                if(j==0||j==boundary-1){
                    game_interface[i][j]='#';
                }
                else{
                    int a[2]={i,j};
                    if(snk.is_snake(a)){
                        game_interface[i][j]='@';
                    }
                    else if(i==fruit[0] && j==fruit[1]){
                        game_interface[i][j]='*';
                    }
                    else{
                        game_interface[i][j]=' ';
                    }
                    
                }
            }
        }
    }
}
//显示游戏
void Game::show()const{
    for(int i=0; i<boundary;++i){
        for(int j=0; j<boundary; ++j){
            mvaddch(j,i,game_interface[i][j]);
            // std::cout << game_interface[i][j] << std::endl;
        }
        std::cout<<std::endl;
    }
    mvprintw(10,60,"mark: %d",mark);
}
//刷新屏幕
void *refres_game(void *abs){
    Game *game = (Game *)(long)abs;
    while(1){
        clear();
        if(WIN){
            mvaddstr(20,20,"Congratulations!");
            break;
        }
        else if(GAMEOVER){
            mvaddstr(20,20,"sorry! gameover");
            break;
        }
        else{
            game->Move_snake();
            game->refresh_snake();
            game->show();
            refresh();
            usleep(100000);
        }
        
    }
    pthread_exit(0);
}
//刷新操作
void *chang_game(void *abs){
    Game *game = (Game *)(long)abs;
    game->operate_snake();
    pthread_exit(0);
}
void initial_curses(){//初始化curses
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    refresh();
}
int main(){
    initial_curses();//开启curses 模式
    Game G;
    G.Init_game();
    pthread_t th1,th2;
    if(pthread_create(&th1, NULL,refres_game,&G)==-1){
        std::cout << "Error: Failed to create thread" << std::endl;
        return -1;
    }
    if(pthread_create(&th2,NULL,chang_game,&G)==-1){
        std::cout << "Error: Failed to create thread" << std::endl;
        return -1;
    }
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    std::cout << "game over" << std::endl;
    endwin();
}
