#ifndef _DICE_PROBABILITY_H_
#define _DICE_PROBABILITY_H_

#include <map>
#include <vector>
#include <string>
using namespace std;

class dice_probability {
private:
	map<string, vector<pair<int, int> > > all_dice;
	map<string, vector<pair<int, int> > > unique_dice;
	map<string, vector<pair<int, int> > > four_of_a_kind;
	map<string, vector<pair<int, int> > > three_of_a_kind;
	map<string, vector<pair<int, int> > > two_pairs;
	map<string, vector<pair<int, int> > > one_pair;
	map<string, vector<pair<int, int> > > all_different;
	

	// Number of ways to make particular dice roll
	static int const NUM_FOUR_OF_A_KIND;
	static int const NUM_THREE_OF_A_KIND;
	static int const NUM_TWO_PAIRS;
	static int const NUM_ONE_PAIR;
	static int const NUM_ALL_DIFFERENT;

	static int const NUM_DICE_ROLLS;

	void sort_dice(string&);

	int num_matching_pairs(vector<pair<int, int> >, int, int, int);

public:
	dice_probability();

	double get_probability(int, int, int);

	double get_expected_progress(int, int, int);

};

#endif /* _DICE_PROBABILITY_H_ */