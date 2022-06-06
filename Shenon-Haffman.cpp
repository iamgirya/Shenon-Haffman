#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include<set>
#include<vector>
#include<queue>
typedef long double ll;
#include <string>
using namespace std;


struct myhash
{
	vector<myhash*> v;
	ll inf=0;
	string dopinf = "";
};

void hinit(myhash &hah)
{
	hah.v.resize(256, NULL);
}

auto ahash(char h)
{
	return ((long long)h     +127);
}

void hplus(myhash &hah, string s)
{
	if (s == "")
	{
		hah.inf++;
	}
	else
	{
		auto ah = ahash(s[0]);
		if (hah.v[ah] != NULL)
		{
			hplus(*hah.v[ah], s.substr(1, s.size()-1));
		}
		else
		{
			myhash* r = new myhash;
			hah.v[ah] = r;
			hinit(*r);
			hplus(*hah.v[ah], s.substr(1, s.size() - 1));
		}
	}
}

ll htake(myhash hah, string s)
{
	if (s == "")
	{
		return hah.inf;
	}
	else
	{
		auto ah = ahash(s[0]);
		return htake(*hah.v[ah], s.substr(1, s.size() - 1));
	}
}

string hdoptake(myhash hah, string s)
{
	if (s == "")
	{
		return hah.dopinf;
	}
	else
	{
		auto ah = ahash(s[0]);
		return hdoptake(*hah.v[ah], s.substr(1, s.size() - 1));
	}
}

void hdopzap(myhash &hah, string s, string infa)
{
	if (s == "")
	{
		hah.dopinf = infa;
	}
	else
	{
		auto ah = ahash(s[0]);
		return hdopzap(*hah.v[ah], s.substr(1, s.size() - 1), infa);
	}
}

void hdeli(myhash &hah, string s, ll delitel)
{
	if (s == "")
	{
		hah.inf /= delitel;
	}
	else
	{
		auto ah = ahash(s[0]);
		return hdeli(*hah.v[ah], s.substr(1, s.size() - 1), delitel);
	}
}

void hinse(myhash& hah, string s, ll x)
{
	if (s == "")
	{
		hah.inf = x;
	}
	else
	{
		auto ah = ahash(s[0]);
		if (hah.v[ah] != NULL)
		{
			hinse(*hah.v[ah], s.substr(1, s.size() - 1), x);
		}
		else
		{
			myhash* r = new myhash;
			hah.v[ah] = r;
			hinit(*r);
			hinse(*hah.v[ah], s.substr(1, s.size() - 1), x);
		}
	}
}

////////////////////////
ll num = 2;
////////////////////////
ll kol_simvol = 0;
myhash mainslovar;


vector<pair<string, ll>> schet()
{
	ifstream f("slovar.txt");
	vector<pair<string, ll>> trueslovar;
	hinit(mainslovar);
	while (!f.eof())
	{
		string s;
		string xs;
		
		getline(f,s);
		ll itg;
		getline(f, xs);
		stringstream stoll(xs);
		
		stoll >> itg;

		trueslovar.push_back(make_pair(s, itg));
		hinse(mainslovar, s, itg);
	}

	return trueslovar;
}
vector<pair<string, ll>> chet(string text)
{
	vector<pair<string, ll>> trueslovar;
	set<string> slova;
	hinit(mainslovar);
	for (ll h = 0; h <= text.size() - num; h += num)
	{
		kol_simvol += num;

		string s = text.substr(h, num);
		slova.insert(s);
		hplus(mainslovar, s);

	}
	for (auto u: slova)
	{
		trueslovar.push_back({ u, num*htake(mainslovar, u)/ kol_simvol });
	}

	return trueslovar;
} 

void qsortRecursive(vector<pair<string, ll>>& mas, ll n, ll k) {
	//Указатели в начало и в конец массива
	ll i = n;
	ll j = k;

	//Центральный элемент массива
	ll mid = mas[(n+k) / 2].second;

	//Делим массив
	do {
		//Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
		//В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
		while (mas[i].second > mid) {
			i++;
		}
		//В правой части пропускаем элементы, которые больше центрального
		while (mas[j].second < mid) {
			j--;
		}

		//Меняем элементы местами
		if (i <= j) {
			swap(mas[i], mas[j]);

			i++;
			j--;
		}
	} while (i <= j);


	//Рекурсивные вызовы, если осталось, что сортировать
	if (j > n) {
		//"Левый кусок"
		qsortRecursive(mas, n, j);
	}
	if (i < k) {
		//"Прaвый кусок"
		qsortRecursive(mas, i, k);
	}
}

