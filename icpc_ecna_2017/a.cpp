// Author: Mikhail Andrenkov
// Date: November 5, 2017
// Problem: ICPC ECNA 2017 [A]
//
// This solution does not consider ternary Polygon intersections.
//____________________________________________________________

#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
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
constexpr bool DEBUG = false;
constexpr double EPSILON = 1E-12;
constexpr long long MOD = 1000000007;
constexpr int MAX_INT = 2147483647;
constexpr long long MAX_LONG = 9223372036854775807;

// Macros
#define _pop pop_back
#define _push emplace_back
#define _top back

// Functions
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
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

bool equals(double d1, double d2) {
    return abs(d1 - d2) < EPSILON;
}

// Classes
struct Point {
    double x = 0;
    double y = 0;

    Point() {}

    Point(double x, double y) : x(x), y(y) {}

    Point average(const Point& p) const {
        return Point{(x + p.x)/2.0, (y + p.y)/2.0};
    }

    double operator*(const Point& p) const {
        return x*p.y - p.x*y;
    }

    bool operator<(const Point& point) const {
        return x < point.x || (x == point.x && y < point.y);
    }

    bool operator>(const Point& point) const {
        return point < *this && *this != point;
    }

    bool operator>=(const Point& point) const {
        return *this == point || *this > point;
    }

    bool operator==(const Point& p) const {
        return equals(x, p.x) && equals(y, p.y);
    }

    bool operator!=(const Point& point) const {
        return !equals(x, point.x) || !equals(y, point.y);
    }

    friend ostream& operator<<(ostream& out, const Point& p) {
        out << "(" << p.x << ", " << p.y << ")";
    }
};

namespace std {
    template <>
    struct hash<Point> {
        float operator()(const Point& p) const {
            return p.x*10000 + p.y;
        }
    };
}

struct Line {
    Point start;
    Point end;

    bool vertical;

    double m = -1;
    double b = -1;

    double min_x = -1;
    double min_y = -1;
    double max_x = -1;
    double max_y = -1;

    Line(Point& start, Point& end) : start(start), end(end) {
        Point p1 = start;
        Point p2 = end;

        if (p2 < p1) {
            swap(p1, p2);
        }

        double x1 = p1.x;
        double y1 = p1.y;
        double x2 = p2.x;
        double y2 = p2.y;

        vertical = start.x == end.x; 
        if (!vertical) {
            m = (y2 - y1)/(x2 - x1);
            b = y1 - m*x1;
        }

        min_x = min(x1, x2);
        max_x = max(x1, x2);
        min_y = min(y1, y2);
        max_y = max(y1, y2);
    }

    bool collinear(Line& line) {
        if (vertical) {
            return line.vertical && equals(min_x, line.min_x);
        } else {
            return equals(m, line.m) && equals(b, line.b);
        }
    }

    Point intersection(Line& line) {
        if (vertical) {
            // |   /
            // |  /
            // | /
            if (!line.vertical) {
                double x = min_x;
                if (line.min_x <= x && x <= line.max_x) {
                    double y = line.m*x + line.b;
                    if (line.min_y <= y && y <= line.max_y &&
                        min_y <= y && y <= max_y) {
                        return Point{x, y};                   
                    }
                }
            }
        } else {
            //   / |
            //  /  |
            // /   |
            if (line.vertical) {
                double x = line.min_x;
                if (min_x <= x && x <= max_x) {
                    double y = m*x + b;
                    if (line.min_y <= y && y <= line.max_y &&
                        min_y <= y && y <= max_y) {
                        return Point{x, y};                   
                    }                
                }
            } else {
                bool parallel = abs(m - line.m) < EPSILON;
                //   / /
                //  / /
                // / /
                if (!parallel) {
                    double x = (line.b - b)/(m - line.m);
                    double y = m*x + b;
                    if (min_x <= x && x <= max_x &&
                        line.min_x <= x && x <= line.max_x) {
                        return Point{x, y};
                    }
                }
            }
        }
        return Point{-1, -1};
    }

    friend ostream& operator<<(ostream& out, const Line& line) {
        out << "[" << line.start << " --> " << line.end << "]";
    }
};

struct Path {
    string name;
    unordered_map<Point, Point> m;

    Path(string name) : name(name) {}

    bool has(Point& key) {
        return m.find(key) != m.end();
    }

    void set(Point& key, Point& value) {
        m[key] = value;
    }

    Point operator[](Point& key) {
        return m[key];
    }

