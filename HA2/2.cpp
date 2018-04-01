// Find all formal concepts for the context set up in setMatrix()

#include <iostream>
#include <stdexcept>
#include <array>
#include <set>
#include <utility>
#include <algorithm>

#define G 13
#define M 9

typedef std::array<std::array<bool, M>, G> 	matrix_t;
typedef std::set<unsigned int> 				object_t;
typedef std::set<object_t>					subsets_t;
typedef std::set<char>						feature_t;
typedef std::pair< object_t, feature_t > 	formal_concept_t;


void generateAllSubsetsWithElem(unsigned int element, subsets_t& previousSets)
{
  	subsets_t set;
  	object_t elemset, new_set;
  	elemset.insert(element); // set of only current element

  	set.insert(elemset);

  	for(auto elem: previousSets) {
  		std::set_union(
  			elemset.begin(), elemset.end(),
  			elem.begin(), elem.end(),
  			std::inserter(new_set, new_set.begin()));
  		set.insert(new_set);
  		new_set.clear();
  	}

  	for(auto subset: set) {
  		previousSets.insert(subset);
  	}
}

subsets_t generateAllObjectsSubsets()
{
	subsets_t subsets;
	for(int i = 0; i < G; ++i) {
		generateAllSubsetsWithElem(i, subsets);
	}

	object_t empty = object_t();
	subsets.insert(empty);
	return subsets;
}

feature_t allFeatures()
{
	feature_t features;
	for(char c = 'a'; c < 'a' + M; ++c) {
		features.insert(c);
	}
	return features;
}

object_t allObjects()
{
	object_t objects;
	for(int i = 0; i < G; ++i){
		objects.insert(i);
	}
	return objects;
}

object_t closeSet(feature_t features, const matrix_t& matrix)
{
	object_t objects = allObjects();

	if(!features.empty()) {
		for(int i = 0; i < G; i++) {
			for(auto feature: features) {
				if(!matrix[i][feature - 'a']) {
					objects.erase(i);
				}
			}
		}
	}

	return objects;
}

feature_t closeSet(object_t objects, const matrix_t& matrix)
{
	feature_t features = allFeatures();

	if(!objects.empty()) {
		for(int i = 0; i < M; i++) {
			for(auto object: objects) {
				if(!matrix[object][i]) {
					features.erase('a' + i);
				}
			}
		}
	}

	return features;
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::set<T>& set) // print set<int> and set<char>
{
	char c = '{';
	if(set.empty()){
		out << c << ' ';
	}
	else {
		for(auto s: set) {
			out << c << ' ' << s;
			c = ',';
		}
	}
	return out << " }";
}

std::ostream& operator << (std::ostream& out, const std::set<formal_concept_t>& fc) // print FC
{
	char c = '(';
	if(fc.empty()) {
		out << c << ' ';
	}
	else {
		for(auto concept: fc) {
			out << c << concept.first << ", " << concept.second;
			c = '\n';
		}
	}
	return out << ")";
}

matrix_t setMatrix()
{
	const bool X = true, O = false;
	return {{ {X, X, X, X, X, O, X, X, X},
			  {O, X, O, X, O, O, O, O, X},
			  {O, X, O, X, O, O, O, O, X},
			  {O, O, O, X, O, O, O, O, O},
			  {O, X, X, X, X, X, O, O, X},
			  {O, X, O, O, O, O, O, O, O},
			  {X, X, X, X, X, X, X, O, X},
			  {X, O, X, O, O, O, X, X, X},
			  {X, X, O, X, O, X, O, O, X},
			  {O, X, X, X, X, X, O, O, X},
			  {X, X, O, X, O, O, O, X, X},
			  {X, X, X, X, O, O, X, X, X},
			  {X, X, O, X, O, X, X, O, X} }};
}

void run(const matrix_t& matrix)
{
	subsets_t objectsSubsets = generateAllObjectsSubsets();
	std::set<formal_concept_t> formalConcepts;


	for(const object_t A: objectsSubsets) {
		feature_t B = closeSet(A, matrix);
		if(A == closeSet(B, matrix)) {
			formal_concept_t new_fc = std::make_pair(A, B);
			formalConcepts.insert(new_fc);
		}
	}

	std::cout << "Total formal consepts: " << formalConcepts.size() << std::endl;
	std::cout << "List of formal consepts: \n" << formalConcepts << std::endl;
}

int main(int argc, char** argv)
{
	try {
			matrix_t matrix = setMatrix();
			run(matrix);

		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "find_formal_concepts: " << e.what() << std::endl;
		return 1;
	}
}