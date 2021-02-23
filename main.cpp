#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

/*Binary Search Tree*/
struct BSTNode {
	int value;
	BSTNode* left;
	BSTNode* right;
	BSTNode(int _value) : value(_value), left(nullptr), right(nullptr) {}
};

template <typename Node>
void deleteTree(Node* node) {
	if (node == nullptr) {
		return;
	}
	deleteTree(node->right);
	deleteTree(node->left);
	delete node;
}

class BSTree {
private:
	BSTNode* root;
public:
	
	BSTree() {
		root = nullptr;
	}
	~BSTree() {
		deleteTree(root);
	}

	void insert(int v);
	bool search(int v);
	//Root getter
	BSTNode* getRoot();
};


void BSTree::insert(int v) {
	BSTNode* parent = nullptr;
	BSTNode* ptr = root;

	while (ptr != nullptr) {
		if (v < ptr->value) {
			parent = ptr;
			ptr = ptr->left;
		}
		else {
			parent = ptr;
			ptr = ptr->right;
		}
	}

	BSTNode* inserted = new(nothrow) BSTNode(v);

	if (parent == nullptr) {
		root = inserted;
	}
	else {
		if (v < parent->value) {
			parent->left = inserted;
		}
		else {
			parent->right = inserted;
		}
	}
}

bool BSTree::search(int v) {
	BSTNode* node = root;
	while (node != nullptr) {
		if (v > node->value) {
			node = node->right;
		}
		else if (v < node->value) {
			node = node->left;
		}
		else {
			return true;
		}
	}
	return false;
}

//Getter(for root) 
BSTNode* BSTree::getRoot() { return root; }

int max(int a, int b) { return a > b ? a : b; }

/*AVL TREE*/
struct AVLNode {
	int value;
	int height;
	AVLNode* left;
	AVLNode* right;
	AVLNode(int _value) : value(_value), left(nullptr), right(nullptr), height(0) {}
};

class AVLTree {
private:
	AVLNode* root;
public:
	AVLTree() {
		root = nullptr;
	}
	~AVLTree() {
		deleteTree(root);
	}

	AVLNode* RightRotation(AVLNode* node);
	AVLNode* LeftRotation(AVLNode* node);
	AVLNode* insert(AVLNode* node, int v);
	int updateHeight(AVLNode* node);
	int getHeight(AVLNode* node);
	void insert(int v);
	bool search(int v);
	AVLNode* getRoot() { return root; }

};


int AVLTree::updateHeight(AVLNode* node) {
	int h = -1;
	if (node != nullptr) {
		return 1 + max(getHeight(node->left), getHeight(node->right));
	}
	return h;
}

int AVLTree::getHeight(AVLNode* node) {
	int h = 0;
	if (node != nullptr) {
		return node->height;
	}
	return h;
}

AVLNode* AVLTree::RightRotation(AVLNode* node){
	AVLNode* temp = node->left;
	node->left = temp->right;
	temp->right = node;
	temp->height = updateHeight(temp);
	node->height = updateHeight(node);
	return temp;
}


AVLNode* AVLTree::LeftRotation(AVLNode* node) {
	AVLNode* temp = node->right;
	node->right = temp->left;
	temp->left = node;
	temp->height = updateHeight(temp);
	node->height = updateHeight(node);
	return temp;
}


AVLNode* AVLTree::insert(AVLNode* node, int v) {
	if (node == nullptr)
	{
		node = new AVLNode(v);
	}
	else if (v < node->value)
	{
		node->left = insert(node->left, v);
		if (getHeight(node->left) - getHeight(node->right) == 2)
		{
			if (v < node->left->value) {
				node = RightRotation(node);
			}
			else {
				//Left Right case
				node->left = LeftRotation(node->left);
				node = RightRotation(node);
			}
		}
	}
	else if (v > node->value)
	{
		node->right = insert(node->right, v);
		if (getHeight(node->right) - getHeight(node->left) == 2)
		{
			if (v > node->right->value) {
				node = LeftRotation(node);
			}
			else {
				//Rate Left case
				node->right = RightRotation(node->right);
				node = LeftRotation(node);
			}
		}
	}
	node->height = updateHeight(node);
	return node;
}

void AVLTree::insert(int v)
{
	root = insert(root, v);
}

//same search method as in Binary Tree
bool AVLTree::search(int v) {
	AVLNode* node = root;
	while (node != nullptr) {
		if (v > node->value) {
			node = node->right;
		}
		else if (v < node->value) {
			node = node->left;
		}
		else {
			return true;
		}
	}
	return false;
}


