#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <string>
using namespace std;

// Function to calculate the sum of proper divisors of a number.
// Proper divisors exclude the number itself.
int sum_of_proper_divisors(int n) {
    if (n <= 1) {
        return 0;
    }
    int sum = 1; // 1 is always a divisor (for n>1)
    // Iterate from 2 to sqrt(n) to find divisors in pairs
    int i;
    for (i = 2; i * i < n; i++) {
        if (n % i == 0) {
            sum += i;         // Add divisor i
            sum += n / i;     // Add the complementary divisor
        }
    }
    // If n is a perfect square, add the square root once
    if (i * i == n) {
        sum += i;
    }
    return sum;
}

int main() {
    int start, end;
    cout << "Enter the start of the range: ";
    cin >> start;
    cout << "Enter the end of the range: ";
    cin >> end;

    // Validate the range
    if (start < 1) {
        start = 1;
    }
    if (end < start) {
        cout << "Invalid range." << endl;
        return 1;
    }

    const int max_chain = 100; // Maximum chain length to avoid infinite loops
    vector<bool> visited(end + 1, false); // Tracks visited numbers in the range

    cout << "Searching for perfect, amicable, and sociable numbers in the range [" 
         << start << ", " << end << "]:" << endl;

    // Iterate through each number in the range
    for (int n0 = start; n0 <= end; n0++) {
        if (visited[n0]) {
            continue; // Skip if already visited
        }

        vector<int> chain; // Stores the current chain of numbers
        unordered_set<int> seen; // Tracks numbers in the current chain for quick lookup
        int current = n0;
        bool broken_by_cycle = false;
        bool in_range = (current >= start && current <= end);

        // Build the chain until a break condition is met
        while (in_range && !visited[current] && seen.find(current) == seen.end() && chain.size() < max_chain) {
            chain.push_back(current);
            seen.insert(current);
            current = sum_of_proper_divisors(current); // Compute next number in the chain
            in_range = (current >= start && current <= end);
        }

        // Mark all numbers in the chain as visited
        for (int num : chain) {
            if (num >= start && num <= end) {
                visited[num] = true;
            }
        }

        // Skip processing if current is out of range or already visited
        if (!in_range || visited[current] || chain.size() >= max_chain) {
            continue;
        }

        // Check if the loop broke due to a cycle
        if (seen.find(current) != seen.end()) {
            // Find the start index of the cycle
            int idx = -1;
            for (int i = 0; i < chain.size(); i++) {
                if (chain[i] == current) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) continue; // Should not happen

            // Extract the cycle from the chain
            vector<int> cycle_vec(chain.begin() + idx, chain.end());
            if (current >= start && current <= end) {
                visited[current] = true; // Mark the current if in range
            }

            // Print each number in the cycle
            for (int i = 0; i < cycle_vec.size(); i++) {
                int num = cycle_vec[i];
                string kind;
                if (cycle_vec.size() == 1) {
                    kind = "perfect";
                } else if (cycle_vec.size() == 2) {
                    kind = "amicable";
                } else {
                    kind = "sociable (cycle length " + to_string(cycle_vec.size()) + ")";
                }

                // Print the cycle starting from the current number
                cout << num << ": " << kind << ", cycle: [";
                for (int j = 0; j < cycle_vec.size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << cycle_vec[(i + j) % cycle_vec.size()];
                }
                cout << "]" << endl;
            }
        }
    }

    return 0;
}
