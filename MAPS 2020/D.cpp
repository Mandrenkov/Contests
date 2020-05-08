#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(10);

    int T;
    cin >> T;
    while (T --> 0) {
        double N, L, D, G;
        cin >> N >> L >> D >> G;
        double padding = D * G;
        double internal_angle = (N - 2) * M_PI / N;
        double slice_area_triangle = (L / 2) * tan(internal_angle / 2) * (L / 2);
        double slice_area_rectangle = L * padding;
        double slice_area_sector = M_PI * padding * padding * (M_PI - internal_angle) / (2 * M_PI);
        double total_area = N * (slice_area_triangle + slice_area_rectangle + slice_area_sector);
        cout << total_area << '\n';
    }

    return 0;
}