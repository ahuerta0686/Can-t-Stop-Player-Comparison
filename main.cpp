#include <iostream>
#include <fstream>
#include <vector>

#include "dice_probability.h"

using namespace std;

pair<bool, vector<double> > probabilistic_decision(vector<int>);
pair<bool, vector<double> > rollout_decision(vector<int>);

vector<int> const COLUMN_HEIGHTS = {3, 5, 7, 9, 11, 13, 11, 9, 7, 5, 3};

int main() {
	ofstream states_file("states2.dat");
	ofstream eval_file("eval2.dat");

	vector<int> empty_vector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (int i = 0; i < COLUMN_HEIGHTS.size(); i++) {
		for (int j = i + 1; j < COLUMN_HEIGHTS.size(); j++) {
			for (int k = j + 1; k < COLUMN_HEIGHTS.size(); k++) {
				for (int a = 1; a <= COLUMN_HEIGHTS[i]; a++) {
					for (int b = 1; b <= COLUMN_HEIGHTS[j]; b++) {
						for (int c = 1; c <= COLUMN_HEIGHTS[k]; c++) {
							vector<int> output_vector = vector<int>(empty_vector);
							output_vector[i] = a;
							output_vector[j] = b;
							output_vector[k] = c;
							
							pair<bool, vector<double> > probabilistic_result = probabilistic_decision(output_vector);
							pair<bool, vector<double> > rollout_result = rollout_decision(output_vector);

							if (probabilistic_result.first != rollout_result.first) {
								for (int x : output_vector)
									states_file << x << " ";
								states_file << endl;

								eval_file << probabilistic_result.second[0] << " " << probabilistic_result.second[1] << " ";
								eval_file << rollout_result.second[0] << " " << rollout_result.second[1] << endl;
							}
						}
					}
				}
			}
		}
	}

	states_file.close();
	eval_file.close();

	return 0;
}

pair<bool, vector<double> > probabilistic_decision(vector<int> state) {
	dice_probability dice_p;
	int current_columns[3];
	int current_columns_progress[3];
	int counter = 0;

	// Extract progressed tokens
	for (int i = 0; i < 11; i++) {
		if (state[i] != 0) {
			current_columns[counter] = i + 2;
			current_columns_progress[counter++] = state[i];
		}
	}

	// Sanity check
	if (counter != 3)
		cout << "WTF" << endl;

	// Current progress on columns
	int a = current_columns_progress[0];
	int b = current_columns_progress[1];
	int c = current_columns_progress[2];

	// Current columns with progress
	int d = current_columns[0];
	int e = current_columns[1];
	int f = current_columns[2];

	double progress = a / COLUMN_HEIGHTS[d - 2] + b / COLUMN_HEIGHTS[e - 2] + c / COLUMN_HEIGHTS[f - 2];										// h
	double success_probability = dice_p.get_probability(d, e, f);		// p
	double expected_progress = dice_p.get_expected_progress(d, e, f);	// g

	vector<double> relative_expected_progresses;
	for (int i = 0; i < 3; i++) {
		relative_expected_progresses.push_back(expected_progress / COLUMN_HEIGHTS[current_columns[i] - 2]);
	}

	expected_progress = 0.0;
	for (double rp : relative_expected_progresses) {
		expected_progress += rp;
	}

	expected_progress /= 3;

	// FALSE is stop, TRUE is continue
	bool decision = success_probability * (progress + expected_progress) >= progress;

	// threshold (p in this case), value (h / (h + g))
	vector<double> output;
	output.push_back(success_probability);
	output.push_back(progress / (progress + expected_progress));

	return make_pair(decision, output);
}

pair<bool, vector<double> > rollout_decision(vector<int> state) {
	int current_columns[3];
	int current_columns_progress[3];
	int counter = 0;

	// Extract progressed tokens
	for (int i = 0; i < 11; i++) {
		if (state[i] != 0) {
			current_columns[counter] = i + 2;
			current_columns_progress[counter++] = state[i];
		}
	}

	// Sanity check
	if (counter != 3)
		cout << "WTF" << endl;

	// Roll random dice
	vector<int> dice;
	for (int i = 0; i < 4; i++) {
		dice.push_back(rand() % 6 + 1);
	}

	int probability = 0;
	for (int i = 0; i < 100; i++) {
		vector<int> dice_pairs;
		dice_pairs.push_back(dice[0] + dice[1]);
		dice_pairs.push_back(dice[2] + dice[3]);
		dice_pairs.push_back(dice[0] + dice[2]);
		dice_pairs.push_back(dice[1] + dice[3]);
		dice_pairs.push_back(dice[0] + dice[3]);
		dice_pairs.push_back(dice[2] + dice[1]);

		bool valid_pair = false;
		for (int x : dice_pairs) {
			for (int j = 0; j < 3; j++) {
				if (x == current_columns[j]) valid_pair = true;
			}
		}

		if (valid_pair)
			probability++;
	}

	for (int i = 0; i < 3; i++) {
		probability -= current_columns_progress[i] * 2;
	}

	bool decision = probability >= 75;

	vector<double> output;
	output.push_back(75.0 / 100.0);
	output.push_back(probability / 100.0);

	return make_pair(decision, output);
}