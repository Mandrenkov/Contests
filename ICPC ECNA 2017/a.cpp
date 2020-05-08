// Author: Mikhail Andrenkov
// Date: November 19, 2017
// Problem: ICPC ECNA 2017 [A]
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
constexpr double EPSILON = 1E-10;
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

inline bool equals(double a, double b) {
    return abs(a - b) < EPSILON;
}

// Solution
//____________________________________________________________

// Classes

// Point represents a geometric point on the Cartesian plane.
struct Point {
    // The x-coordinate of the Point.
    double x = 0;
    // The y-coordinate of the Point.
    double y = 0;

    // Constructor that creates an empty Point.
    Point() : x(-1), y(-1) {}

    // Constructor that initializes the coordinates of this Point.
    Point(double x, double y) : x(x), y(y) {}

    // Returns the Point located at the midpoint between this Point and the given Point.
    Point average(const Point& p) const {
        return Point{(x + p.x)/2.0, (y + p.y)/2.0};
    }

    // Returns the cross product between this Point and the given Point (interpreted as geometric vectors).
    double operator*(const Point& p) const {
        return x*p.y - p.x*y;
    }

    // Returns true if this Point lies to the left of the given Point.
    // If the two Points share the same x-coordinate, the lower Point is preferred. 
    bool operator<(const Point& point) const {
        return x < point.x || (x == point.x && y < point.y);
    }

    // Returns true if this Point and the given Point represent the same geometric location.
    bool operator==(const Point& p) const {
        return equals(x, p.x) && equals(y, p.y);
    }

    // Returns true if this Point and the given Point represent different geometric locations.
    bool operator!=(const Point& point) const {
        return !equals(x, point.x) || !equals(y, point.y);
    }

    // Returns the given stream with a representation of the given Point.
    friend ostream& operator<<(ostream& out, const Point& p) {
        out << "(" << p.x << ", " << p.y << ")";
    }
};

// An invalid Point object.
Point nullpoint;

// Line represents a geometric line segment.
struct Line {
    // The starting Point of this Line.
    Point start;
    // The ending Point of this Line.
    Point end;

    // Determines whether this Line is a vertical line.
    bool vertical;

    // The slope of this line.
    double m = -1;
    // The y-intercept of this Line.
    double b = -1;

    // The minimum x-coordinate of this Line.
    double min_x = -1;
    // The minimum y-coordinate of this Line.
    double min_y = -1;
    // The maximum x-coordinate of this Line.
    double max_x = -1;
    // The maximum y-coordinate of this Line.
    double max_y = -1;

    // The ID of the Polygon this Line belongs to.
    int polygonID = -1;
    // The ID that identifies this Line within the associated Polygon's border.
    int lineID = -1;

    // Constructor that creates an undefined Line.
    Line() {}

    // Constructor that creates a Line with the given start and end Points.
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

    // Constructor that creates a Line with the given start and end Points,
    // as well as the provided Polygon and Line IDs.
    Line(Point& start, Point& end, int polygonID, int lineID) : Line(start, end) {
        this->polygonID = polygonID;
        this->lineID = lineID;
    }

    // Returns true if the given Point lies along this Line.
    bool has(Point& p) const {
        // Check if the Point is the start or end of this Line.
        if (p == start || p == end) {
            return true;
        // Check if the point lies along this vertical Line.
        } else if (vertical) {
            if (min_x == p.x && min_y <= p.y && p.y <= max_y) {
                return true;
            }
        // Check if the Point lies along this non-vertical Line.
        } else if (min_x <= p.x && p.x <= max_x) {
            double y = m*p.x + b;
            if (equals(y, p.y)) {
                return true;
            }
        }
        return false;
    }

    // Return true if this Line is parallel to the given Line.
    inline bool parallel(const Line& line) const {
        return vertical == line.vertical && equals(m, line.m);
    }

    // Returns true if the given Point is an endpoint of this Line.
    inline bool vertex(const Point& p) const {
        return p == start || p == end;
    }

