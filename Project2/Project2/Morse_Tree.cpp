#include "Morse_Tree.h"

// Constructor
Morse_Tree::Morse_Tree(std::vector<Morse_Letter> morse_vec, std::map<std::string, std::string> letter_code_map) {

	this->letter_code_map = letter_code_map;

	// Sorts vector based on struct comparison, necessary before split
	// Sorts based on code_value, which represents an inorder sort
	std::sort(morse_vec.begin(), morse_vec.end());

	// Splits vector into two halves, ones whose code start with a _ and ones with a .
	std::size_t const half_size = morse_vec.size() / 2;
	std::vector<Morse_Letter> dash_letters(morse_vec.begin(), morse_vec.begin() + half_size);
	std::vector<Morse_Letter> dot_letters(morse_vec.begin() + half_size, morse_vec.end());
	// Orders both new vectors to be ready for insertion
	dash_letters = morse_insertion_order(dash_letters);
	dot_letters = morse_insertion_order(dot_letters);
	
	// Create left and right subtrees
	for (auto dash_letter : dash_letters)
		dash_tree.insert(dash_letter);
	for (auto dot_letter : dot_letters)
		dot_tree.insert(dot_letter);
	
	// Empty head, used to match specifications of Project 2 tree
	// Code needs to have length > 1 for debugging purposes
	Morse_Letter head = { "", " " };

	this->root = new BTNode<Morse_Letter>(head) ;
	this->root->left = dash_tree.getRoot();
	this->root->right = dot_tree.getRoot();
	
}

// Sorts a Morse_Letter vector to be ready for insertion 
// Think of it as ordering from the top down based on the example diagram
std::vector<Morse_Letter> Morse_Tree::morse_insertion_order(std::vector<Morse_Letter> morse_vec) {

	std::vector<Morse_Letter> temp_vec;
	// Essentially looks at trailing 0s
	// #'s that end in 000, 00, and then 0
	// Exits before numbers that end in a non-zero value, they can be
		// inserted in any order as they are the leaves
	for (int mod_val = HEIGHT_CODE_VALUE; mod_val > 1; mod_val /= 10) {
		for (int i = 0; i < morse_vec.size(); i++) {
			if ((morse_vec.at(i).code_value % mod_val) == 0) {
				temp_vec.push_back(morse_vec.at(i));
				morse_vec.erase(morse_vec.begin() + i);
			}
		}

	}

	// Combine temporary vector and all remaining items
	std::vector<Morse_Letter> combo_vec;
	combo_vec.reserve(morse_vec.size() + temp_vec.size()); // preallocate memory
	combo_vec.insert(combo_vec.end(), temp_vec.begin(), temp_vec.end());
	combo_vec.insert(combo_vec.end(), morse_vec.begin(), morse_vec.end());

	return combo_vec;
}

// Wrapper function to return a letter from a given code
const std::string Morse_Tree::letter_from_code(const std::string& code) const {
	// '.' means left tree
	if (code.at(0) == '.')
		return letter_from_code(this->root->left, Morse_Letter(" ", code), code.substr(1));
	else
		return letter_from_code(this->root->right, Morse_Letter(" ", code), code.substr(1));
}

// Recursive function to return a letter from a given code
// partial_code moves forward one character each recursive call, used to search left or right
std::string Morse_Tree::letter_from_code(BTNode<Morse_Letter>* local_root,
	const Morse_Letter& target, std::string partial_code) const {
	if (local_root == NULL)
		return "";
	if (local_root->data.code == target.code)
		return local_root->data.letter;
	if (partial_code.at(0) == '.')
		return letter_from_code(local_root->left, target, partial_code.substr(1));
	else
		return letter_from_code(local_root->right, target, partial_code.substr(1));
}

// Returns code for a given letter, public call for private map variable
std::string Morse_Tree::code_from_letter(const std::string& letter) {
	return this->letter_code_map[letter];
}