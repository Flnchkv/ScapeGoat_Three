#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>
using namespace std;

//Создаём класс элементов дерева
class Node
{
public:
	Node *left_child, *right_child, *parent;
	float value;
	Node(float v)
	{
		value = v;
		left_child = right_child = parent = NULL;
	}
};

//Создаём корень
Node *root;

//Создаём пустое дерево козла отпущения
class ScapeGoatTree
{
public:
	int n, q;
	ScapeGoatTree() {
		root = NULL;
		n = 0;
		q = 0;
	}

  //Создаём функцию для нахождения высоты дерева
	int size(Node *node)
	{
		if (node == NULL)
			return 0;
		return 1 + size(node->left_child) + size(node->right_child);
	}

	int log32(int q)
	{
		return (int)(log(q) / log(1.5));
	}

//Операция поиска элемента в дереве. Стандартный. Пока не нашли значение обходим дерево от левого поддерева
	bool Search(Node *r, float val)
	{
		while (r != NULL)
		{
			int r_val = r->value;
			if (val < r_val)
				r = r->left_child;
			else if (val > r_val)
				r = r->right_child;
			else
				return true;
		}
		return false;
	}

//Операция вставки элемента в дерево. Если коэффициент несбалансированности отходит от нормы, то вызываем функцию перебалансировки дерева.
	bool Insert(float x)
	{
		Node *node = new Node(x);
		int d = AddWithDepth(node);
		if (d > log32(q))
		{
			Node *p = node->parent;
			while (3 * size(p) <= 2 * size(p->parent))
				p = p->parent;
      
			RebuildTree(p->parent);
		}
		return d >= 0;
	}

	vector <Node *> arr;
	int MakeArrOfPointers(Node *ptr, int i)
	{
		if (!ptr)
			return i;

		i = MakeArrOfPointers(ptr->left_child, i);
		arr[i++] = ptr;
		return MakeArrOfPointers(ptr->right_child, i);
	}

//Функция перебалансировки. 
	void RebuildTree(Node *u)
	{
		int n = size(u);
		Node *p = u->parent;
		arr.resize(n);
		MakeArrOfPointers(u, 0);
		if (p == NULL)
		{
			root = buildBalancedTree(0, n);
			root->parent = NULL;
		}
		else if (p->right_child == u)
		{
			p->right_child = buildBalancedTree(0, n);
			p->right_child->parent = p;
		}
		else
		{
			p->left_child = buildBalancedTree(0, n);
			p->left_child->parent = p;
		}
	}

//Строим сбалансированное дерево из массива
	Node * buildBalancedTree(int i, int n)
	{
		if (n == 0)
			return NULL;
		int m = n / 2;

		arr[i + m]->left_child = buildBalancedTree(i, m);

		if (arr[i + m]->left_child != NULL)
			arr[i + m]->left_child->parent = arr[i + m];

		arr[i + m]->right_child = buildBalancedTree(i + m + 1, n - m - 1);
		if (arr[i + m]->right_child != NULL)
			arr[i + m]->right_child->parent = arr[i + m];

		return arr[i + m];
	}

	int AddWithDepth(Node *u)
	{
		Node *temp_root = root;
		if (temp_root == NULL)
		{
			root = u;
			n++;
			q++;
			return 0;
		}
    
		bool done = false;
		int d = 0;
		do
		{
			if (u->value < temp_root->value)
			{
				if (temp_root->left_child == NULL)
				{
					temp_root->left_child = u;
					u->parent = temp_root;
					done = true;
				}
				else
					temp_root = temp_root->left_child;
			}
			else if (u->value > temp_root->value)
			{
				if (temp_root->right_child == NULL)
				{
					temp_root->right_child = u;
					u->parent = temp_root;
					done = true;
				}
				else
					temp_root = temp_root->right_child;
			}
			else
				return -1;
			d++;
		} while (!done);

		n++;
		q++;
		return d;
	}

//Прямой обход
  void Pre_order(Node *r)
	{
		if (r != NULL)
		{
			cout << r->value << "   ";
			Pre_order(r->left_child);
			Pre_order(r->right_child);
		}
	}

//Поуровневый обход
		void Level_order(Node *r)
	{
		if (!r)
		{
			return;
		}
		queue <Node *> q;
		q.push(r);
		cout << endl;
		while (!q.empty())
		{
			int s = q.size();
			while (s--)
			{
				auto p = q.front();
				q.pop();
				cout << p->value << " ";
				if (p->left_child)
					q.push(p->left_child);
				if (p->right_child)
					q.push(p->right_child);
			}
			cout << endl;
		}
	}

//Симметричный обход
	Node *InorderPred(Node *r)
	{
		r = r->left_child;
		while (r->right_child)
		{
			r = r->right_child;
		}
		return r;
	}

//Удаление дерева
	Node* DeleteBST(Node *r, float key)
	{
		if (!r)
			return NULL;
		if (r->value > key)
		{
			r->left_child = DeleteBST(r->left_child, key);
		}
		else if (r->value < key)
		{
			r->right_child = DeleteBST(r->right_child, key);
		}
		else
		{
			if (!r->left_child)
			{
				Node* temp = r->right_child;
				return temp;
			}
			else if (!r->right_child)
			{
				Node* temp = r->left_child;
				return temp;
			}
			Node *temp = InorderPred(r);
			r->value = temp->value;
			r->left_child = DeleteBST(r->left_child, temp->value);
		}
		return r;
	}

//Функция удаления элемента
	void DeleteNode(Node *r, float key)
	{
		if (Search(r, key))
		{
			root = DeleteBST(r, key);
			n--;
			if (q > 2 * n)
			{
				RebuildTree(root);
				q = n;
			}
			cout << key << " удалён." << endl;
		}
		else
			cout << "Элемент не найден." << endl;
	}
};