    // Returns the intersection Point between this Line and the given Line.
    // If the lines do not meaningfully intersect, |nullpoint| is returned.
    Point operator*(const Line& line) const {
        if (parallel(line)) {
            return nullpoint;
        }

        if (vertical) {
            if (!line.vertical) {
                // |  /
                // | /
                // |/
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
            if (line.vertical) {
                //   /|
                //  / |
                // /  |                
                double x = line.min_x;
                if (min_x <= x && x <= max_x) {
                    double y = m*x + b;
                    if (line.min_y <= y && y <= line.max_y &&
                        min_y <= y && y <= max_y) {
                        return Point{x, y};                   
                    }                
                }
            } else {
                //  \  /
                //   \/
                //   /
                double x = (line.b - b)/(m - line.m);
                double y = m*x + b;
                if (min_x <= x && x <= max_x &&
                    line.min_x <= x && x <= line.max_x) {
                    return Point{x, y};
                }
            }
        }
        return nullpoint;
    }

    // Returns the evaluation of this Line at the given x-coordinate.
    double operator[](const double x) const {
        return m*x + b;
    }

    // Returns true if this Line represents the same geometric line as the given Line.
    bool operator==(const Line& line) const {
        return start == line.start && end == line.end;
    }

    // Returns the given stream with a representation of the given Line.
    friend ostream& operator<<(ostream& out, const Line& line) {
        Point p1 = line.start < line.end ? line.start : line.end;
        Point p2 = line.end < line.start ? line.start : line.end;

        out << "[" << p1 << " --> " << p2 << "]" << " (" << line.polygonID << ")";
    }
};

// Polygon represents a geometric Polygon.
struct Polygon {
    // Counter variable used to assign unique IDs to Polygons.
    static int idCounter;

    // The ID of this Polygon.
    int id = -1;
    // The number of sides of this Polygon.
    int N = 0;
    // The Points defining the vertices of this Polygon.
    vector<Point> points;
    // The Lines defining the border of this Polygon.
    vector<Line> lines;
    // The area of this Polygon.
    double area = -1;

    // Constructs an empty Polygon.
    Polygon() {
        id = idCounter++;
    }

    // Returns a list of triangle Polygons that constitute this Polygon.
    vector<Polygon> triangulate() {
        vector<Point> points = this->points;
        vector<Polygon> triangles;

        for (int i = 0; points.size() > 2; ++i) {
            Point& p1 = points[i % points.size()];
            Point& p2 = points[(i + 1) % points.size()];
            Point& p3 = points[(i + 2) % points.size()];

            Point avg = p1.average(p3);
            // Check if the Polygon exists between |p1| and |p3|.
            if (this->has(avg)) {
                int intersections = 0;
                Line base{p1, p3};
                for (auto& line : lines) {
                    Point p = base*line;
                    if (p != nullpoint && p != p1 && p != p3) {
                        ++intersections;
                    }
                }

                // Check if the Polygon only intersects the line between |p1| and |p3| at |p1| and |p3|.
                if (intersections == 0) {
                    Polygon triangle = Polygon();
                    triangle += p1;
                    triangle += p2;
                    triangle += p3;
                    triangle.close();
                    triangles.push_back(triangle);

                    // Point |p2| can no longer be a member of any other triangle.
                    int mid_point = (i + 1) % points.size();
                    points.erase(points.begin() + mid_point);
                }
            }
        }
        return triangles;
    }

    // Closes this Polygon by constructing the border around this Polygon and calculates its area.
    // The area of this Polygon is computed using the shoelace formula. 
    void close() {
        N = points.size();
        area = 0;
        for (int i = 0; i < N; ++i) {
            Point& p1 = points[i];
            Point& p2 = points[(i + 1) % N];
            area += p1*p2;

            Line line(p1, p2, id, i);
            lines.push_back(line);
        }
        area /= 2;

        if (area > 0) {
            reverse(points.begin(), points.end());
        }
    }

    // Returns the area of this Polygon.
    double getArea() const {
        return abs(area);
    }

    // Returns true if this Polygon contains the given Point.
    bool has(Point& p) {
        // Check if the Point lies along the border of this Polygon.
        if (any_of(lines.begin(), lines.end(), [&](const Line& line) {
            return line.has(p);
        })) {
            return true;
        };

        Point& start = p;
        Point end = Point{p.x + 10001, p.y + 1};
        Line base = Line{start, end}; 

        // Check if the Point lies strictly within this Polygon.
        bool in = false;
        for (auto& line : lines) {
            if (base*line != nullpoint) {
                in = !in;
            }
        }
        return in;
    }