void zapis(string &kdk, vector<pair<string, string>> *kods, string *text)
{
	for (auto j : (*kods))
	{
		hdopzap(mainslovar, j.first, j.second);
	}


	for (ll h = 0; h <= (*text).size() - num; h += num)
	{
		string s = (*text).substr(h, num);
		kdk += hdoptake(mainslovar, s);
	}
}


struct dvatree
{
	dvatree* l=NULL, * r = NULL;
	string slovo = "";
};

void dtzapis(dvatree dt, vector<pair< string, string>> &kodi, string nowkod)
{
	if (dt.slovo == "")
	{
		dtzapis(*dt.l, kodi, nowkod + "0");
		dtzapis(*dt.r, kodi, nowkod + "1");
	}
	else
		kodi.push_back({ dt.slovo, nowkod });
}

void podshenon(vector<pair<string, ll>> *slovar, ll n, ll k, dvatree &tkod, ll sum)
{
	ll mindelta = 1;
	if (n == k - 1)
	{
		tkod.slovo = (*slovar)[n].first;
		return;
	}
	ll ind = n;
	ll sumver = 0;
	while (true)
	{
		sumver += (*slovar)[ind].second;

		ll delta = abs((sum / 2) - sumver);
		if (delta < mindelta) mindelta = delta;
		else break;
		ind++;
	}
	sumver -= (*slovar)[ind].second;
	
	dvatree* r = new dvatree, * l = new dvatree;
	tkod.r = r;
	tkod.l = l;
	podshenon(slovar, n, ind, *tkod.l, sumver);
	podshenon(slovar, ind, k, *tkod.r, sum - sumver);
}

string shenon(vector<pair<string, ll>> slovar, string text)
{
	vector<pair< string, string>> kodi;
	dvatree treekod;
	podshenon(&slovar, 0, slovar.size(), treekod, 1);
	dtzapis(treekod, kodi, "");
	string kodtext = "";
	zapis(kodtext, &kodi, &text);

	//for (auto u : kodi)
	//	cout << u.first << ' ' << u.second << endl;
	

	return kodtext;
}

string haffman(vector<pair<string, ll>> slovar, string text)
{
	dvatree treekod;
	vector<pair<string, string>> kodi;
	

	priority_queue < pair<ll, dvatree*>> sloj;

	for (ll i = 0; i <slovar.size(); i++)
	{
		//kodi.push_back({ slovar[i].first, "" });
		
		dvatree *treekod = new dvatree;
		treekod->slovo = slovar[i].first;
		sloj.push({ 1- slovar[i].second, treekod });
	}
	while(sloj.size() !=1)
	{
		
		auto u1 = sloj.top();
		sloj.pop();
		auto u2 = sloj.top();
		sloj.pop();
		ll min = 1 - ((1 - u1.first) + (1 - u2.first));

		dvatree* treekod = new dvatree;
		treekod->l = u1.second;
		treekod->r = u2.second;

		sloj.push({ min, treekod });
	}

	dtzapis(*sloj.top().second, kodi, "");

	string kodtext = "";
	zapis(kodtext, &kodi, &text);


	for (auto u : kodi)
	{
		cout << u.first << ' ' << u.second << endl;
	}


	return kodtext;
}


#include <Windows.h> 



int main()
{
	SetConsoleCP(1251);  // Подключаем ввод русских букв
	SetConsoleOutputCP(1251);

	string text;
	ifstream i("in.txt");
	ofstream o("out.txt");

	while (!i.eof())
	{
		string s;

		getline(i, s);
		text +=  s;
	}
	//auto slovar = chet(text);     // сам считай
	
	auto slovar = schet();          //  готовый словарь читай
	
	qsortRecursive(slovar, 0, slovar.size() - 1);
	
	
	string kods;
	
	kods = shenon(slovar, text);
	o << kods;
	cout << endl << kol_simvol;
	cout << endl << kods.size();
	cout << endl<<  endl<< endl;
	/*
	kods =  haffman(slovar, text);
	cout << endl << kol_simvol;
	cout << endl << kods.size();
	o << endl<<endl << kods;
	*/
	
	
}

