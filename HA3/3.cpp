/* Find all formal concepts for the context presented as pattert structure */

#include <iostream>
#include <stdexcept>
#include <array>
#include <set>
#include <utility>
#include <algorithm>

#define G 6
#define M 14

typedef std::array<float, 2>						segment_t;
typedef std::array<std::array<segment_t, M>, G> 	matrix_t;
typedef std::set<unsigned int> 						object_t;
typedef std::set<object_t>							subsets_t;
typedef std::array<segment_t, M>					feature_t;
typedef std::pair< object_t, feature_t > 			formal_concept_t;


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

feature_t allFeatures(const matrix_t& matrix)
{
	feature_t features;
	segment_t feature_value;

	for(int i = 0; i < M; ++i){
		feature_value = {10000, -10000};
		for(auto object: matrix) {
			if(object[i][0] < feature_value[0]){
				feature_value[0] = object[i][0];
			}
			if(object[i][1] > feature_value[1]){
				feature_value[1] = object[i][1];
			}
			features[i] = feature_value;
		}
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
		for(int i = 0; i < G; ++i) {
			for(int j = 0; j < M; ++j) {
				if((matrix[i][j][0] < features[j][0]) || (matrix[i][j][1] > features[j][1])){
					objects.erase(i);
				}
			}
		}
	}

	return objects;
}

feature_t closeSet(object_t objects, const matrix_t& matrix)
{
	feature_t features;
	segment_t feature_value;

	if(!objects.empty()) {
		for(int i = 0; i < M; i++) {
			feature_value = {10000, -10000};
			for(auto object: objects) {
				if(matrix[object][i][0] < feature_value[0]){
					feature_value[0] = matrix[object][i][0];
				}
				if(matrix[object][i][1] > feature_value[1]){
					feature_value[1] = matrix[object][i][1];
				}
			}
			features[i] = feature_value;
		}
	}
	else {
		features = allFeatures(matrix);
	}

	return features;
}

template <typename T, size_t L>
std::ostream& operator <<(std::ostream& out, const std::array<T, L>& array) // print set<int> and set<char>
{
	char c = '[';
	if(array.empty()){
		out << c << ' ';
	}
	else {
		for(const auto e: array) {
			out << c << ' ' << e;
			c = ',';
		}
	}
	return out << " ]";
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::set<T>& set) // print set<int> and set<char>
{
	char c = '{';
	if(set.empty()){
		out << c << ' ';
	}
	else {
		for(const auto s: set) {
			out << c << ' ' << s + 1;
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
		for(const auto concept: fc) {
			out << c << concept.first << ", " << concept.second;
			c = '\n';
		}
	}
	return out << ")";
}

matrix_t setMatrix()
{
	std::array<std::array<float, M>, G> context = {{ {1, 14.23, 1.71, 2.43, 15.6, 127, 2.8, 3.06, 0.28, 2.29, 5.64, 1.04, 3.92, 1065},
    		{ 1, 13.2, 1.78, 2.14, 11.2, 100, 2.65, 2.76, 0.26, 1.28, 4.38, 1.05, 3.4, 1050},
    		{ 2, 12.37, 0.94, 1.36, 10.6, 88, 1.98, 0.57, 0.28, 0.42, 1.95, 1.05, 1.82, 520},
    		{ 2, 12.33, 1.1, 2.28, 16, 101, 2.05, 1.09, 0.63, 0.41, 3.27, 1.25, 1.67, 680},
    		{ 3, 13.78, 2.76, 2.3, 22, 90, 1.35, 0.68, 0.41, 1.03, 9.58, 0.7, 1.68, 615},
    		{ 3, 13.73, 4.36, 2.26, 22.5, 88, 1.28, 0.47, 0.52, 1.15, 6.62, 0.78, 1.75, 520} }};

    matrix_t matrix;
	segment_t seg;
	std::array<segment_t, M> features;

    for(int i = 0; i < G; ++i) {
    	for(int j = 0; j < M; ++j) {
    		seg[0] = context[i][j];
    		seg[1] = seg[0];
    		features[j] = seg;
    	}
    	matrix[i] = features;
    }
    return matrix;
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