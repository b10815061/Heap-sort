#include<iostream>
#include<map>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;

class node {
public:
	int index;
	node* parents;
	node* left;
	node* right;
	node() {
		index = 0;
		left = right = parents = NULL;
	};
	node(int a) {

		index = a;
		left = right = parents = NULL;
	};
	node(int a, node* par) {

		index = a;
		parents = par;
		left = right = NULL;
	}
	/*~node() {
		index = 0;
		left = right = parents = NULL;
		delete this;
	}*/

	int show_index() {
		return index;
	}

};

class Tree {
public:
	node* root;
	int node_count;
	int degree;
	bool full_degree;
	Tree() {
		root = NULL;
		node_count = degree = 0;
		full_degree = false;
	};
	Tree(int a) {
		root = new node(a);
		node_count = 0;
		degree = 0;
		full_degree = 0;
	}
	/*~Tree() {

		node_count = degree = 0;
		full_degree = 0;
		root = NULL;
	}*/
	void count_node(node* cur) {

		if (cur == NULL) {
			return;
		}
		else {
			node_count += 1;
			count_node(cur->left);
			count_node(cur->right);
		}
	}

	void find_degree() {
		node_count = 0;
		count_node(root);
		degree = (int)log2(node_count);
		if (pow(2, degree + 1) - 1 == node_count) {
			full_degree = 1;
		}
		else {
			full_degree = 0;
		}
	}

	node* search(node* cur, bool full, int cur_degree = 0) {
		if (cur == NULL) {
			return NULL;
		}
		if (full) {
			if (cur->left == NULL) {
				return cur;
			}
			else {
				return search(cur->left, full);
			}
		}
		else {
			if (cur_degree >= degree) {
				return NULL;
			}
			else {
				if (cur->left == NULL) {
					return cur;
				}
				else if (cur->right == NULL) {
					return cur;
				}
				else {
					node* tmp = search(cur->right, full, cur_degree + 1);
					if (tmp == NULL) {
						return search(cur->left, full, cur_degree + 1);
					}
					else {
						return tmp;
					}

				}
			}
		}

	}

	node* lately(node* cur, int cur_degree = 0) {
		if (cur == NULL) {
			return NULL;
		}
		if (cur_degree == degree) {
			return cur;
		}
		else {
			node* tmp = lately(cur->right, cur_degree + 1);
			if (tmp == NULL) {
				return lately(cur->left, cur_degree + 1);
			}
			else {
				return tmp;
			}
		}
	}

	void insert(int a) {
		find_degree();
		node* cur = search(root, full_degree);
		if (cur->left == NULL) {
			//	cout << "insert " << a << " into " << cur->index << "'s left" << endl;
			cur->left = new node(a, cur);
			swap(cur->left);
		}
		else if (cur->right == NULL) {
			//	cout << "insert " << a << " into " << cur->index << "'s right" << endl;
			cur->right = new node(a, cur);
			swap(cur->right);
		}
		//perform change if llegel

	}

	void swap(node* cur) {
		if (cur->parents != NULL) {
			if (cur->index < cur->parents->index) {
				//cout << "swap " << cur->index << " with " << cur->parents->index << endl;
				int tmp = cur->index;
				cur->index = cur->parents->index;
				cur->parents->index = tmp;
				swap(cur->parents);
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}

	void swap_down(node* cur) {
		if (cur != NULL) {
			if (cur->right != NULL && cur->left != NULL) {
				if (cur->index > cur->right->index && cur->index > cur->left->index) {
					if (cur->right->index < cur->left->index) {
						int tmp = cur->index;
						cur->index = cur->right->index;
						cur->right->index = tmp;
						swap_down(cur->right);
					}
					else {
						int tmp = cur->index;
						cur->index = cur->left->index;
						cur->left->index = tmp;
						swap_down(cur->left);
					}
				}
				/////////
				else if (cur->index > cur->right->index) {
					int tmp = cur->index;
					cur->index = cur->right->index;
					cur->right->index = tmp;
					swap_down(cur->right);
				}
				else if (cur->index > cur->left->index) {
					int tmp = cur->index;
					cur->index = cur->left->index;
					cur->left->index = tmp;
					swap_down(cur->left);
				}
				else {
					return;
				}
			}
			else if (cur->right != NULL) {
				if (cur->index > cur->right->index) {
					int tmp = cur->index;
					cur->index = cur->right->index;
					cur->right->index = tmp;
					swap_down(cur->right);
				}
			}
			else if (cur->left != NULL) {
				if (cur->index > cur->left->index) {
					int tmp = cur->index;
					cur->index = cur->left->index;
					cur->left->index = tmp;
					swap_down(cur->left);
				}

			}
			else {
				return;
			}

		}
	}
	//good
	int pop() {
		find_degree();
		node* NEWroot = lately(root);
		int result = root->index;
		if (NEWroot->parents != NULL) {
			if (NEWroot->parents->right == NEWroot) {
				NEWroot->parents->right = NULL;
			}
			else if (NEWroot->parents->left = NEWroot) {
				NEWroot->parents->left = NULL;
			}
		}
		NEWroot->parents = NULL;
		if (root->right != NULL) {
			NEWroot->right = root->right;
			root->right->parents = NEWroot;
		}
		if (root->left != NULL) {
			NEWroot->left = root->left;
			root->left->parents = NEWroot;
		}
		root = NEWroot;
		if (node_count == 1) {
			root = NULL;
		}
		else {
			swap_down(root);
		}
		return result;
	} //
	//good
	bool is_empty() {
		if (root != NULL) {
			return false;
		}
		return true;
	}
};

void determine_prior(Tree& tree, map<int, string>& index2string) {
	string input;
	getline(cin, input);
	bool first = 1;
	while (input != "") {
		if (input.find(";") != -1) {

			string tmp = input.substr(0, input.find(";"));
			string value = tmp.substr(0, tmp.find(" "));
			string index = tmp.substr(tmp.find(" ") + 1);
			index2string.insert(pair<int, string>(stoi(index), value));
			if (first) {
				tree = Tree(stoi(index));
				Tree a(stoi(index));
				tree = a;
				first = !first;
			}
			else {
				tree.insert(stoi(index));
			}
			input = input.substr(input.find(";") + 1, input.length());
		}
		else {
			string value = input.substr(0, input.find(" "));
			string index = input.substr(input.find(" ") + 1);
			tree.insert(stoi(index));
			index2string.insert(pair<int, string>(stoi(index), value));
			input = "";
		}

	}
}

string map2string(map<int, string>index2string, int a) {
	auto it = index2string.find(a);
	return it->second;
}

int main() {
	string input;
	map<int, string>index2string;
	Tree tree;
	determine_prior(tree, index2string);
	while (!tree.is_empty()) {
		cout << map2string(index2string, tree.pop());
	}
	return 0;
}