/*Print*/
template <typename Node>
void printTree(Node* root, int space)
{
	int spaces = 4;
	if (root == NULL)return;
	space += spaces;
	printTree(root->right, space);
	for (int i = spaces; i < space; i++) cout << " ";
	cout << "->" << root->value << endl;
	printTree(root->left, space);
}

template <typename Node>
void print(Node* root)
{
	printTree(root, 0);
}

const int iterations = 50;
int main() {
	ofstream insertData, searchData;
	insertData.open("insertData.csv");
	searchData.open("searchData.csv");
	if (!insertData || !searchData) {
		cout << "Failed to open a file!" << endl;
		return 1;
	}
	insertData << "Size, BST_ordered_insert, BST_random_insert, BST_nearly_ordered_insert, AVL_ordered_insert, AVL_random_insert, AVL_nearly_ordered_insert" << endl;
	searchData << "Size, BST_ordered_included_search, BST_random_included_search, BST_nearly_ordered_included_search, AVL_ordered_included_search, AVL_random_included_search, AVL_nearly_ordered_included_search, BST_ordered_excluded_search, BST_random_excluded_search, BST_nearly_ordered_excluded_search, AVL_ordered_excluded_search, AVL_random_excluded_search, AVL_nearly_ordered_excluded_search" << endl;
	random_device rd;
	mt19937 gen(rd());
	for (int size = 50; size <= 3000; size += 50) {
		cout << "Benchmarking size: " << size << " ";

		BSTree odBSTree, rmBSTree, ndBSTree;
		AVLTree odAVLTree, rmAVLTree, ndAVLTree;

		//for insert BST and AVL
		chrono::nanoseconds odBSTIns(0), rmBSTIns(0), ndBSTIns(0), odAVLIns(0), rmAVLIns(0), ndAVLIns(0);

		//for search included 
		chrono::nanoseconds odBSTsIN(0), rmBSTsIN(0), ndBSTsIN(0), odAVLsIN(0), rmAVLsIN(0), ndAVLsIN(0);

		//for search excluded
		chrono::nanoseconds odBSTsEX(0), rmBSTsEX(0), ndBSTsEX(0), odAVLsEX(0), rmAVLsEX(0), ndAVLsEX(0);

		for (int i = 0; i < iterations; i++) {
			/*BINARY SEARCH*/
			//Get nanoseconds for ordered data in Binary Search Tree(Worst case)

			for (int j = 0; j < size - 1; j++) {
				odBSTree.insert(j);
			}
			auto StartodBStree = std::chrono::steady_clock::now();
			odBSTree.insert(size - 1);
			auto EndodBStree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odBStreeTimeTaken(EndodBStree - StartodBStree);
			odBSTIns += odBStreeTimeTaken;

			//Get nanoseconds for random data in Binary Search Tree(Worst case)
			vector<int> randomBSTData; // a vector need for search to get a random value from list
			int randomValue = gen(); // this variable is used to push a data in to the tree and vector

			for (int j = 0; j < size - 1; j++) {
				randomBSTData.push_back(randomValue);
				rmBSTree.insert(randomValue);
				randomValue = gen();
			}
			randomBSTData.push_back(randomValue);
			auto StartrmBSTree = std::chrono::steady_clock::now();
			rmBSTree.insert(randomValue);
			auto EndrmBSTree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmBSTreeTimeTaken(EndrmBSTree - StartrmBSTree);
			rmBSTIns += rmBSTreeTimeTaken;

			//Get nanoseconds for nearly ordered data in Binary Search Tree(Worst case)
			vector<int> nearlyOrderedBSTData; // a vector need for search to get a random value from list
			int v; // this variable is used to push a data in to the tree and vector

			for (int j = 0; j < size - 1; j++) {
				if (j % 5 == 0) {
					v = j / 5 + 1;
				}
				else {
					v = j;
				}
				ndBSTree.insert(v);
				nearlyOrderedBSTData.push_back(v);
			}
			v = size - 10;
			nearlyOrderedBSTData.push_back(v);
			auto StartndBSTree = std::chrono::steady_clock::now();
			ndBSTree.insert(v);
			auto EndndBSTree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndBSTreeTimeTaken(EndndBSTree - StartndBSTree);
			ndBSTIns += ndBSTreeTimeTaken;
			/*==========AVL TREE INSERTATION===========*/
			//Get nanoseconds for ordered data in AVL Tree(Worst case)
			for (int j = 0; j < size - 1; j++) {
				odAVLTree.insert(j);
			}
			auto StartodAVLTree = std::chrono::steady_clock::now();
			odAVLTree.insert(size - 1);
			auto EndodAVLTree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odAVLTreeTimeTaken(EndodAVLTree - StartodAVLTree);
			odAVLIns += odAVLTreeTimeTaken;

			//Get nanoseconds for random data in AVL Tree(Worst case)
			vector<int> randomAVLData; // a vector need for search to get a random value from list
			int rValue = gen(); // this variable is used to push a data in to the tree and vector

			for (int j = 0; j < size - 1; j++) {
				randomAVLData.push_back(rValue);
				rmAVLTree.insert(rValue);
				randomValue = gen();
			}
			randomAVLData.push_back(rValue);
			auto StartrmAVLTree = std::chrono::steady_clock::now();
			rmAVLTree.insert(rValue);
			auto EndrmAVLTree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmAVLTreeTimeTaken(EndrmAVLTree - StartrmAVLTree);
			rmAVLIns += rmAVLTreeTimeTaken;

			//Get nanoseconds for nearly ordered data in AVL Tree(Worst case)
			vector<int> nearlyOrderedAVLData; // a vector need for search to get a random value from list
			v; // this variable is used to push a data in to the tree and vector

			for (int j = 0; j < size - 1; j++) {
				if (j % 5 == 0) {
					v = j / 5 + 1;
				}
				else {
					v = j;
				}
				ndAVLTree.insert(v);
				nearlyOrderedAVLData.push_back(v);
			}
			v = size - 10;
			nearlyOrderedAVLData.push_back(v);
			auto StartndAVLTree = std::chrono::steady_clock::now();
			ndAVLTree.insert(v);
			auto EndndAVLTree = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndAVLTreeTimeTaken(EndndAVLTree - StartndAVLTree);
			ndAVLIns += ndAVLTreeTimeTaken;

			/*==========BST SEARCH INCLUDED===========*/
			//Get nanoseconds for ordered binary search
			//a random value from range of size will be used for search
			int s; // search value

			s = rand() % size;
			auto StartodBSTsIN = std::chrono::steady_clock::now();
			odBSTree.search(s);
			auto EndodBSTsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odBSTsINTimeTaken(EndodBSTsIN - StartodBSTsIN);
			odBSTsIN += odBSTsINTimeTaken;

			//Get nanoseconds for random binary search tree
			//Value for search will be taken from vector(above random values were inserted in a vector)
			s = randomBSTData[rand() % size]; // get a random value from vector
			auto StartrmBSTsIN = std::chrono::steady_clock::now();
			rmBSTree.search(s);
			auto EndrmBSTsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmBSTsINTimeTaken(EndrmBSTsIN - StartrmBSTsIN);
			rmBSTsIN += rmBSTsINTimeTaken;

			//Get nanoseconds for nearly ordered binary search tree

			s = nearlyOrderedBSTData[rand() % size];
			auto StartndBSTsIN = std::chrono::steady_clock::now();
			ndBSTree.search(s);
			auto EndndBSTsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndBSTsINTimeTaken(EndndBSTsIN - StartndBSTsIN);
			ndBSTsIN += ndBSTsINTimeTaken;

			/*==========AVL TREE SEARCH INCLUDED===========*/
			s = rand() % size;
			auto StartodAVLsIN = std::chrono::steady_clock::now();
			odAVLTree.search(s);
			auto EndodAVLsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odAVLsINTimeTaken(EndodAVLsIN - StartodAVLsIN);
			odAVLsIN += odAVLsINTimeTaken;

			s = randomAVLData[rand() % size]; // get a random value from vector
			auto StartrmAVLsIN = std::chrono::steady_clock::now();
			rmAVLTree.search(s);
			auto EndrmAVLsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmAVLsINTimeTaken(EndrmAVLsIN - StartrmAVLsIN);
			rmAVLsIN += rmAVLsINTimeTaken;

			s = nearlyOrderedAVLData[rand() % size];
			auto StartndAVLsIN = std::chrono::steady_clock::now();
			ndAVLTree.search(s);
			auto EndndAVLsIN = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndAVLsINTimeTaken(EndndAVLsIN - StartndAVLsIN);
			ndAVLsIN += ndAVLsINTimeTaken;

			/*==========BST TREE SEARCH EXCLUDED===========*/
			//same work as above
			s = size + 1; // As it`s ordered we insert data [0, 1, 2 .. size-1], there is no size+1 
			auto StartodBSTsEX = std::chrono::steady_clock::now();
			odBSTree.search(size);
			auto EndodBSTsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odBSTsEXTimeTaken(EndodBSTsEX - StartodBSTsEX);
			odBSTsEX += odBSTsEXTimeTaken;

			s = *max_element(randomBSTData.begin(), randomBSTData.end()) + 1; // we are getting max value from vector and adding 1
			auto StartrmBSTsEX = std::chrono::steady_clock::now();
			rmBSTree.search(s);
			auto EndrmBSTsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmBSTsEXTimeTaken(EndrmBSTsEX - StartrmBSTsEX);
			rmBSTsEX += rmBSTsEXTimeTaken;

			s = size + 1;
			auto StartndBSTsEX = std::chrono::steady_clock::now();
			ndBSTree.search(size + 1);
			auto EndndBSTsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndBSTsEXTimeTaken(EndndBSTsEX - StartndBSTsEX);
			ndBSTsEX += ndBSTsEXTimeTaken;

			/*==========AVL TREE SEARCH INCLUDED===========*/
			s = size + 1; // As it`s ordered we insert data [0, 1, 2 .. size-1], there is no size+1 
			auto StartodAVLsEX = std::chrono::steady_clock::now();
			odAVLTree.search(size);
			auto EndodAVLsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds odAVLsEXTimeTaken(EndodAVLsEX - StartodAVLsEX);
			odAVLsEX += odAVLsEXTimeTaken;

			s = *max_element(randomAVLData.begin(), randomAVLData.end()) + 1;
			auto StartrmAVLsEX = std::chrono::steady_clock::now();
			rmAVLTree.search(s);
			auto EndrmAVLsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds rmAVLsEXTimeTaken(EndrmAVLsEX - StartrmAVLsEX);
			rmAVLsEX += rmAVLsEXTimeTaken;

			s = size + 1;
			auto StartndAVLsEX = std::chrono::steady_clock::now();
			ndAVLTree.search(size + 1);
			auto EndndAVLsEX = std::chrono::steady_clock::now();
			std::chrono::nanoseconds ndAVLsEXTimeTaken(EndndAVLsEX - StartndAVLsEX);
			ndAVLsEX += ndAVLsEXTimeTaken;



		}
		cout << odBSTIns.count() / iterations << " " << rmBSTIns.count() / iterations << " " << ndBSTIns.count() / iterations << " "
			<< odAVLIns.count() / iterations << " " << rmAVLIns.count() / iterations << " " << ndAVLIns.count() / iterations << " "
			<< odBSTsIN.count() / iterations << " " << rmBSTsIN.count() / iterations << " " << ndBSTsIN.count() / iterations << " "
			<< odAVLsIN.count() / iterations << " " << rmAVLsIN.count() / iterations << " " << ndAVLsIN.count() / iterations << " "
			<< odBSTsEX.count() / iterations << " " << rmBSTsEX.count() / iterations << " " << ndBSTsEX.count() / iterations << " "
			<< odAVLsEX.count() / iterations << " " << rmAVLsEX.count() / iterations << " " << ndAVLsEX.count() / iterations << endl;
		insertData << size << ", " << odBSTIns.count() / iterations << ", " << rmBSTIns.count() / iterations << ", " << ndBSTIns.count() / iterations << ", "
			<< odAVLIns.count() / iterations << ", " << rmAVLIns.count() / iterations << ", " << ndAVLIns.count() / iterations << endl;
		searchData << size << ", " << odBSTsIN.count() / iterations << ", " << rmBSTsIN.count() / iterations << ", " << ndBSTsIN.count() / iterations << ", "
			<< odAVLsIN.count() / iterations << ", " << rmAVLsIN.count() / iterations << ", " << ndAVLsIN.count() / iterations << ", "
			<< odBSTsEX.count() / iterations << ", " << rmBSTsEX.count() / iterations << ", " << ndBSTsEX.count() / iterations << ", "
			<< odAVLsEX.count() / iterations << ", " << rmAVLsEX.count() / iterations << ", " << ndAVLsEX.count() / iterations << endl;

	}


	/*
	AVLTree tree;
	tree.insert(5);
	tree.insert(4);
	tree.insert(2);
	tree.insert(7);
	tree.insert(9);
	tree.insert(10);
	tree.insert(1);
	tree.insert(2);
	tree.insert(13);
	tree.insert(4);
	tree.insert(5);
	tree.insert(6);
	tree.insert(7);
	tree.insert(8);
	tree.insert(9);
	tree.insert(10);
	tree.insert(11);
	tree.insert(6);
	print(tree.getRoot());
	
	

	/*
	AVLTree tree;
	tree.insert(18);
	tree.insert(17);
	tree.insert(16);
	tree.insert(15);
	tree.insert(14);
	tree.insert(13);
	tree.insert(12);
	tree.insert(11);
	tree.insert(10);
	tree.insert(9);
	tree.insert(8);
	tree.insert(7);
	tree.insert(6);
	tree.insert(5);
	tree.insert(4);
	tree.insert(3);
	tree.insert(2);
	tree.insert(1);
	printTreeNode(tree.getRoot());
	*/
	
}