//Список доступных операций
enum interface
{
	Interface,
	Insert, 
	Search,
	Delete,
	Display_Tree,
	End,
};

//Вывод списка операций
void PrintInterface()
{
	cout << "\nScapeGoat Tree Операции\n";
  cout << interface::Interface << ". Вывод интерфейса" << endl;
	cout << interface::Insert << ". Вставка " << endl;
	cout << interface::Search << ". Поиск" << endl;
	cout << interface::Delete << ". Удаление" << endl;
	cout << interface::Display_Tree << ". Вывод дерева" << endl;
	cout << interface::End << ". Завершить программу" << endl;
}

//Вывод дерева
void DisplayTree(ScapeGoatTree &sgt)
{
	cout << "\nПрямой обход : ";
	sgt.Pre_order(root);
	cout << "\nПоуровневый обход : ";
	sgt.Level_order(root);
}

void AutoInteract(ifstream &input, ScapeGoatTree &sgt)
{
	float val;
	int choice;
	while (input >> choice)
	{
		switch (choice)
		{
			case interface::Insert :
				input >> val;
				sgt.Insert(val);
				break;

				case interface::Search :
					input >> val;
					if (sgt.Search(root, val))
						cout << val << " Элемент найден в дереве" << endl;
					else
						cout << val << " Элемент не найден" << endl;
					break;

					case interface::Delete :
						input >> val;
						sgt.DeleteNode(root, val);

						case interface::Display_Tree :
							DisplayTree(sgt);
							break;
		}
	}

}
void NormalInteract(ScapeGoatTree &sgt)
{
	float val;
	int choice;
	int file = 0;
	PrintInterface();
	do
	{
		cout << "Выберите команду: ";
		cin >> choice;
		switch (choice)
		{
			case interface::Interface :
				PrintInterface();
				break;

				case interface::End :
						break;

					case interface::Display_Tree :
						DisplayTree(sgt);
					break;

							case interface::Insert :
                {
								cout << "Введите элемент типа int для вставки: ";
								cin >> val;
                double start = clock();
								sgt.Insert(val);
                double end = clock();
							  double time = 0.0;
                time = (double)(end - start);
							  cout << "Время выполнения: " << time << " ms." << endl;
								break;
                  }
      

								case interface::Search :
                  {
									cout << "Введите элемент типа int для поиска: ";
									cin >> val;
                  double start = clock();
                    
									if (sgt.Search(root, val))
                  {
										cout << val << " Элемент найден в дереве" << endl;
                    double end = clock();
							      double time = 0.0;
                    time = (double)(end - start);
							      cout << "Время выполнения: " << time << " ms." << endl;
                    }
                  else
                  {
										cout << val << " Элемент не найден" << endl;
                    double end = clock();
							      double time = 0.0;
                    time = (double)(end - start);
							      cout << "Время выполнения: " << time << " ms." << endl;
									  break;
                    }
                    }

									case interface::Delete : {
										cout << "Введите элемент типа int для удаления: ";
										cin >> val;
                    double start = clock();
										sgt.DeleteNode(root, val);
                    double end = clock();
							      double time = 0.0;
                    time = (double)(end - start);
							      cout << "Время выполнения: " << time << " ms." << endl;
									  break;
                    }
                    
									default: break;

		}
	} while (choice != interface::End);
}
