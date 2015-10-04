#include <vector>
#include <algorithm>

#include "sort/order_statistics.hpp"

namespace algorithms {

double FindMedianSortedArrays(std::vector<int>& A, std::vector<int>& B, int left, int right) { // [left, right]
    int n = A.size() + B.size();

    if (left > right) {
        return FindMedianSortedArrays(B, A, std::max(0, n / 2 - int(A.size())), std::min(int(B.size()), n / 2));
    }

    int i = (left + right) / 2;
    int j = (n / 2) - i - 1;

    if ((j < 0 || (j < int(B.size()) && (A[i] >= B[j]))) &&             // B[j] <= A[i] <= B[j + 1] if applicable
        (j + 1 >= int(B.size()) || (j + 1 >= 0 && A[i] <= B[j + 1]))) {
        if (n % 2 == 0) {
            int prev;
            if (i == 0 && j >= 0) {
                prev = B[j];
            } else if (i >= 0 && j < 0) {
                prev = A[i - 1];
            } else {
                prev = std::max(A[i - 1], B[j]);
            }
            return 0.5 * (A[i] + prev);
        } else {
            return A[i];
        }
    } else if ((j < 0 || (j < int(B.size()) && (A[i] >= B[j]))) &&      // A[i] > B[j], A[i] > B[j + 1]
               (j >= 0 && j + 1 < int(B.size()) && A[i] > B[j + 1])) {
        return FindMedianSortedArrays(A, B, left, i - 1);
    } else {
        return FindMedianSortedArrays(A, B, i + 1, right);
    }
}


double FindMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
    int n = nums1.size() + nums2.size();
    if (nums1.empty()) {
        return FindMedianArray(nums2);
    } else if (nums2.empty()) {
        return FindMedianArray(nums1);
    }

    return FindMedianSortedArrays(nums1, nums2, 0, nums1.size() - 1);
}

} // namespace algorithms
