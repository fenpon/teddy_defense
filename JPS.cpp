
#include <algorithm>
#include <cstring>

#include <iostream>
#include <fstream>
#include <queue>
#include < string>
#include <time.h>


using namespace std;

const int LIMIT = 10000;
int d[LIMIT][LIMIT];
int jump[LIMIT][LIMIT];
int a[LIMIT][LIMIT];
int n, m;
int dx[8] = { 0,1,0,-1 , 1,1 ,-1,-1};//아래 오른쪽 위로 왼쪽 오아래 오위 좌위 좌아래
int dy[8] = { 1,0,-1,0 , 1,-1,-1,1};//인덱스 4부터 대각선
int des_x;
int des_y;

// https://zerowidth.com/2013/a-visual-explanation-of-jump-point-search.html

int jump_chk[8][4][2] = { 
	{ {-1,0},{1,0},{-1,1},{1,1} },
	{ {0,-1},{0,1},{1,-1},{1,1} },
	{ {-1,0},{1,0},{-1,-1},{1,-1} },
	{ {0,-1},{0,1},{-1,-1},{-1,1} },

	{ {0,-1},{-1,0},{1,-1},{-1,1}},
	{ {-1,0},{0,1},{-1,-1},{1,1} },
	
	{ {0,1},{1,0},{-1,1},{1,-1}},
	{ {0,-1},{1,0},{-1,-1},{1,1}}
};//방향틀릴수있다
//02 13
struct D{
	int x;
	int y;
	float w;
	int dir;
};
struct comp {
	bool operator()(D a, D b)
	{
		int ra = des_x - a.x + des_y - a.y;
		int rb = des_x - b.x + des_y - b.y;
		//cout << "comp : " << a.x <<":"<<a.y <<":"<<" == " << b.x << ":"<<b.y << ":"<<ra << ":"<<rb<< '\n';
		return a.w > b.w; //멘헤튼 퓨리스틱
	}
};
bool check_des(int x, int y)
{
	return (x == des_x && y == des_y);
}
bool able(int x,int y,bool chk_jump_point = false)
{
	if (chk_jump_point)
	{
		if (a[y][x] == 0 && d[y][x] == 0)
		{
			return true;
		}
	}
	else
	{
		if (a[y][x] == 0 && d[y][x] != 1)
		{
			return true;
		}
	}
	
	return false;
}
bool check_range(int x, int y)
{
	//cout << " find : " << x << ":" << y << ":" << n << ":" << n << '\n';
	return (x >= 0 && x < n&& y >= 0 && y < n);


}
bool chk_Jump_point(int x, int y, int dir)
{
	bool ans = false;
	if (able(x,y))
	{

		int cc[2] = {0,0};
		for (int i = 0; i < 4; ++i)
		{
			int nx = x+jump_chk[dir][i][0];
			int ny = y+jump_chk[dir][i][1];
			
			if (check_range(nx, ny))
			{
				
				//cout <<i << " { "<< x << ":" << y << " :  {" << dir << "} : " << nx << ":" << ny << " - " << i % 2 << " = " << a[ny][nx] << '\n';
				//cc[i % 2]++;
				if (i <= 1)
				{
					if (a[ny][nx] == 1)
					{
						cc[i % 2]++;
					}
				}
				else
				{
					if (a[ny][nx] == 0)
					{
						cc[i % 2]++;
					}
				}
				
			}
			else
			{
				//cc[i % 2]++;
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			//cout << "C:" << cc[i] << '\n';
			if (cc[i] == 2)
			{
				
				ans = true;
				break;
			}
		}
	}

	return ans;
}

void print(bool isjps = true)
{
	cout << '\n';
	for (int y = 0; y < n; ++y)
	{
		
		for (int x = 0; x < n; ++x)
		{
			if (isjps)
			{
				if (jump[y][x] == 1)
				{
					cout << "J";
				}
				else
				{
					cout << d[y][x];
				}
			}
			else
			{
				cout << d[y][x];
			}
			
		}
		cout << '\n';
	}
}
int c = 1;
double jps_t = 0;
double A_t = 0;
int JPS(bool is_print)
{
	int Grid_Search_T = 0;
	int Search_T = 0;
	memset(d, 0, sizeof(d));
	memset(jump, 0, sizeof(jump));
	c = 0;
	priority_queue<D, vector<D>, comp> q;
	q.push({ 0,0 ,0,9 });
	bool is_fin = false;
	clock_t s = clock();
	d[0][0] = 1;
	jump[0][0] = 1;
	Search_T = 1;
	//cout << n <<'\n';
	while (q.size() > 0)
	{
		//cout << "------" << '\n';
		D now = q.top();
		c++;
		q.pop();
		
		
		
			
		
			
			//cout << " Now : " << now.x << ":" << now.y << ":" << now.dir << '\n';
			//cout << "now:" << f_now.x << ":" << f_now.y <<":"<< fq.size() << '\n';
			
			//cout << "start \n";
			//점프포인트 가능한지 확인하는 로직 필요
			bool t = false;
			if (check_des(now.x, now.y))
			{
				cout << "JPS FIN" << '\n';
				is_fin = true;
				break;
			}
			
			
			
			
			if (now.dir < 4)
			{

				int nx = now.x + dx[now.dir];
				int ny = now.y + dy[now.dir];
				if (check_range(nx, ny))
				{
					if (able(nx, ny))
					{
						if (chk_Jump_point(nx, ny, now.dir))
						{
							Search_T++;
							jump[ny][nx] = 1;
							q.push({ nx,ny,0,4});
						}
						else
						{
							Grid_Search_T++;
							q.push({ nx,ny,now.w,now.dir });//직선쭉
						}
						d[ny][nx] = 1;
						
					}

				}
			}
			else
			{
				//대각선

				for (int i = 0; i < 8; ++i)
				{
					int nx = now.x + dx[i];
					int ny = now.y + dy[i];

					if (check_range(nx, ny))
					{

						if (able(nx, ny))
						{
							
							
							if (i < 4)
							{
								if (chk_Jump_point(nx, ny, now.dir))
								{
									Search_T++;
									
									jump[ny][nx] = 1;
									
									q.push({ nx,ny,0,4 });//점프 포인트 최우선 계싼
								}
								else
								{
									Grid_Search_T++;
									q.push({ nx,ny, now.w + 1,i });
								}
								d[ny][nx] = 1;
							}
							else
							{
								Grid_Search_T++;
								q.push({ nx,ny,now.w + 1.2f,i });
								d[ny][nx] = 1;
							}
							
						}


					}
				}

			}

			

			if (is_print)
				print();


		
		//d[now.y][now.x] = 2;
		if (is_fin)
		{
			break;
		}
		//cout << "------- fin ------ \n";
	}
	clock_t end = clock();
	double ttt = (double)(end - s);
	jps_t = ttt;
	cout << "전체 JPS : " << c <<" 실행 시간 : "<< ttt << '\n';
	cout << "JPS MTime : " << Search_T << " 실행 시간 GTime : " << Grid_Search_T << '\n'; // -> 그리드 탐색을 줄이면 성능이 엄청나게 늘어난다 JPS B+등의 로직이 있다 연구중입니다.
	 
	return c;
}
int ASTAR(bool is_print)
{
	int Grid_Search_T = 0;
	int Search_T = 0;

	memset(d, 0, sizeof(d));
	memset(jump, 0, sizeof(jump));
	c = 0;
	priority_queue<D, vector<D>, comp> q;
	q.push({ 0,0 ,0,0 });
	bool is_fin = false;
	clock_t s = clock();
	d[0][0] = 1;
	while (q.size() > 0)
	{
		
		D now = q.top();
		q.pop();
		c++;
		Search_T++;

		if (check_des(now.x, now.y))
		{
			cout << "A* FIN" << '\n';
			break;
		}
		for (int i = 0; i < 8; ++i)
		{
			int nx = now.x + dx[i];
			int ny = now.y + dy[i];

			if (check_range(nx, ny))
			{

				if (able(nx, ny))
				{
					//Grid_Search_T++;
					
					d[ny][nx] = 1;
					if (i < 4)
					{
						q.push({ nx,ny,now.w + 1.2f,i });
					}
					else
					{
						q.push({ nx,ny,now.w + 1,0 });
					}
					
				}


			}
		}
		if (is_print)
			print(false);
	}
	clock_t end = clock();
	double ttt = (double)(end - s);

	A_t = ttt;
	cout << "전체 카운터 A* : " << c <<" : "<<ttt<< '\n';
	
	return c;
}
int main()
{
	//cin >> n;

	//cout << "N:" << n << '\n';
	memset(a, 0, sizeof(a));
	string filePath = "C:/data99.txt"; //데이터 샘플 경로
	ifstream openFile(filePath.data());
	int y = -1;
	string ln;
	if (openFile.is_open()) {
		
		string line;
		while (getline(openFile, line)) {
			ln =  line;
			//cout << y << ":"<< line << endl;
			if (y == -1)
			{
				n =	stoi(line);
			
			}
			else
			{
				for (int x = 0; x < n; ++x)
				{
					a[y][x] = line[x] - '0';
				}
			}
			
			y++;
		}
		
		openFile.close();
	}
	des_x = n - 1; 
	des_y = n - 1;
	cout << "Start" << '\n';


	int JPS_C = JPS(0);//jps특성상 목적지와의 거리가 멀어질수록 A*별대비 훨씬 빨라진다 -> 일반적인 JPS라 A*대비 큰 성능 차이를 체감하긴 표본에 따라 달라집니다.
	//-> 그리드 탐색과 점프포인트 연산의 횟수 비율이 9:1  이라 그리드 탐색을 줄이면 엄청나게 빨라진다고 합니다. 
	// 	   https://www.youtube.com/watch?v=rfOgaPXCADQ&ab_channel=G.Physicsl%EC%A7%80%ED%94%BC%EC%A7%81%EC%8A%A4
	//print();
	int A_start_C = ASTAR(0);
	//print();
	cout << '\n';
	cout << "루프 횟수 비교 A* : " << A_start_C << " JPS : " << JPS_C << '\n';
	cout << "실행시간 비교 A* : " << A_t << " JPS : " << jps_t << '\n';
	return 0;
}