#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Pattern
{
	string name;
	int num;
};
class PatternArray
{
	Pattern *arr;
	int last;
public:
	PatternArray(int size)
	{
		arr = new Pattern[size];
		last = 0;
	}
	int exist(string _name)
	{
		if (last == 0)
			return -1;
		else
		{
			for (int i = 0; i < last; i++)
			{
				if (arr[i].name == _name) return i;
			}
		}
		return -1;
	}
	void push(string _name)
	{
		int place = exist(_name);
		if (place < 0)
		{
			arr[last].name = _name;
			arr[last].num = 1;
			last++;
		}
		else
		{
			arr[place].num++;
		}
	}
	string findFreqWord()
	{
		if (last == 0) throw "Text is empty";
		else
		{
			int maxnum = 0;
			string s;
			for (int i = 0; i < last; i++)
			{
				if (arr[i].num > maxnum)
				{
					maxnum = arr[i].num;
					s = arr[i].name;
				}
				else if (arr[i].num == maxnum)
				{
					s += " ";
					s += arr[i].name;
				}

			}
			return s;
		}
		return 0;
	}
	~PatternArray()
	{
		delete[] arr;
	}
};
class Queue
{
	int *p, n;
	int si, ei;

public:
	Queue()
	{
		n = 100;
		p = new int[n];
		si = 0;
		ei = 0;
	}
	Queue(int n)
	{
		this->n = n;
		p = new int[n];
		si = 0;
		ei = 0;
	}
	Queue & operator = (const Queue &q)
		//Queue(const Queue &q)
	{
		int *pp = new int[q.n];
		si = 0;
		ei = 0;
		for (int i = 0; i < q.ei; i++)
		{
			pp[i] = q.p[i];
			ei = (ei + 1) % n;
		}
		delete[] p;
		p = pp;
		return *this;
	}
	bool empty()
	{
		return (ei == si);
	}
	bool full()
	{
		return (ei + 1) % n == si;
	}
	void push(char v)
	{
		if (full()) throw "—тек полон, операци€ push невозможна";
		p[ei] = v;
		ei = (ei + 1) % n;
	}
	int pop()
	{
		if (empty()) throw "—тек пуст, операци€ pop невозможна";
		si = (si + 1) % n;
		return p[si - 1];
	}
	string print()
	{
		string s;
		if (!empty())
		{
			for (int i = si; i < ei; i++)
			{
				stringstream number;
				number << p[i];
				s += number.str();
				s += "  ";
				//s += '\n';
			}
			s += '\n';
		}
		else
		{
			s += "queue is empty";
			s += '\n';
		}
		return s;
	}
	/*~Queue(void)
	{
	delete[] p;
	}*/
};
class MatrixDNA
{
	int m, n, k;
	int fullStr;
	char **mas;
	int row_pl;
public:
	MatrixDNA(int m, int n, int k)
	{
		this->m = m;
		this->n = n;
		this->k = k;
		fullStr = 0;
		row_pl = n - k + 1;
		mas = new char*[m];
		for (int i = 0; i < m; i++)
			mas[i] = new char[n];
	}
	void push(string s)
	{
		if (fullStr != m)
		{
			for (int i = 0; i < n; i++)
				mas[fullStr][i] = s[i];
			fullStr++;
		}
		else throw "You're trying push too many string";
	}
	void print()
	{
		if (fullStr != m) throw "it're not enough strings";

	}
	int min(Queue *s)
	{
		int *TotalDistance = new int[row_pl];
		int *score = new int[row_pl];
		for (int ii = 0; ii < row_pl; ii++) score[ii] = 0;
		for (int i = 0; i < row_pl; i++) //проходим по всем переданным очеред€м, считаем дл€ них score
		{
			int *M = new int[k];
			int *st = new int[m];
			for (int ii = 0; ii < m; ii++)
				st[ii] = s[i].pop();    //текущие стартовые позиции

			for (int c = 0; c < k; c++)
			{
				int *letters = new int[4];
				for (int ii = 0; ii < 4; ii++) letters[ii] = 0;
				for (int r = 0; r < m; r++)
				{
					switch (mas[r][st[r] + c])
					{
					case 'A': letters[0]++; break;
					case 'C': letters[1]++; break;
					case 'G': letters[2]++; break;
					case 'T': letters[3]++; break;
					}
				}

				int max = letters[0];
				for (int ii = 1; ii < 4; ii++)
					if (letters[ii]>max) max = letters[ii];
				M[c] = max;

			}
			delete[] st;


			for (int ii = 0; ii < k; ii++)
				score[i] += M[ii];

		}
		TotalDistance[0] = row_pl - score[0];
		int min = TotalDistance[0];		
		int mini = 0;
		for (int ii = 1; ii < row_pl; ii++)			
		{
			TotalDistance[ii] = row_pl - score[ii];
			if (TotalDistance[ii] <= min)
			{
				min = TotalDistance[ii];
				mini = ii;
			}
		}
		return mini;
	}
	Queue rec_func(int q, Queue _s)
	{
		Queue *s = new Queue[row_pl + 2];
		Queue *ss = new Queue[row_pl + 2];
		int max;
		if (q == m)
		{
			for (int i = 0; i < row_pl; i++)
			{
				s[i] = _s;
				s[i].push(i);
				ss[i] = s[i];
			}

			max = min(s);

		}
		else
		{
			for (int i = 0; i < row_pl; i++)
			{
				s[i] = _s;
				s[i].push(i);
				ss[i] = s[i];
				int qq = q + 1;
				ss[i] = rec_func(qq, s[i]);

			}
			max = min(ss);
		}

		return ss[max];
	}
	Queue* minimizeStartPositions()
	{
		int q = 1;
		Queue *the_end=new Queue();
		Queue st(2 * m);
		the_end[0] = rec_func(q, st);
		return the_end;
	}
	string MedianString()
	{
		string medianString = "";
		Queue *f = new Queue();
		f = minimizeStartPositions();
		int *st = new int[m];
		for (int i = 0; i < m; i++)
			st[i] = f->pop();
		for (int c = 0; c < k; c++)
		{
			int *letters = new int[4];
			for (int ii = 0; ii < 4; ii++) letters[ii] = 0;
			for (int r = 0; r < m; r++)
			{
				switch (mas[r][st[r] + c])
				{
				case 'A': letters[0]++; break;
				case 'C': letters[1]++; break;
				case 'G': letters[2]++; break;
				case 'T': letters[3]++; break;
				}
			}

			int max = letters[0];
			int maxi = 0;
			for (int ii = 1; ii < 4; ii++)
				if (letters[ii]>max)
				{
					max = letters[ii];
					maxi = ii;
				}
			switch (maxi)
			{
				case 0: medianString += 'A'; break;
				case 1: medianString += 'C'; break;
				case 2: medianString += 'G'; break;
				case 3: medianString += 'T'; break;
			}
		}
		return medianString;
	}
};

void main() {
	//считываем по строкам из файла  /////////

	string *ss=new string[1];
	ifstream fin("input.txt", ios_base::in);
	getline(fin, ss[0]);	
	int ii;
	int k = stoi(ss[0]);
	int m = 5;               ////////////////////////////////change
	string *s = new string[m];
	//////////////////////////////////////////
	for (int i = 0; i < 5; i++)
	{
		getline(fin, s[i]);
	}
	int n = s[0].length();
	MatrixDNA *dna = new MatrixDNA(m, n, k);
	for (int i = 0; i < m; i++)
		dna->push(s[i]);
	dna->print();


	//
	string medianString = dna->MedianString();
	cout << "Median String =  " << medianString << endl;

	//// записываем результат в файл   ////////////////////////////
	ofstream fout("output.txt", ios_base::out | ios_base::trunc);
	fout << medianString;
	fout.close();

	delete[] s;


}