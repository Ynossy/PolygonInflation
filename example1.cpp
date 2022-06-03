/**
 * https://stackoverflow.com/questions/45067757/poor-or-incorrect-results-when-using-boost-geometry-for-polygon-buffering
 */

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/io/io.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <fstream>
#include <iostream>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<float> BoostPoint;
typedef bg::model::polygon<BoostPoint> BoostPolygon;
typedef bg::model::multi_polygon<BoostPolygon> BoostMultipolygon;

int main() {
    BoostPolygon input;
    BoostMultipolygon output;

    boost::geometry::read_wkt("POLYGON((61.2101898 81.9854202, 61.3715706 82.0616913, 61.4335442 82.1924744, 61.4778328 82.2606735, 61.5202942 82.3236465, 61.5283432 82.3527832, 61.5431557 82.4063950, 61.5221367 82.4381790, 61.3944855 82.4706116, 61.3497124 82.4679184, 61.3284111 82.4674301, 61.1539803 82.3401947, 61.1297760 82.2854843, 61.0671043 82.1489639, 61.0682831 82.0264740, 61.0667953 82.0112915, 61.0663414 82.0066376, 61.0707321 81.9976196, 61.0998306 81.9980850, 61.2101898 81.9854202))", input);
    {
        std::string reason;
        if (!bg::is_valid(input, reason))
            std::cout << "Input is not valid: " << reason << "\n";
    }
    bg::correct(input);
    {
        std::string reason;
        if (!bg::is_valid(input, reason))
            std::cout << "Input is not valid: " << reason << "\n";
        else
            std::cout << "Input is valid";
    }

    // Declare boost strategies for buffer function.
    bg::strategy::buffer::distance_symmetric<double> distance_strategy(-0.05);
    bg::strategy::buffer::join_round join_strategy;
    bg::strategy::buffer::end_round end_strategy;
    bg::strategy::buffer::point_circle point_strategy;
    bg::strategy::buffer::side_straight side_strategy;
    // Perform polygon buffering.
    bg::buffer(input, output, distance_strategy, side_strategy, join_strategy, end_strategy, point_strategy);

    {
        std::ofstream svg("output.svg");
        boost::geometry::svg_mapper<BoostPoint> mapper(svg, 400, 400);
        mapper.add(output);
        mapper.add(input);

        mapper.map(input, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
        mapper.map(output, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(202,153,0);stroke-width:2");
    }
}