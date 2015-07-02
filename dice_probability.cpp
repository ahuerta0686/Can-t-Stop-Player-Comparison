#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

#include "dice_probability.h"

int const dice_probability::NUM_FOUR_OF_A_KIND			= 1;
int const dice_probability::NUM_THREE_OF_A_KIND			= 4;
int const dice_probability::NUM_TWO_PAIRS				= 6;
int const dice_probability::NUM_ONE_PAIR				= 12;
int const dice_probability::NUM_ALL_DIFFERENT			= 24;

int const dice_probability::NUM_DICE_ROLLS				= 1296;

void dice_probability::sort_dice(string& s) {
	vector<char> dv = {s[0], s[1], s[2], s[3]};
	sort(dv.begin(), dv.end());

	s = {dv[0], dv[1], dv[2], dv[3], 0};
}



int dice_probability::num_matching_pairs(vector<pair<int, int> > v, int a, int b, int c) {
	int output = 0;
	for (pair<int, int> p : v) {
		if (p.first == a) {
			output = 1;
			if (p.second == b || p.second == c)
				return 2;
		}
		else if (p.first == b) {
			output = 1;
			if (p.second == a || p.second == c)
				return 2;
		}
		else if (p.first == c) {
			output = 1;
			if (p.second == a || p.second == b)
				return 2;
		}
		else if (p.second == a) {
			output = 1;
			if (p.first == b || p.first == c)
				return 2;
		}
		else if (p.second == b) {
			output = 1;
			if (p.first == a || p.first == c)
				return 2;
		}
		else if (p.second == c) {
			output = 1;
			if (p.first == a || p.first == b)
				return 2;
		}
	}
	return output;
}



dice_probability::dice_probability() {
	// All different
	for (int i = 1; i <= 3; i++) {
		for (int j = i + 1; j <= 4; j++) {
			for (int k = j + 1; k <= 5; k++) {
				for (int l = k + 1; l <= 6; l++) {
					string key = to_string(i) + to_string(j) + to_string(k) + to_string(l);
					sort_dice(key);
					// cout << key << endl;

					unique_dice[key].push_back(make_pair(i+j, k+l));
					unique_dice[key].push_back(make_pair(i+k, j+l));
					unique_dice[key].push_back(make_pair(i+l, j+k));

					all_different[key].push_back(make_pair(i+j, k+l));
					all_different[key].push_back(make_pair(i+k, j+l));
					all_different[key].push_back(make_pair(i+l, j+k));
				}
			}
		}
	}

	// 1 Pair
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j<= 5; j++) {
			for (int k = j + 1; k <= 6; k++) {
				if (i != j && i != k) {
					string key = to_string(i) + to_string(i) + to_string(j) + to_string(k);
					sort_dice(key);
					// cout << key << endl;

					unique_dice[key].push_back(make_pair(i+i, j+k));
					unique_dice[key].push_back(make_pair(i+j, i+k));

					one_pair[key].push_back(make_pair(i+i, j+k));
					one_pair[key].push_back(make_pair(i+j, i+k));
				}
			}
		}
	}

	// 2 Pair
	for (int i = 1; i <= 5; i++) {
		for (int j = i + 1; j <= 6; j++) {
			string key = to_string(i) + to_string(i) + to_string(j) + to_string(j);
			sort_dice(key);
			// cout << key << endl;

			unique_dice[key].push_back(make_pair(i+i, j+j));
			unique_dice[key].push_back(make_pair(i+j, i+j));

			two_pairs[key].push_back(make_pair(i+i, j+j));
			two_pairs[key].push_back(make_pair(i+j, i+j));
		}
	}

	// 3-of-a-kind
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			if (i != j) {
				string key = to_string(i) + to_string(i) + to_string(i) + to_string(j);
				sort_dice(key);
				// cout << key << endl;

				unique_dice[key].push_back(make_pair(i+i, i+j));

				three_of_a_kind[key].push_back(make_pair(i+i, i+j));
			}
		}
	}

	// 4-of-a-kind
	for (int i = 1; i <= 6; i++) {
		string key = to_string(i) + to_string(i) + to_string(i) + to_string(i);
		sort_dice(key);
		// cout << key << endl;

		unique_dice[key].push_back(make_pair(i+i, i+i));

		four_of_a_kind[key].push_back(make_pair(i+i, i+i));
	}

	// All dice
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			for (int k = 1; k <= 6; k++) {
				for (int l = 1; l <= 6; l++) {
					string key = to_string(i) + to_string(j) + to_string(k) + to_string(l);
					//sort_dice(key);

					all_dice[key].push_back(make_pair(i+j, k+l));
					all_dice[key].push_back(make_pair(i+k, j+l));
					all_dice[key].push_back(make_pair(i+l, j+k));
				}
			}
		}
	}
}

double dice_probability::get_probability(int a, int b, int c) {
	double output = 0;
	for (const auto& kv : four_of_a_kind) {
		if (num_matching_pairs(kv.second, a, b, c) > 0)
			output += NUM_FOUR_OF_A_KIND;
	}

	for (const auto& kv : three_of_a_kind) {
		if (num_matching_pairs(kv.second, a, b, c) > 0)
			output += NUM_THREE_OF_A_KIND;
	}

	for (const auto& kv : two_pairs) {
		if (num_matching_pairs(kv.second, a, b, c) > 0)
			output += NUM_TWO_PAIRS;
	}

	for (const auto& kv : one_pair) {
		if (num_matching_pairs(kv.second, a, b, c) > 0)
			output += NUM_ONE_PAIR;
	}

	for (const auto& kv : all_different) {
		if (num_matching_pairs(kv.second, a, b, c) > 0)
			output += NUM_ALL_DIFFERENT;
	}

	output /= NUM_DICE_ROLLS;
	return output;
}

double dice_probability::get_expected_progress(int a, int b, int c) {
	double output = 0;
	int successful_dice_rolls = 0;
	for (const auto& kv : four_of_a_kind) {
		if (num_matching_pairs(kv.second, a, b, c) > 0) {
			output += num_matching_pairs(kv.second, a, b, c) * NUM_FOUR_OF_A_KIND;
			successful_dice_rolls += NUM_FOUR_OF_A_KIND;
		}
	}

	for (const auto& kv : three_of_a_kind) {
		if (num_matching_pairs(kv.second, a, b, c) > 0) {
			output += num_matching_pairs(kv.second, a, b, c) * NUM_THREE_OF_A_KIND;
			successful_dice_rolls += NUM_THREE_OF_A_KIND;
		}
	}

	for (const auto& kv : two_pairs) {
		if (num_matching_pairs(kv.second, a, b, c) > 0) {
			output += num_matching_pairs(kv.second, a, b, c) * NUM_TWO_PAIRS;
			successful_dice_rolls += NUM_TWO_PAIRS;
		}
	}

	for (const auto& kv : one_pair) {
		if (num_matching_pairs(kv.second, a, b, c) > 0) {
			output += num_matching_pairs(kv.second, a, b, c) * NUM_ONE_PAIR;
			successful_dice_rolls += NUM_ONE_PAIR;
		}
	}

	for (const auto& kv : all_different) {
		if (num_matching_pairs(kv.second, a, b, c) > 0) {
			output += num_matching_pairs(kv.second, a, b, c) * NUM_ALL_DIFFERENT;
			successful_dice_rolls += NUM_ALL_DIFFERENT;
		}
	}

	output /= successful_dice_rolls;
	return output;
}