    // Adds the given Point to this Polygon.
    Polygon& operator+=(Point& p) {
        points.push_back(p);
        return *this;
    }

    // Returns the given stream with a representation of the given Polygon.
    friend ostream& operator<<(ostream& out, const Polygon& poly) {
        out << "[" << poly.id << ": " << poly.points[0];
        for (int i = 1; i < poly.N; ++i) {
            out << " -> " << poly.points[i];
        }
        return out << "] = " << poly.getArea();
    }
};

// Event represents the creation, termination, or intersection of a Line.
struct Event {
    // The type of event.
    //   '+' refers to the creation of a Line.
    //   '-' refers to the termination of a Line.
    //   'x' refers to the intersection of two Lines.
    char code;
    // The Line associated with the Event.
    Line line;
    // The Point at which this Event occurs.
    Point point;

    // Constructs an Event using the given parameters.
    Event(const char code, const Line& line, const Point& point) : code(code), line(line), point(point) {}

    // Returns an Event representing the creation of the given Line and the provided Point. 
    static Event create(const Line& line, const Point& p) {
        return Event('+', line, p);
    }

    // Returns an Event representing the termination of the given Line and the provided Point.
    static Event terminate(const Line& line, const Point& p) {
        return Event('-', line, p);
    }

    // Returns an Event representing the intersection of two Lines at the given Point.
    static Event intersect(const Point& p) {
        return Event('x', Line{}, p);
    }

    // Returns true if this Event occurs before the given Event (depending on the ordering of the Points). 
    bool operator<(const Event& event) const {
        return point < event.point;
    }

    // Returns true if this Event represents the same Event as the given Event.
    bool operator==(const Event& event) const {
        return code == event.code && point == event.point && line == event.line;
    }

    // Returns the given stream with a representation of the given Event.
    friend ostream& operator<<(ostream& out, const Event& event) {
        return out << event.code << " @" << event.point << " " << event.line;
    }
};

namespace std {
    // Template specialization to return the hash of a Point.
    template <> struct hash<Point> {
        float operator()(const Point& p) const {
            return p.x*10000 + p.y;
        }
    };

    // Template specialization to return the hash of a Line.
    template <> struct hash<Line> {
        double operator()(const Line& line) const {
            return line.polygonID*10 + line.lineID;
        }
    };

