

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <iomanip>
using std::setfill;
using std::setw;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::binary_search;
using std::max_element;
using std::max;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;

#include <algorithm>
using std::transform;

#include <iterator>
using std::back_inserter;

#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";

void MainProgram::test_get_functions(TownID id)
{
    ds_.get_town_name(id);
    ds_.get_town_coordinates(id);
    ds_.get_town_tax(id);
}

MainProgram::CmdResult MainProgram::cmd_add_town(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    TownID id = *begin++;
    string name = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    string taxstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);
    int tax = convert_string_to<int>(taxstr);

    bool success = ds_.add_town(id, name, {x, y}, tax);

    view_dirty = true;
    return {ResultType::LIST, {success ? id : NO_TOWNID}};
}

MainProgram::CmdResult MainProgram::cmd_print_town(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    TownID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    return {ResultType::LIST, {id}};
}

void MainProgram::test_print_town()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_change_town_name(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID id = *begin++;
    string newname = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.change_town_name(id, newname);

    view_dirty = true;
    return {ResultType::LIST, {success ? id : NO_TOWNID}};
}

void MainProgram::test_change_town_name()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto newname = n_to_name(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.change_town_name(id, newname);
    }
}

MainProgram::CmdResult MainProgram::cmd_add_vassalship(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID vassalid = *begin++;
    TownID taxerid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_vassalship(vassalid, taxerid);
    if (ok)
    {
        auto vassalname = ds_.get_town_name(vassalid);
        auto taxername = ds_.get_town_name(taxerid);
        output << "Added vassalship: " << vassalname << " -> " << taxername << endl;
    }
    else
    {
        output << "Adding vassalship failed!" << endl;
    }

    view_dirty = true;
    return {};
}

MainProgram::CmdResult MainProgram::cmd_add_road(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID town1id = *begin++;
    TownID town2id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_road(town1id, town2id);
    if (ok)
    {
        auto town1name = ds_.get_town_name(town1id);
        auto town2name = ds_.get_town_name(town2id);
        output << "Added road: " << (town1name.empty() ? "*" : town1name) << " <-> " << (town2name.empty() ? "*" : town2name) << endl;
    }
    else
    {
        output << "Adding road failed!" << endl;
    }

    view_dirty = true;
//    return {ResultType::PATH, {town1id, town2id}};
    return {};
}

MainProgram::CmdResult MainProgram::cmd_remove_road(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID town1id = *begin++;
    TownID town2id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.remove_road(town1id, town2id);
    if (ok)
    {
        auto town1name = ds_.get_town_name(town1id);
        auto town2name = ds_.get_town_name(town2id);
        output << "Removed road: " << (town1name.empty() ? "*" : town1name) << " <-> " << (town2name.empty() ? "*" : town2name) << endl;
    }
    else
    {
        output << "Removing road failed!" << endl;
    }

    view_dirty = true;
    return {};
}

