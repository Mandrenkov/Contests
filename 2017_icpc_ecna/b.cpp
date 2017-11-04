// Author: Mikhail Andrenkov
// Date: November 4, 2017
// Problem: ICPC ECNA 2017 [B]
//____________________________________________________________

#include <cassert>
#include <iostream>
#include <map>
#include <math.h>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Common Utilities
//____________________________________________________________

// Constants
constexpr bool debug = false;
constexpr float EPSILON = 1E-10;
constexpr long long MOD = 1000000007;
constexpr int MAX_INT = 2147483647;
constexpr long long MAX_LONG = 9223372036854775807;

// Macros
#define _pop pop_back
#define _push emplace_back
#define _top back

// Functions
template <typename T>
ostream& operator<<(ostream& out, const vector<T> &v) {
    if (v.size() == 0) {
        return out << "[]";
    }

    out << '[' << v[0];
    for (int i = 1; i < v.size(); ++i) {
        out << ' ' << v[i];
    }
    return out << ']';
}

// Solution
//____________________________________________________________

// Classes

// Meteor represents a crater.
struct Meteor {
    // The X coordinate of the Meteor center.
    int x;
    // The Y coordinate of the Meteor center.
    int y;
    // The radius of the Meteor.
    int r;
};

// Point represent a point along a Meteor's perimeter.
struct Point {
    // The X coordinate of the Point. 
    long double x;
    // The Y coordinate of the Point.
    long double y;
    // The angle of this Point relative to the associated Meteor.
    long double a;
    // The Meteor associated with this Point.
    Meteor* meteor;

    // Returns the angle between this Point and the given Point in the range [0, 2*PI].
    long double angle(const Point& point) const {
        long double dx = point.x - x;
        long double dy = point.y - y;
        long double a = atan2(dy, dx);
        a = fmod(a + 2*M_PI, 2*M_PI);
        return a;
    }

    // Returns the distance between this Point and the given Point.
    long double to(const Point& point) const {
        long double dx = point.x - x;
        long double dy = point.y - y;
        return sqrt(dy*dy + dx*dx);
    }

    // Returns true if the cross product between the lines formed from |before| to |this| and
    // |this| to |after| is positive.
    bool bend(const Point& before, const Point& after) const {
        // Prevent extreme anomalies from becoming part of the convex hull.
        if (before.to(after) < EPSILON && to(before) > EPSILON) {
            return true;
        }

        long double x1 = x - before.x;
        long double y1 = y - before.y;

        long double x2 = after.x - x;
        long double y2 = after.y - y;

        return x1*y2 - x2*y1 >= 0;
    }

    // Returns true if this Point lies to the left of the given Point.
    bool operator<(const Point& p) const {
        return x < p.x;
    }
};

// Adds the given Point to the provided output stream and returns the stream.  
ostream& operator<<(ostream& out, const Point& p) {
    return out << '(' << p.x << ", " << p.y << ", " << p.meteor->r << ')';
}

// Declarations

// Functions

// Returns a list of points representing the convex hull of the given Points.
// The convex hull is calculated using Andrew's algorithm.
vector<Point*> convex(vector<Point>& points) {
    vector<Point*> hull = {&points[0], &points[1]};

    // Top side of the convex hull (left to right).
    for (int i = 2; i < points.size(); ++i) {
        Point& p = points[i];
        while (hull.size() > 1 && hull.back()->bend(*hull[hull.size() - 2], p)) {
            hull.pop_back();
        }
        hull.push_back(&p);
    }

    // Bottom side of the convex hull (right to left).
    for (int i = points.size() - 2; i >= 0; --i) {
        Point& p = points[i];
        while (hull.size() > 1 && hull.back()->bend(*hull[hull.size() - 2], p)) {
            hull.pop_back();
        }
        hull.push_back(&p);
    }

    return hull;
}

// Execution entry point.
int main() {
    int N;
    cin >> N;

    vector<Meteor> meteors(N);
    for (int i = 0; i < N; ++i) {
        int x, y, r;
        cin >> x >> y >> r;
        r += 10;
        meteors[i] = Meteor{x, y, r};
    }

    // Angle between consecutive points on a Meteor's perimeter.
    long double delta = 2*M_PI/3087;

    // Initialize points along the perimeter of each Meteor.
    vector<Point> points;
    for (int i = 0; i < N; ++i) {
        Meteor& meteor = meteors[i];
        for (long double angle = 0; angle < 2*M_PI; angle += delta) {
            long double x = meteor.x + meteor.r*cos(angle);
            long double y = meteor.y + meteor.r*sin(angle);
            Point p = Point{x, y, angle, &meteor};
            points.push_back(p);
        }
    }
    sort(points.begin(), points.end());

    // Compute the initial convex hull.
    vector<Point*> hull = convex(points);

    // Iteratively focus on the critical Points of the convex hull.
    // A critical Point is a Point that is an endpoint of a line that connects two different Meteors.
    for (int i = 0; i < 7; ++i) {
        int children = 5;
        long double range = delta/2;
        delta /= children;
        range -= delta;

        // Spawn child Points around each critical point of the current convex hull.
        vector<Point> new_points;
        for (int j = 0; j < hull.size() - 1; ++j) {
            Point& p_hull = *hull[j];
            Meteor& meteor = *(p_hull.meteor);
            if (j > 0 && hull[j - 1]->meteor == &meteor && hull[j + 1]->meteor == &meteor) {
                continue;
            }
            for (long double angle = p_hull.a - range; angle <= p_hull.a + range; angle += delta) {
                long double x = meteor.x + meteor.r*cos(angle);
                long double y = meteor.y + meteor.r*sin(angle);
                long double pos_angle = fmod(angle + 2*M_PI, 2*M_PI);
                Point p = Point{x, y, pos_angle, &meteor};
                new_points.push_back(p);
            }
        }
        points = new_points;
        sort(points.begin(), points.end());

        // Compute the refined convex hull.
        hull = convex(points);
    }

    // Calculate the perimeter of the convex hull.
    long double fence = 0.0;
    for (int i = 0; i < hull.size() - 1; ++i) {
        Point* start = hull[i];
        Point* end   = hull[i + 1];

        // The |start| and |end| points belong to different Meteors. 
        if (start->meteor != end->meteor) {
            fence += start->to(*end);
            //cerr << "Line from " << *start << " to " << *end << ".\n";
        // The |start| and |end| points belong to the same Meteor.
        } else {
            // Determine the next Point that belongs to a different Meteor. 
            int next = i + 2;
            while (next < hull.size() && start->meteor == hull[next]->meteor) {
                ++next;
            }
            end = hull[next - 1];
            //cerr << "Curve from " << *start << " to " << *hull[next - 1] << ".\n"s;

            // Compute the angle of the traversed arc between the |start| point and the new |end| point. 
            long double a = 2*M_PI - fmod(end->a - start->a + 2*M_PI, 2*M_PI);

            fence += start->meteor->r*a;
            i = next - 2;
        }
    }

    printf("%.10Lf\n", fence);

    return 0;
}