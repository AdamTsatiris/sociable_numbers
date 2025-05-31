#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <numeric> // For accumulate
#include <algorithm> // for min_element

using namespace std;

// Function to compute the sum of proper divisors of a number
int sumOfProperDivisors(int n) {
    int sum = 1; // 1 is a proper divisor of all n > 1
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += i;
            int complement = n / i;
            if (complement != i) {
                sum += complement;
            }
        }
    }
    return (n == 1) ? 0 : sum;
}

// Function to generate the divisor sum sequence until it repeats or exceeds bounds
vector<int> generateSequence(int start, int maxRange, int maxLength = 10) {
    vector<int> sequence;
    set<int> seen;
    int current = start;

    while (sequence.size() < maxLength) {
        int next = sumOfProperDivisors(current);
        if (next > maxRange || next == 0) break;
        if (seen.count(next)) {
            sequence.push_back(next);
            break;
        }
        sequence.push_back(current);
        seen.insert(current);
        current = next;
    }

    return sequence;
}

// Check if a sequence is a valid sociable chain
bool isSociableChain(const vector<int>& sequence) {
    if (sequence.size() < 2) return false;
    return sequence.front() == sequence.back();
}

int main() {
    int lower, upper;
    cout << "Enter range (lower upper): ";
    cin >> lower >> upper;

    // To avoid processing same sociable/amicable numbers multiple times
    unordered_map<int, bool> processed;

    for (int i = lower; i <= upper; ++i) {
        if (processed[i]) continue;

        int sum1 = sumOfProperDivisors(i);

        // 1. Check for Perfect Numbers
        if (sum1 == i) {
            cout << "Perfect Number: " << i << " → Sequence: [" << i << "]\n";
            processed[i] = true;
        }

        // 2. Check for Amicable Numbers
        else if (sum1 > i && sum1 <= upper) {
            int sum2 = sumOfProperDivisors(sum1);
            if (sum2 == i && i != sum1) {
                cout << "Amicable Pair: [" << i << ", " << sum1 << "]\n";
                cout << "Proof: " << i << " → " << sum1 << " → " << sum2 << "\n";
                processed[i] = true;
                processed[sum1] = true;
            }
        }

        // 3. Check for Sociable Numbers (cycle of >2 numbers)
        else {
            vector<int> seq = generateSequence(i, upper);
            if (seq.size() > 2 && isSociableChain(seq)) {
                // Ensure it's a unique cycle (not rotated version)
                int minVal = *min_element(seq.begin(), seq.end());
                if (i == minVal) { // print only starting from smallest
                    cout << "Sociable Chain (length " << seq.size() - 1 << "): ";
                    for (int j = 0; j < seq.size() - 1; ++j) {
                        cout << seq[j] << " → ";
                        processed[seq[j]] = true;
                    }
                    cout << seq.front() << "\n"; // close the loop
                }
            }
        }
    }

    return 0;
}