void MainProgram::test_remove_road()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id1 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto id2 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.remove_road(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_taxer_path(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.taxer_path(id);
    if (result.empty()) { return {ResultType::HIERARCHY, {NO_TOWNID}}; }
    else { return {ResultType::HIERARCHY, result}; }
}

void MainProgram::test_taxer_path()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.taxer_path(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_longest_vassal_path(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.longest_vassal_path(id);
    if (result.empty()) { return {ResultType::HIERARCHY, {NO_TOWNID}}; }
    else { return {ResultType::HIERARCHY, result}; }
}

void MainProgram::test_longest_vassal_path()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.longest_vassal_path(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_total_net_tax(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    TownID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.total_net_tax(id);
    auto name = ds_.get_town_name(id);
    output << "Total net tax of " << name <<": ";
    if (result != NO_VALUE)
    {
        output << result << endl;
    }
    else
    {
        output << "NO_VALUE" << endl;
    }

    return {};
}

void MainProgram::test_total_net_tax()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.total_net_tax(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_towns_nearest(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto result = ds_.towns_nearest({x, y});

    return {ResultType::LIST, result};
}

void MainProgram::test_towns_nearest()
{
    int x = random<int>(1, 10000);
    int y = random<int>(1, 10000);
    ds_.towns_nearest({x, y});
}

MainProgram::CmdResult MainProgram::cmd_remove_town(ostream& output, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto name = ds_.get_town_name(id);
    bool success = ds_.remove_town(id);
    if (success)
    {
        output << name << " removed." << endl;
        view_dirty = true;
        return {};
    }
    else
    {
        return {ResultType::LIST, {NO_TOWNID}};
    }
}

void MainProgram::test_remove_town()
{
    // Choose random number to remove
    if (random_towns_added_ > 0) // Don't remove if there's nothing to remove
    {
        auto name = n_to_name(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.remove_town(name);
    }
}

void MainProgram::add_random_towns(unsigned int size, Coord min, Coord max)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        string name = n_to_name(random_towns_added_);
        TownID id = n_to_townid(random_towns_added_);

        int x = random<int>(min.x, max.x);
        int y = random<int>(min.y, max.y);
        int tax = random<int>(1, 10000);

        ds_.add_town(id, name, {x, y}, tax);

        // Add random taxer whose number is smaller
        if (random_towns_added_ > 0)
        {
            TownID taxerid = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
            ds_.add_vassalship(id, taxerid);
        }

        ++random_towns_added_;
    }
}

MainProgram::CmdResult MainProgram::cmd_random_add(ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    string minxstr = *begin++;
    string minystr = *begin++;
    string maxxstr = *begin++;
    string maxystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    Coord min{1, 1};
    Coord max{100, 100};
    if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() && !maxystr.empty())
    {
        min.x = convert_string_to<unsigned int>(minxstr);
        min.y = convert_string_to<unsigned int>(minystr);
        max.x = convert_string_to<unsigned int>(maxxstr);
        max.y = convert_string_to<unsigned int>(maxystr);
    }
    else
    {
        auto towns = ds_.all_towns();
        if (!towns.empty())
        {
            // Find out bounding box
            min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
            for (auto const& town : towns)
            {
                auto [x,y] = ds_.get_town_coordinates(town);
                if (x < min.x) { min.x = x; }
                if (y < min.y) { min.y = y; }
                if (x > max.x) { max.x = x; }
                if (y > max.y) { max.y = y; }
            }
        }
    }

    add_random_towns(size, min, max);

    output << "Added: " << size << " towns." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_add()
{
    add_random_towns(1);
}

MainProgram::CmdResult MainProgram::cmd_town_count(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of towns: " << ds_.town_count() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_all_towns(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto towns = ds_.all_towns();
    if (towns.empty())
    {
        output << "No towns!" << endl;
    }

    std::sort(towns.begin(), towns.end());
    return {ResultType::LIST, towns};
}

void MainProgram::test_all_towns()
{
    ds_.all_towns();
}

MainProgram::CmdResult MainProgram::cmd_all_roads(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto roads = ds_.all_roads();
    if (roads.empty())
    {
        output << "No roads!" << endl;
    }

    std::sort(roads.begin(), roads.end());

    unsigned long int n = 1;
    for (auto const& p : roads)
    {
        auto coord1 = ds_.get_town_coordinates(p.first);
        auto coord2 = ds_.get_town_coordinates(p.second);
        auto dist = calc_distance(coord1, coord2);
        output << n << ": " << p.first << " <-> " << p.second << " (" << dist << ")" << std::endl;
        ++n;
    }

    return {};
}

void MainProgram::test_all_roads()
{
    ds_.all_roads();
}

MainProgram::CmdResult MainProgram::cmd_town_vassals(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto vassals = ds_.get_town_vassals(id);
    std::sort(vassals.begin(), vassals.end());

    if (vassals.empty())
    {
        output << "No towns!" << endl;
    }

    return {ResultType::LIST, vassals};
}

void MainProgram::test_town_vassals()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.get_town_vassals(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_roads_from(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto vassals = ds_.get_roads_from(id);
    if (vassals.empty())
    {
        output << "No roads!" << endl;
    }

    std::sort(vassals.begin(), vassals.end());

    return {ResultType::LIST, vassals};
}

void MainProgram::test_roads_from()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.get_roads_from(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_random_roads(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string sizestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    if (ds_.town_count() < 2)
    {
        output << "Not enough towns to add roads!" << std::endl;
        return {};
    }

    unsigned int random_roads = convert_string_to<unsigned int>(sizestr);

    add_random_nonintersecting_roads(random_roads);

    output << "Added random " << random_roads << " roads." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_roads()
{
    if (random_towns_added_ > 0) // Don't do anything if there's no towns
    {
        add_random_roads(10);
    }
}

void MainProgram::add_random_roads(unsigned int n)
{
    for (unsigned int i=0; i<n; ++i)
    {
        auto id1 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto id2 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.add_road(id1, id2);
    }
}

Distance MainProgram::calc_distance(Coord c1, Coord c2)
{
    if (c1 == NO_COORD || c2 == NO_COORD) { return NO_DISTANCE; }

    long long int deltax = c1.x - c2.x;
    long long int deltay = c1.y - c2.y;
    return static_cast<Distance>(std::sqrt(deltax*deltax + deltay*deltay));
}

MainProgram::CmdResult MainProgram::cmd_random_road_network(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    if (ds_.town_count() < 2)
    {
        output << "Not enough towns to add roads!" << std::endl;
        return {};
    }

    create_road_network();

    output << "Added road network." << endl;

    view_dirty = true;

    return {};
}

//void MainProgram::test_vassals()
//{
//    // Choose random town
//    if (random_towns_added_ > 0) // Don't do anything if there are no towns
//    {
//        auto id = n_to_id(random<decltype(random_towns_added_)>(0, random_towns_added_));
//        ds_.get_vassals(id);
//    }
//}

MainProgram::CmdResult MainProgram::cmd_clear_all(ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear_all();
    init_primes();

    output << "Cleared all towns" << endl;

    view_dirty = true;

    return {};
}

string MainProgram::print_town(TownID id, ostream& output, bool nl)
{
    try
    {
        if (id != NO_TOWNID)
        {
            auto name = ds_.get_town_name(id);
            auto xy = ds_.get_town_coordinates(id);
            if (!name.empty())
            {
                output << name << ": ";
            }
            else
            {
                output << "*: ";
            }

            auto tax = ds_.get_town_tax(id);
            if (tax != NO_VALUE)
            {
                output << "tax=" << tax << ", ";
            }
            else
            {
                output << "tax=NO_VALUE, ";
            }

            output << "pos=";
            print_coord(xy, output, false);
            output << ", id=" << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_TOWNID--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing town : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing town : " << e.what() << endl;
        return "";
    }
}

MainProgram::CmdResult MainProgram::cmd_find_towns(ostream& output, MatchIter begin, MatchIter end)
{
    string name = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.find_towns(name);
    std::sort(result.begin(), result.end());

    if (result.empty())
    {
        output << "No towns found!" << std::endl;
    }

    return {ResultType::LIST, result};
}

void MainProgram::test_find_towns()
{
    // Choose random number to remove
    if (random_towns_added_ > 0) // Don't find if there's nothing to find
    {
        auto name = n_to_name(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.find_towns(name);
    }
}

MainProgram::CmdResult MainProgram::cmd_any_route(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.any_route(fromid, toid);
    if (result.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_any_route()
{
    if (random_towns_added_ > 0)
    {
        // Choose two random towns
        auto id1 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto id2 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.any_route(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_shortest_route(ostream& output, MatchIter begin, MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.shortest_route(fromid, toid);
    if (result.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_shortest_route()
{
    if (random_towns_added_ > 0)
    {
        // Choose two random towns
        auto id1 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto id2 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.shortest_route(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_least_towns_route(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.least_towns_route(fromid, toid);
    if (result.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_least_towns_route()
{
    if (random_towns_added_ > 0)
    {
        // Choose two random towns
        auto id1 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        auto id2 = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.least_towns_route(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_road_cycle_route(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.road_cycle_route(fromid);
    if (result.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    if (result.front() == NO_TOWNID)
    {
        output << "Town not found!" << endl;
        return {};
    }

    if (result.size() < 2)
    {
        output << "Too short route (" << result.size() << ") to contain cycles!" << endl;
        return {ResultType::ROUTE, result};
    }

    auto lasttown = result.back();
    auto cycbeg = std::find_if(result.begin(), result.end()-1, [lasttown](auto const& id){ return id == lasttown; });
    if (cycbeg == result.end()-1)
    {
        output << "No cycle found in returned route!";
        return {ResultType::ROUTE, result};
    }

    // If necessary, swap cycle so that it starts with smaller townid
    if ((cycbeg+1) < (result.end()-2))
    {
        auto idfirst = *cycbeg;
        auto idlast = *(result.end()-2);
        if (idlast < idfirst)
        {
           std::reverse(cycbeg+1, result.end()-1);
        }
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_road_cycle_route()
{
    if (random_towns_added_ > 0)
    {
        // Choose random town
        auto id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.road_cycle_route(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_trim_road_network(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    Distance total_dist;
    total_dist = ds_.trim_road_network();

    output << "The remaining road network has total distance of " << total_dist << std::endl;

    view_dirty = true;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_clear_roads(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    ds_.clear_roads();
    output << "All roads removed." << std::endl;

    return {};
}

void MainProgram::test_trim_road_network()
{
    ds_.trim_road_network();
}

MainProgram::CmdResult MainProgram::cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(std::ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    string silentstr =  *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool silent = !silentstr.empty();
    ostream* new_output = &output;

    ostringstream dummystr; // Given as output if "silent" is specified, the output is discarded
    if (silent)
    {
        new_output = &dummystr;
    }

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, *new_output, PromptStyle::NORMAL);
        if (silent) { output << "...(output discarded in silent mode)..." << endl; }
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(std::ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = max_element(actual_lines.cbegin(), actual_lines.cend(),
                                               [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

std::string MainProgram::print_town_name(TownID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_TOWNID)
        {
            auto name = ds_.get_town_name(id);
            if (!name.empty())
            {
                output << name;
            }
            else
            {
                output << "*";
            }

            ostringstream retstream;
            retstream << name;
            if (nl) { output << endl; }
            return retstream.str();
        }
        else
        {
            output << "--NO_TOWNID--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing town name : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing town name : " << e.what() << endl;
        return "";
    }
}

std::string MainProgram::print_coord(Coord coord, std::ostream& output, bool nl)
{
    if (coord != NO_COORD)
    {
        output << "(" << coord.x << "," << coord.y << ")";
        ostringstream retstream;
        retstream << "(" << coord.x << "," << coord.y << ")";
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_COORD--)";
        if (nl) { output << endl; }
        return "";
    }
}

string const townidx = "([a-zA-Z0-9]+)";
string const namex = "([a-zA-Z0-9-]+)";
string const numx = "([0-9]+)";
string const optcoordx = "\\([[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*[0-9]+[[:space:]]*\\)";
string const coordx = "\\([[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)[[:space:]]*\\)";
string const wsx = "[[:space:]]+";

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
{
    {"add_town", "ID Name (x,y) tax", townidx+wsx+namex+wsx+coordx+wsx+numx, &MainProgram::cmd_add_town, nullptr },
    {"random_add", "number_of_towns_to_add  (minx,miny) (maxx,maxy) (coordinates optional)",
     numx+"(?:"+wsx+coordx+wsx+coordx+")?", &MainProgram::cmd_random_add, &MainProgram::test_random_add },
    {"random_roads", "max_number_of_roads_to_add", numx,
     &MainProgram::cmd_random_roads, &MainProgram::test_random_roads },
    {"random_road_network", "", "", &MainProgram::cmd_random_road_network, nullptr },
    {"print_town", "TownID", townidx, &MainProgram::cmd_print_town, &MainProgram::test_print_town },
    {"all_towns", "", "", &MainProgram::cmd_all_towns, &MainProgram::test_all_towns },
    {"all_roads", "", "", &MainProgram::cmd_all_roads, &MainProgram::test_all_roads },
    {"town_count", "", "", &MainProgram::cmd_town_count, nullptr },
    {"clear_all", "", "", &MainProgram::cmd_clear_all, nullptr },
    {"towns_alphabetically", "", "", &MainProgram::NoParListCmd<&Datastructures::towns_alphabetically>, &MainProgram::NoParListTestCmd<&Datastructures::towns_alphabetically> },
    {"towns_distance_increasing", "", "", &MainProgram::NoParListCmd<&Datastructures::towns_distance_increasing>,
                                          &MainProgram::NoParListTestCmd<&Datastructures::towns_distance_increasing> },
    {"mindist", "", "", &MainProgram::NoParTownCmd<&Datastructures::min_distance>, &MainProgram::NoParTownTestCmd<&Datastructures::min_distance> },
    {"maxdist", "", "", &MainProgram::NoParTownCmd<&Datastructures::max_distance>, &MainProgram::NoParTownTestCmd<&Datastructures::max_distance> },
    {"towns_nearest", "(x,y)", coordx, &MainProgram::cmd_towns_nearest, &MainProgram::test_towns_nearest },
    {"remove_town", "ID", townidx, &MainProgram::cmd_remove_town, &MainProgram::test_remove_town },
    {"find_towns", "name", namex, &MainProgram::cmd_find_towns, &MainProgram::test_find_towns },
    {"change_town_name", "ID newname", townidx+wsx+namex, &MainProgram::cmd_change_town_name, &MainProgram::test_change_town_name },
    {"add_vassalship", "VassalID TaxerID", townidx+wsx+townidx, &MainProgram::cmd_add_vassalship, nullptr },
    {"town_vassals", "TownID", townidx, &MainProgram::cmd_town_vassals, &MainProgram::test_town_vassals },
    {"add_road", "Town1ID Town2ID", townidx+wsx+townidx, &MainProgram::cmd_add_road, nullptr },
    {"remove_road", "Town1ID Town2ID", townidx+wsx+townidx, &MainProgram::cmd_remove_road, &MainProgram::test_remove_road },
    {"roads_from", "TownID", townidx, &MainProgram::cmd_roads_from, &MainProgram::test_roads_from },
    {"clear_roads", "", "", &MainProgram::cmd_clear_roads, nullptr },
    {"taxer_path", "ID", townidx, &MainProgram::cmd_taxer_path, &MainProgram::test_taxer_path },
    {"longest_vassal_path", "ID", townidx, &MainProgram::cmd_longest_vassal_path, &MainProgram::test_longest_vassal_path },
    {"total_net_tax", "ID", townidx, &MainProgram::cmd_total_net_tax, &MainProgram::test_total_net_tax },
    {"any_route", "Town1ID Town2ID", townidx+wsx+townidx, &MainProgram::cmd_any_route, &MainProgram::test_any_route },
    {"shortest_route", "Town1ID Town2ID", townidx+wsx+townidx, &MainProgram::cmd_shortest_route, &MainProgram::test_shortest_route },
    {"least_towns_route", "Town1ID Town2ID", townidx+wsx+townidx, &MainProgram::cmd_least_towns_route, &MainProgram::test_least_towns_route },
    {"road_cycle_route", "TownID", townidx, &MainProgram::cmd_road_cycle_route, &MainProgram::test_road_cycle_route },
    {"trim_road_network", "", "", &MainProgram::cmd_trim_road_network, &MainProgram::test_trim_road_network },
    {"quit", "", "", nullptr, nullptr },
    {"help", "", "", &MainProgram::help_command, nullptr },
    {"read", "\"in-filename\" [silent]", "\"([-a-zA-Z0-9 ./:_]+)\"(?:"+wsx+"(silent))?", &MainProgram::cmd_read, nullptr },
    {"testread", "\"in-filename\" \"out-filename\"", "\"([-a-zA-Z0-9 ./:_]+)\""+wsx+"\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
    {"perftest", "cmd1|all|compulsory[;cmd2...] timeout repeat_count n1[;n2...] (parts in [] are optional, alternatives separated by |)",
     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)"+wsx+numx+wsx+numx+wsx+"([0-9]+(?:;[0-9]+)*)", &MainProgram::cmd_perftest, nullptr },
    {"stopwatch", "on|off|next (alternatives separated by |)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
    {"random_seed", "new-random-seed-integer", numx, &MainProgram::cmd_randseed, nullptr },
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
};

MainProgram::CmdResult MainProgram::help_command(std::ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    try {
    // Note: everything below is indented too little by one indentation level! (because of try block above)

    vector<string> optional_cmds({"remove_town", "towns_nearest", "longest_vassal_path", "total_net_tax"});
    vector<string> nondefault_cmds({"remove_town", "find_towns"});

    string commandstr = *begin++;
    unsigned int timeout = convert_string_to<unsigned int>(*begin++);
    unsigned int repeat_count = convert_string_to<unsigned int>(*begin++);
    string sizes = *begin++;
    assert(begin == end && "Invalid number of parameters");

    vector<string> testcmds;
    bool additional_get_cmds = true;
    if (commandstr != "all" && commandstr != "compulsory")
    {
        additional_get_cmds = false;
        smatch scmd;
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        for ( ; regex_search(cbeg, cend, scmd, commands_regex_); cbeg = scmd.suffix().first)
        {
            testcmds.push_back(scmd[1]);
        }
    }

    vector<unsigned int> init_ns;
    smatch size;
    auto sbeg = sizes.cbegin();
    auto send = sizes.cend();
    for ( ; regex_search(sbeg, send, size, sizes_regex_); sbeg = size.suffix().first)
    {
        init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }

    output << "Timeout for each N is " << timeout << " sec. " << endl;
    output << "For each N perform " << repeat_count << " random command(s) from:" << endl;

    // Initialize test functions
    vector<void(MainProgram::*)()> testfuncs;
    if (testcmds.empty())
    { // Add all commands
        for (auto& i : cmds_)
        {
            if (i.testfunc)
            {
                if (find(nondefault_cmds.begin(), nondefault_cmds.end(), i.cmd) == nondefault_cmds.end() &&
                    (commandstr == "all" || find(optional_cmds.begin(), optional_cmds.end(), i.cmd) == optional_cmds.end()))
                {
                    output << i.cmd << " ";
                    testfuncs.push_back(i.testfunc);
                }
            }
        }
    }
    else
    {
        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const& cmd){ return cmd.cmd == i; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i << " ";
                testfuncs.push_back(pos->testfunc);
            }
            else
            {
                output << "(cannot test " << i << ") ";
            }
        }
    }
    output << endl << endl;

    if (testfuncs.empty())
    {
        output << "No commands to test!" << endl;
        return {};
    }

#ifdef USE_PERF_EVENT
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "add (count)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "cmds (count)"  << " , " << setw(12) << "total (sec)" << " , " << setw(12) << "total (count)" << endl;
#else
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "total (sec)" << endl;
#endif
    flush_output(output);

    auto stop = false;
    for (unsigned int n : init_ns)
    {
        if (stop) { break; }

        output << setw(7) << n << " , " << flush;

        ds_.clear_all();
        ds_.clear_roads();
        init_primes();

        Stopwatch stopwatch(true); // Use also instruction counting, if enabled

        // Add random towns
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_towns(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_towns(n % 1000);
            stopwatch.stop();
        }

        // Add random roads
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_roads(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_roads(n % 1000);
            stopwatch.stop();
        }

#ifdef USE_PERF_EVENT
        auto addcount = stopwatch.count();
#endif
        auto addsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << addsec << " , " << setw(12) << addcount << " , " << flush;
#else
        output << setw(12) << addsec << " , " << flush;
#endif

        if (addsec >= timeout)
        {
            output << "Timeout!" << endl;
            stop = true;
            break;
        }

        stopwatch.start();
        for (unsigned int repeat = 0; repeat < repeat_count; ++repeat)
        {
            auto cmdpos = random(testfuncs.begin(), testfuncs.end());

            (this->**cmdpos)();
            if (additional_get_cmds)
            {
                if (random_towns_added_ > 0) // Don't do anything if there's no towns
                {
                    TownID id = n_to_townid(random<decltype(random_towns_added_)>(0, random_towns_added_));
                    ds_.get_town_name(id);
                    ds_.get_town_coordinates(id);
                    ds_.get_town_tax(id);
                }
            }

            if (repeat % 10 == 0)
            {
                stopwatch.stop();
                if (stopwatch.elapsed() >= timeout)
                {
                    output << "Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
                stopwatch.start();
            }
        }
        stopwatch.stop();
        if (stop) { break; }

#ifdef USE_PERF_EVENT
        auto totalcount = stopwatch.count();
#endif
        auto totalsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalcount-addcount << " , " << setw(12) << totalsec << " , " << setw(12) << totalcount;
#else
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;
#endif

//        unsigned long int maxmem;
//        string unit;
//        tie(maxmem, unit) = mempeak();
//        maxmem -=  startmem;
//        if (maxmem != 0)
//        {
//            output << ", memory " << maxmem << " " << unit;
//        }
        output << endl;
        flush_output(output);
    }

    ds_.clear_all();
    ds_.clear_roads();
    init_primes();

    }
    catch (NotImplemented const&)
    {
        // Clean up after NotImplemented
        ds_.clear_all();
        init_primes();
        throw;
    }

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

bool MainProgram::command_parse_line(string inputline, ostream& output)
{
//    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
//    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    if (inputline.empty()) { return true; }

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex_);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        smatch match2;
        bool matched2 = regex_match(params, match2, pos->param_regex);
        if (matched2)
        {
            if (pos->func)
            {
                assert(!match2.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

               TestStatus initial_status = test_status_;
               test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                CmdResult result;
                try
                {
                    result = (this->*(pos->func))(output, ++(match2.begin()), match2.end());
                }
                catch (NotImplemented const& e)
                {
                    output << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                    std::cerr << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                }

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                switch (result.first)
                {
                    case ResultType::NOTHING:
                    {
                        break;
                    }
                    case ResultType::LIST:
                    {
                        auto& towns = result.second;
                        if (!towns.empty())
                        {
                            if (towns.size() == 1 && towns.front() == NO_TOWNID)
                            {
                                output << "Failed (NO_... returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (TownID id : towns)
                                {
                                    ++num;
                                    if (towns.size() > 1) { output << num << ". "; }
                                    print_town(id, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::HIERARCHY:
                    {
                        auto& towns = result.second;
                        if (!towns.empty())
                        {
                            if (towns.size() == 1 && towns.front() == NO_TOWNID)
                            {
                                output << "Failed (NO_... returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (TownID id : towns)
                                {
                                    ++num;
                                    if (towns.size() > 1)
                                    {
                                        output << num << ". ";
                                        print_town_name(id, output,false);
//                                        if (num < towns.size()) { output << " ->"; }
                                    }
                                    else
                                    {
                                        print_town_name(id, output,false);
                                    }
                                    output << std::endl;
                                }
                            }
                        }
                        break;
                    }
                case ResultType::ROUTE:
                    {
                        auto& route = result.second;
                        if (!route.empty())
                        {
                            if (route.size() == 1 && route.front() == NO_TOWNID)
                            {
                                output << "Failed (NO_TOWNID returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 1;
                                Distance dist = 0;
                                Coord prev_coord = NO_COORD;
                                for (auto townid : route)
                                {
                                    output << num << ". ";
                                    print_town_name(townid, output, false);

                                    Coord coord = ds_.get_town_coordinates(townid);
                                    if (num != 1)
                                    {
                                        Distance d = calc_distance(prev_coord, coord);
                                        if (d != NO_DISTANCE && dist != NO_DISTANCE)
                                        {
                                            dist += d;
                                            output << " (distance " << dist << ")";
                                        }
                                        else
                                        {
                                            output << " (NO_DISTANCE!)";
                                            dist = NO_DISTANCE;
                                        }
                                    }
                                    prev_coord = coord;
                                    output << endl;

                                    ++num;
                                }
                            }
                        }
                        break;
                    }
                    default:
                    {
                        assert(false && "Unsupported result type!");
                    }
                }

                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec" << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
//        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(std::ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(std::ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
    init_regexs();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() < 1 || args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2 && args[1] != "--console")
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    if (mainprg.test_status_ == TestStatus::DIFFS_FOUND)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_towns_added_ = 0;
}

Name MainProgram::n_to_name(unsigned long n)
{
    unsigned long int hash = prime1_*n + prime2_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}

TownID MainProgram::n_to_townid(unsigned long n)
{
 std::ostringstream ostr;
 ostr << "R" << n;
 return ostr.str();
}

Coord MainProgram::n_to_coord(unsigned long n)
{
    unsigned long int hash = prime1_ * n + prime2_;
    hash = hash ^ (hash + 0x9e3779b9 + (hash << 6) + (hash >> 2)); // :-P

    return {static_cast<int>(hash % 1000), static_cast<int>((hash/1000) % 1000)};
}

void MainProgram::init_regexs()
{
    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto& cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;

        cmd.param_regex = regex(cmd.param_regex_str+"[[:space:]]*", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    }
    cmds_regex_str += ")(?:[[:space:]]*$|"+wsx+"(.*))";
    cmds_regex_ = regex(cmds_regex_str, std::regex_constants::ECMAScript | std::regex_constants::optimize);
    coords_regex_ = regex(coordx+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    times_regex_ = regex(wsx+"([0-9][0-9]):([0-9][0-9]):([0-9][0-9])", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    commands_regex_ = regex("([0-9a-zA-Z_]+);?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    sizes_regex_ = regex(numx+";?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
}

void MainProgram::create_road_network()
{
    vector<pair<Coord, Coord>> addedroads;
    auto towns = ds_.all_towns();
    sort(towns.begin(), towns.end()); // Sort town IDs to get deterministic results

    // Add random road network
    using townssize = decltype(towns)::size_type;
    set<tuple<Distance, townssize, townssize>> roads;
    using roadssize = decltype(roads)::size_type;
    for (auto i1 = towns.size(); i1 != 0; --i1)
    {
        for (auto i2 = i1-1; i2 != 0; --i2)
        {
            Coord p1 = ds_.get_town_coordinates(towns[i1-1]);
            Coord p2 = ds_.get_town_coordinates(towns[i2-1]);
            Distance dist = abs(p1.x-p2.x)+abs(p1.y-p2.y);
            roads.emplace(dist, i1-1, i2-1);
        }
    }
//    shuffle(roads.begin(), roads.end(), rand_engine_);
//    sort(roads.begin(), roads.end(), [](auto l, auto r){ return get<2>(l) < get<2>(r); });
    vector<long int> tset(towns.size(), -1);
    while (!roads.empty())
    {
//        auto roadi = next(roads.begin(), random<roadssize>(0, min(static_cast<roadssize>(20), roads.size())));
//        auto roadi = roads.begin();
        auto roadi = next(roads.begin(), random<roadssize>(0, roads.size()/3));
        auto road = *roadi;
        roads.erase(roadi);
        auto i1 = get<1>(road);
        auto i2 = get<2>(road);
        if (i1 == i2) { continue; }
        auto s1 = i1;
        while (tset[s1] >= 0) { s1 = tset[s1]; }
        auto s2 = i2;
        while (tset[s2] >= 0) { s2 = tset[s2]; }
        if (s1 == s2) { continue; }
        Coord p1 = ds_.get_town_coordinates(towns[i1]);
        Coord p2 = ds_.get_town_coordinates(towns[i2]);
        bool intersects = false;
        for (auto const& road : addedroads)
        {
            if (doIntersect(p1, p2, road.first, road.second)) { intersects = true; break; }
        }
        if (intersects) { continue; }
        tset[s2] += tset[s1];
        tset[s1] = s2;
        ds_.add_road(towns[i1], towns[i2]);
        addedroads.push_back({p1, p2});
        if (tset[s2] == -static_cast<long int>(towns.size())) { break; }
    }
}

void MainProgram::add_random_nonintersecting_roads(unsigned int random_roads)
{
    vector<pair<Coord, Coord>> addedroads;
    auto towns = ds_.all_towns();
    sort(towns.begin(), towns.end()); // Sort town IDs to get deterministic results

    // Add given number of totally random roads
    for ( ; random_roads != 0; --random_roads)
    {
        auto i1 = random(towns.begin(), towns.end());
        auto i2 = random(towns.begin(), towns.end());
        if (i1 != i2)
        {
            Coord p1 = ds_.get_town_coordinates(*i1);
            Coord p2 = ds_.get_town_coordinates(*i2);
            bool intersects = false;
            for (auto const& road : addedroads)
            {
                if (doIntersect(p1, p2, road.first, road.second)) { intersects = true; break; }
            }
            if (intersects) { continue; }
            ds_.add_road(*i1, *i2);
            addedroads.push_back({p1, p2});
        }
    }
}

// The functions below are taken and modified from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// point q lies on line segment 'pr'
bool MainProgram::onSegment(Coord p, Coord q, Coord r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int MainProgram::orientation(Coord p, Coord q, Coord r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool MainProgram::doIntersect(Coord p1, Coord q1, Coord p2, Coord q2)
{
    if ((p1 == p2 && q1 == q2) || (p1 == q2 && q1 == p2)) { return true; } // Same line
    if (p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2) { return false; } // One same point, cannot intersect
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
