
#include <algorithm>
#include <cstring>

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include < string>
#include <time.h>


using namespace std;

const int LIMIT = 10000;
int d[LIMIT][LIMIT];
int jump[LIMIT][LIMIT];
int a[LIMIT][LIMIT];
int n, m;
int dx[8] = { 0,1,0,-1 , 1,1 ,-1,-1 };//아래 오른쪽 위로 왼쪽 오아래 오위 좌위 좌아래
int dy[8] = { 1,0,-1,0 , 1,-1,-1,1 };//인덱스 4부터 대각선
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
struct D {
	int x;
	int y;
	float w;
	int dir;
};
struct comp {
	bool operator()(D a, D b)
	{

			int ra = abs((des_x - a.x) + (des_y - a.y));
			int rb = abs((des_x - b.x) + (des_y - b.y));

			return ra > rb;
	}
};
bool check_des(int x, int y)
{
	return (x == des_x && y == des_y);
}
bool able(int x, int y, bool chk_jump_point = false)
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
	if (dir >= 4)
	{
		return false;
	}
	if (able(x, y))
	{

		int cc[2] = { 0,0 };
		for (int i = 0; i < 4; ++i)
		{
			int nx = x + jump_chk[dir][i][0];
			int ny = y + jump_chk[dir][i][1];

			if (check_range(nx, ny))
			{

				//cout <<i << " { "<< x << ":" << y << " :  {" << dir << "} : " << nx << ":" << ny << " - " << i % 2 << " = " << a[ny][nx] << " = " << (int)(i / 2) << '\n';
				//cc[i % 2]++;
				if ((int)(i/2) == 1)
				{
					if (a[ny][nx] == 0)
					{
						cc[i % 2]++;
					}
				}
				else
				{
					if (a[ny][nx] == 1)
					{
						cc[i % 2]++;
					}
				}

			}
			else
			{
				if ((int)(i / 2) == 1)
				{
					
						cc[i % 2]++;
					
				}
				//cc[i % 2]++;
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			//cout << "C:" << cc[i] << '\n';
			if (cc[i] == 2)
			{

				ans = true;
				
			}
			else if (cc[i] == 0)
			{
				ans = false;
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
	cout << "JPS 실행중 ...." << '\n';
	int Grid_Search_T = 0;
	int Search_T = 0;
	memset(d, 0, sizeof(d));
	memset(jump, 0, sizeof(jump));
	c = 0;
	priority_queue<D, vector<D>, comp> q;
	q.push({ 0,0 ,0,4 });
	bool is_fin = false;
	clock_t s = clock();
	d[0][0] = 1;
	jump[0][0] = 1;
	Search_T = 1;
	//cout << n <<'\n';
	while (q.size() > 0)
	{
		//cout << "------" << '\n';
		//여기가 전체 루프 횟수의 10프로 비율 
		//점프 포인트 부분
		D now = q.top();
		
		q.pop();


		jump[now.y][now.x] = 1;



		//cout << " jump : " << now.x << ":" << now.y << ":" << now.dir << '\n';
		//cout << "now:" << f_now.x << ":" << f_now.y <<":"<< fq.size() << '\n';

		//cout << "start \n";
		//점프포인트 가능한지 확인하는 로직 필요
		bool t = false;
		

		stack<D> qq;
		
		qq.push({ now.x,now.y ,0,4 });

		while (qq.size() > 0)//jump 포인트를 찾는과정이다 A*보단 확실히 빠르다 단 루프 총횟수는 많다 
		{
			//일반 A*은 모든 노드에서 퓨리스틱과 함꼐 힙등의 우선순위 큐가 작동하므로 할때마다 logn의 시간복잡도가 드는데 여긴 그런게 없다
			//여기가 전체 루프 횟수의 90프로 비율
			c++;
			D f_now = qq.top();
			qq.pop();
			
			//cout << " Now : " << f_now.x << ":" << f_now.y << ":" << f_now.dir<<":"<< qq.size() << '\n';
			if (check_des(f_now.x, f_now.y))
			{
				cout << "JPS FIN" << '\n';
				is_fin = true;
				break;
			}
			if (chk_Jump_point(f_now.x, f_now.y, f_now.dir))
			{
				//cout << "점프 : " << f_now.x << ":" << f_now.y << " - " << f_now.dir << "\n";
				Search_T++;
				jump[f_now.y][f_now.x] = 1;
				q.push({ f_now.x, f_now.y,0,4 });


			}
			d[f_now.y][f_now.x] = 1;
			if (f_now.dir < 4)
			{

				int nx = f_now.x + dx[f_now.dir];
				int ny = f_now.y + dy[f_now.dir];
				if (check_range(nx, ny))
				{
					if (able(nx, ny))
					{
						
					
						
							Grid_Search_T++;
							qq.push({ nx,ny, f_now.w ,f_now.dir });//직선쭉

						
						

					}
					

				}
			}
			else
			{
				for (int i = 7; i >= 0; --i)
				{
					int nx = f_now.x + dx[i];
					int ny = f_now.y + dy[i];
					//cout << " next : " << nx << ":" << ny << ":" << i << ":" << qq.size() << '\n';
					if (check_range(nx, ny))
					{
						if (able(nx, ny)) {
						
								Grid_Search_T++;
								qq.push({ nx,ny,0,i });
							
							
						}
					}
				}
			}
			

			
			if (is_print)
				print();
		}

		



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
	cout << "전체 JPS : " << c << " 실행 시간 : " << ttt << '\n';
	cout << "JPS MTime : " << Search_T << " 실행 시간 GTime : " << Grid_Search_T << '\n'; // -> 그리드 탐색을 줄이면 성능이 엄청나게 늘어난다 JPS B+등의 로직이 있다 연구중입니다.

	return c;
}
int ASTAR(bool is_print)
{
	cout << "A* 실행중 ...." << '\n';
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
	cout << "전체 카운터 A* : " << c << " : " << ttt << '\n';

	return c;
}
int main()
{
	//cin >> n;

	//cout << "N:" << n << '\n';
	memset(a, 0, sizeof(a));
	string filePath = "C:/data4999.txt"; //데이터 샘플 경로
	ifstream openFile(filePath.data());
	int y = -1;
	string ln;
	cout << " 데이터 읽어 오는중 ..." << '\n';
	if (openFile.is_open()) {

		string line;
		while (getline(openFile, line)) {
			ln = line;
			//cout << y << ":"<< line << endl;
			if (y == -1)
			{
				n = stoi(line);

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


	int JPS_C = JPS(0);//jps특성상 목적지와의 거리가 멀어질수록 A*별대비 훨씬 빨라진다 -> 일반적인 JPS라 A*대비 큰 성능 차이를 체감하긴 표본에 따라 달라집니다. 그래도 차이가 무시못할정도로 납니다.
	//-> 그리드 탐색과 점프포인트 연산의 횟수 비율이 9:1  이라 그리드 탐색을 줄이면 엄청나게 빨라진다고 합니다. 
	// 	   https://www.youtube.com/watch?v=rfOgaPXCADQ&ab_channel=G.Physicsl%EC%A7%80%ED%94%BC%EC%A7%81%EC%8A%A4
	//print();
	int A_start_C = ASTAR(0);
	//print();
	cout << '\n';
	cout << "루프 횟수 비교 A* : " << A_start_C << " JPS : " << JPS_C << '\n';
	cout << "실행시간 비교 A* : " << A_t << " JPS : " << jps_t << '\n';//실행시간을 비교해주세요 우선순위큐를 stl써서 카운트를 넣지 못해서 내부적으로 작동하는 시간과 비교해주세요
	return 0;
}