    friend ostream& operator<<(ostream& out, Path& path) {
        out << "Path " << path.name << ":\n";

        vector<Point> keys;
        for (auto& pair : path.m) {
            keys.push_back(pair.first);
        }
        sort(keys.begin(), keys.end());

        for (auto& key : keys) {
            out << "\t" << key << ": " << path[key] << '\n';
        }
        return out;
    }
};

struct Criticals {
    string name;
    unordered_map<Line*, vector<Point>> m;

    Criticals(string name) : name(name) {}

    void add(Line* key, Point& value) {
        m[key].push_back(value);
    }

    vector<Point> operator[](Line* key) {
        return m[key];
    }

    friend ostream& operator<<(ostream& out, Criticals& cs) {
        out << "Criticals " << cs.name << ":\n";
        for (auto& pair : cs.m) {
            out << "\t" << pair.first << ": " << cs[pair.first] << '\n';
        }
        return out;
    }
};

struct Polygon {
    int N = 0;
    vector<Point> points;
    vector<Line> lines;
    double area = -1;

    double min_x = 1000;
    double min_y = 1000;
    double max_x = 0;
    double max_y = 0;

    Polygon(){}

    vector<Polygon*> triangulate() {
        vector<Point> points = this->points;
        vector<Polygon*> triangles;
        while (points.size() > 2) {
            for (int i = 0; i < points.size() && points.size() > 2; ++i) {
                Point& p1 = points[i];
                Point& p2 = points[(i + 1) % points.size()];
                Point& p3 = points[(i + 2) % points.size()];

                if (this->inside(p1.average(p3))) {
                    int ints = 0;
                    Line base{p1, p3};
                    for (auto& line : lines) {
                        Point p = base.intersection(line);
                        if (p.x != -1 && p != p1 && p != p3) {
                            ++ints;
                        }
                    }

                    if (ints == 0) {
                        Polygon* triangle = new Polygon();
                        *triangle += p1;
                        *triangle += p2;
                        *triangle += p3;
                        triangle->close();

                        triangles.push_back(triangle);
                        int mid_point = (i + 1) % points.size();
                        points.erase(points.begin() + mid_point);
                    }
                } 
            }
        }
        return triangles;
    }

    void fillPath(Path& fwd) {
        for (auto& line : lines) {
            fwd.set(line.start, line.end);
        }
    }

    void close() {
        N = points.size();
        area = 0;
        for (int i = 0; i < N; ++i) {
            Point& p1 = points[i];
            Point& p2 = points[(i + 1) % N];
            area += p1*p2;

            Line line{p1, p2};
            lines.push_back(line);
        }
        area /= 2;

        if (area > 0) {
            reverse(points.begin(), points.end());
        }
    }

    double getArea() const {
        return abs(area);
    }

    bool inside_border(Point& point) {
        for (const auto& line : lines) {
            if (point == line.start) {
                return true;
            }

            // |
            // x
            // |
            if (line.vertical) {
                if (point.x == line.min_x && line.min_y <= point.y && point.y <= line.max_y) {
                    return true;
                }
            //   /
            //  x
            // /
            } else if (line.min_x <= point.x && point.x <= line.max_x) {
                double y = line.m*point.x + line.b;
                if (abs(y - point.y) < EPSILON) {
                    return true;
                }
            }
        }
        return false;
    }

    bool inside_border(Point&& point) {
        return inside_border(point);
    }

    bool inside_region(Point& point) {
        Point end = Point{point.x + 10001, point.y + 1};
        Line base = Line{point, end}; 

        bool in = false;

        for (auto& line : lines) {
            Point p = base.intersection(line);
            if (p.x != -1) {
                in = !in;
            }
        }
        return in;
    }

    bool inside_region(Point&& point) {
        return inside_region(point);
    }

    bool inside(Point& point) {
        bool in = inside_border(point) || inside_region(point);
        return in;
    }

    bool inside(Point&& point) {
        return inside(point);
    }

    bool inside(Polygon& poly) {
        return all_of(points.begin(), points.end(), [&](Point& p){
            return poly.inside(p);
        });
    }

    bool box_check(Polygon& intruder) {
        return (min_x >= intruder.max_x ||
                max_x <= intruder.min_x ||
                min_y >= intruder.max_y ||
                max_y <= intruder.min_y);
    }

