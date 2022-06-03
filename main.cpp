#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

typedef boost::geometry::model::d2::point_xy<double> point;
typedef boost::geometry::model::polygon<point> boost_polygon;

void print_vertices(const boost_polygon &poly)
{
    for (auto it = boost::begin(boost::geometry::exterior_ring(poly)); it != boost::end(boost::geometry::exterior_ring(poly)); ++it)
    {
        double x = boost::geometry::get<0>(*it);
        double y = boost::geometry::get<1>(*it);
        std::cout << "Vertices: " << x << " " << y << std::endl;
    }
}

void save_svg(const boost_polygon& poly1, const boost_polygon& poly2)
{
    std::ofstream svg_file("main.svg");
    boost::geometry::svg_mapper<point> mapper(svg_file, 500, 500);
    mapper.add(poly1);
    mapper.add(poly2);
    mapper.map(poly1, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:1");
    mapper.map(poly2, "fill-opacity:0.5;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:1");
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    // Declare strategies
    const double buffer_distance = 0.5;
    const int points_per_circle = 1;
    const float max_spike_len = 0.5;
    boost::geometry::strategy::buffer::distance_symmetric<double> distance_strategy(buffer_distance);
    // boost::geometry::strategy::buffer::join_round join_strategy(points_per_circle);
    boost::geometry::strategy::buffer::join_miter join_strategy(max_spike_len); // sharp edges, param limits overlong spikes
    boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
    boost::geometry::strategy::buffer::point_circle circle_strategy(points_per_circle);
    boost::geometry::strategy::buffer::side_straight side_strategy;

    // Declare output
    boost::geometry::model::multi_polygon<boost_polygon> result;

    boost_polygon input;
    // fill in CLOCKWISE
    boost::geometry::append(input.outer(), point(5, 0));
    boost::geometry::append(input.outer(), point(0, -1));
    boost::geometry::append(input.outer(), point(-5, 0));
    boost::geometry::append(input.outer(), point(0, 1));
    boost::geometry::append(input.outer(), point(5, 0));

    // print_vertices(input);

    std::string reason;
    if (!boost::geometry::is_valid(input, reason))
        std::cout << "Input is not valid: " << reason << "\n";

    boost::geometry::buffer(input, result,
                            distance_strategy, side_strategy,
                            join_strategy, end_strategy, circle_strategy);


    auto duration = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Runtime: " << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " us" << std::endl;
    std::cout << "\nResult:  \n";
    print_vertices(result[0]);

    save_svg(input, result[0]);
    return 0;
}