    // Template specialization to return the hash of an Event.
    template <> struct hash<Event> {
        double operator()(const Event& event) const {
            int c;
            if (event.code == '+') {
                c = 1;
            } else if (event.code == '-') {
                c = 2;
            } else {
                c = 3;
            }
            return (static_cast<double>(hash<Point>{}(event.point))*MAX_INT) + 10*hash<Line>{}(event.line) + c;
        }
    };
}

// Declarations

// Initialize the Polygon ID counter to 0.
int Polygon::idCounter = 0;

// Functions

// Returns the total Paint used to draw the given Polygons.
double getPaint(vector<Polygon>& triangles) {
    double paint = 0;
    for (const auto& tri : triangles) {
        paint += tri.getArea();
    }
    return paint;
}

// Returns the total Canvas area covered by the given Polygon triangles.
// This function uses a line-sweeping algorithm to compute the union area.
double getCanvas(vector<Polygon>& triangles) {
    vector<Event> events;

    // Instantiate the create and terminate Events.
    for (const auto& tri : triangles) {
        for (const auto& line : tri.lines) {
            // Vertical Lines do not bound a Polygon triangle in the left-to-right sweep.
            if (line.vertical) {
                continue;
            }

            // The creation Point is determined by the leftmost Point of the Line. 
            Point left = line.start < line.end ? line.start : line.end;
            events.push_back(Event::create(line, left));

            // The termination Point is determined by the rightmost Point of the Line.
            Point right = line.end < line.start ? line.start : line.end;
            events.push_back(Event::terminate(line, right));
        }
    }

    // Instantiate the intersection Events.
    for (int i = 0; i < triangles.size(); ++i) {
        Polygon& t1 = triangles[i];
        for (int j = i + 1; j < triangles.size(); ++j) {
            Polygon& t2 = triangles[j];
            for (const auto& l1 : t1.lines) {
                for (const auto& l2 : t2.lines) {
                    Point p = l1*l2;
                    // There is no value in adding intersection Points where Events already exist.
                    if (p != nullpoint && !l1.vertex(p) && !l2.vertex(p)) {
                        events.push_back(Event::intersect(p));
                    }
                }
            }
        }
    }
    // Sort the events by the order of their Points.
    sort(events.begin(), events.end());

    // Associate each x-coordinate with a vector of corresponding Events. 
    map<double, vector<Event>> event_map;
    for (const auto& event : events) {
        event_map[event.point.x].push_back(event);
    }

    // Maintain a set of active Lines through the sweep.
    unordered_set<Line> active_lines;
    double prev_x = -1;
    double canvas = 0;
    for (auto& pair : event_map) {
        double x = pair.first;
        vector<Event>& new_events = pair.second;

        // Sort the active Lines in increasing value at |x|.
        // If two Lines share the value at |x|, sort them based on their value at |prev_x|.
        vector<Line> lines;
        for (const auto& line : active_lines) {
            lines.push_back(line);
        }
        sort(lines.begin(), lines.end(), [&](const Line& l1, const Line& l2) {
            double y1 = l1[x];
            double y2 = l2[x];
            return equals(y1, y2) ? l1[prev_x] < l2[prev_x] : y1 < y2;
        });

        // Calculate the union area between this x-coordinate and the previous x-coordinate
        // based on the set of active Lines.
        double min_prev_y = 1001;
        double max_prev_y = -1;
        double min_next_y = 1001; 
        double max_next_y = -1;
        // Maintain a set of Polygons that need to closed before computing an area partition.
        unordered_set<int> segment;
        for (const auto& line : lines) {
            min_prev_y = min(min_prev_y, line[prev_x]);
            max_prev_y = max(max_prev_y, line[prev_x]);
            min_next_y = min(min_next_y, line[x]);
            max_next_y = max(max_next_y, line[x]);

            int polyID = line.polygonID;
            if (segment.find(polyID) != segment.end()) {
                segment.erase(polyID);
            } else {
                segment.insert(polyID);
            }

            // The current line segment is complete. 
            if (segment.empty()) {
                // Compute the area of the resultant. quadrilateral.
                double avg_min_y = (min_prev_y + min_next_y)/2.0;
                double avg_max_y = (max_prev_y + max_next_y)/2.0;
                double delta = (avg_max_y - avg_min_y)*(x - prev_x);
                canvas += delta;

                if (DEBUG) {
                    cerr << "Adding polygon " <<
                        Point{prev_x, min_prev_y} << " --> " <<
                        Point{prev_x, max_prev_y} << " --> " <<
                        Point{x, min_next_y} << " --> " <<
                        Point{x, max_next_y} << ": ";
                    cerr << avg_min_y << " to " << avg_max_y << " x " << (x - prev_x) << " = " << delta << '\n';
                }

                min_prev_y = 1001;
                max_prev_y = -1;
                min_next_y = 1001; 
                max_next_y = -1;
            }       
        }
        assert(segment.empty());

        // Adjust the current Event set based on the Events that occur at this x-coordinate.
        for (const auto& event : new_events) {
            if (event.code == '-') {
                active_lines.erase(event.line);
            } else if (event.code == '+') {
                active_lines.insert(event.line);
            }
        }

        prev_x = x;
    }
    return canvas;
}

// Execution entry point.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    nullpoint = Point(-1, -1);

    int N;
    cin >> N;

    vector<Polygon> triangles;
    for (int i = 0; i < N; ++i) {
        int sides;
        cin >> sides;

        Polygon poly;
        for (int j = 0; j < sides; ++j) {
            double x, y;
            cin >> x >> y;

            Point p = Point{x, y};
            poly += p;
        }
        poly.close();

        for (const auto& tri : poly.triangulate()) {
            triangles.push_back(tri);
        }
    }

    double paint = getPaint(triangles);
    double canvas = getCanvas(triangles);
    printf("%.8f %.8f\n", paint, canvas);

    return 0;
}