    double overlap(Polygon& guest) {
        Polygon& host = *this;

        if (host.box_check(guest)) {
            return 0;
        } else if (host.inside(guest)) {
            return host.getArea();
        } else if (guest.inside(host)) {
            return guest.getArea();
        }
        

        Path host_fwd("Forward [Host]");
        Path guest_fwd("Forward [Guest]");

        host.fillPath(host_fwd);
        guest.fillPath(guest_fwd);

        Criticals cs_host("Host");
        Criticals cs_guest("Guest");

        unordered_set<Point> cpoints;

        for (auto& gline : guest.lines) {
            for (auto& hline : host.lines) {
                Point p = hline.intersection(gline);

                bool in_bck, in_fwd;
                if (gline.vertical) {
                    in_bck = host.inside(Point{p.x, p.y - EPSILON});
                    in_fwd = host.inside(Point{p.x, p.y + EPSILON});
                } else {
                    double x_bck = p.x - EPSILON;
                    double x_fwd = p.x + EPSILON;

                    in_bck = host.inside(Point{x_bck, x_bck*gline.m + gline.b});
                    in_fwd = host.inside(Point{x_fwd, x_fwd*gline.m + gline.b});
                }

                if (p.x != -1) {
                    if (p != hline.start && p != hline.end) {
                        cs_host.add(&hline, p);
                        cpoints.insert(p);
                    }
                    if (p != gline.start && p != gline.end) {
                        cs_guest.add(&gline, p);
                        cpoints.insert(p);
                    }
                }
            }
        }

        for (auto& cs : {cs_host, cs_guest}) {
            Path& fwd = (cs.name == "Host") ? host_fwd : guest_fwd;

            for (auto& line_vec : cs.m) {
                Line& line = *line_vec.first;
                vector<Point> v = line_vec.second;

                bool ordered = line.start < line.end;

                sort(v.begin(), v.end(), [&](const Point p1, const Point p2) {
                    return p1 < p2 == ordered;
                });

                Point before = line.start;
                for (auto& p : v) {
                    fwd.set(before, p);
                    fwd.set(p, line.end);

                    before = p;
                }
            }
        }

        unordered_set<Point> visited;
        double overlap_area = 0;
        for (auto start : cpoints) {
            if (!guest.inside(start) || visited.find(start) != visited.end()) {
                continue;
            }
            
            Polygon poly{};

            bool valid = true;
            Point p = start;
            do {
                visited.insert(p);
                poly += p;

                Point next = host_fwd[p];
                if (!guest.inside(next)) {
                    next = guest_fwd[p];
                }
                p = next;

                valid = host.inside(p) && guest.inside(p);
            } while (valid && p != start);
            poly.close();

            valid &= poly.N > 2;

            if (valid) {
                overlap_area += poly.getArea();
            }
        }

        return overlap_area;
    }

    Polygon& operator+=(Point& p) {
        min_x = min(min_x, p.x);
        min_y = min(min_y, p.y);
        max_x = max(max_x, p.x);
        max_y = max(max_y, p.y);

        if (points.size() > 1) {
            Point& p1 = points[points.size() - 2];
            Point& p2 = points[points.size() - 1];
            Line a{p1, p2};
            Line b{p2, p};
            if (a.collinear(b)) {
                return *this;
            }
        }

        points.push_back(p);
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Polygon& poly) {
        out << "[" << poly.points[0];
        for (int i = 1; i < poly.N; ++i) {
            out << " -> " << poly.points[i];
        }
        return out << "] = " << poly.getArea();
    }
};


// Declarations

// Functions

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    vector<Polygon*> polygons;
    for (int i = 0; i < N; ++i) {
        int sides;
        cin >> sides;

        Polygon poly{};

        for (int j = 0; j < sides; ++j) {
            double x, y;
            cin >> x >> y;

            Point p = Point{x, y};
            poly += p;
        }
        poly.close();

        for (auto& tri : poly.triangulate()) {
            polygons.push_back(tri);
        }
    }

    double paint = 0;
    for (const auto& poly : polygons) {
        paint += poly->getArea();
    }

    double canvas = paint;
    for (int i = 0; i < polygons.size(); ++i) {
        for (int j = i + 1; j < polygons.size(); ++j) {
            Polygon& host = *polygons[i];
            Polygon& guest = *polygons[j];
            canvas -= host.overlap(guest);
        }
    }
 
    printf("%.8f %.8f\n", paint, canvas);

    for (auto& poly : polygons) {
        delete poly;
    }

    return 0;
}