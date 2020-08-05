// identification comments code block
// Programmer: Iziren Okhamafe
// Programmer's ID: 0997910

//Pre-processor directive C++
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<set>
using namespace std;

//C Preprocessors
#include<cstring>

//Function prototypes
ostream& precisionTwoDecimals(ostream&);
ostream& turnOffFloatingPoint(ostream&);

class Leg {
    private:
        const char* const startCity;
        const char* const endCity;
        const double distance;

    public:
        Leg(const char* const, const char* const, const double);
        Leg& operator=(const Leg&);
        double getDistance() const;
        void output(ostream&) const;
        friend class Route;
        friend class ShortestRoute;
};

class Route {
    private:
        vector<const Leg*> legs; // bag of legs
        const double totalDist;
    public:
        Route(const Leg&);
        Route(const Route&, const Leg&);
        Route& operator=(const Route&);
        double getDistance() const {return totalDist;}
        void output(ostream&) const;
        friend class ShortestRoute;
        friend bool operator < (const Route&, const Route&);
};

class ShortestRoute {
    private:
        static const int legSize;
        static const Leg edges[];
    public:
        static const Route anyRoute(const char* const, const char* const);
        static const Route shortestRoute(const char* const, const char* const);
};

const Leg ShortestRoute::edges[] = {
        Leg("Collerville", "Orlando", 684),

        Leg("Kinsey", "Dilworth", 478),

        Leg("Dilworth", "Minneapolis", 214),

        Leg("Dilworth", "Naperville", 569),

        Leg("Denver", "Lincoln", 484),

        Leg("Denver", "Oklahoma City", 504),

        Leg("Collerville", "Washington", 763),

        Leg("New Orleans", "Orlando", 533),

        Leg("San Francisco", "Las Angeles", 352),

        Leg("Minneapolis", "Akron", 630),

        Leg("Naperville", "Akron", 307),

        Leg("Akron", "Pittsburgh", 115),

        Leg("Pittsburgh", "New York City", 315),

        Leg("Naperville", "Pittsburgh", 409),

        Leg("San Francisco", "Portland", 536),

        Leg("Reno", "Salt Lake City", 427),

        Leg("Las Angeles", "Phoenix", 357),

        Leg("San Francisco", "Reno", 186),

        Leg("El Paso", "Houston", 674),

        Leg("El Paso", "Oklahoma City", 576),

        Leg("Montana City", "Kinsey", 294),

        Leg("Montana City", "Dilworth", 721),

        Leg("Kinsey", "Lincoln", 606),

        Leg("Reno", "Phoenix", 601),

        Leg("Portland", "Montana City", 516),

        Leg("Salt Lake City", "Denver", 389),

        Leg("Salt Lake City", "Kinsey", 494),

        Leg("Lincoln", "Naperville", 435),

        Leg("Lincoln", "St. Louis", 357),

        Leg("Lincoln", "Collerville", 531),

        Leg("Oklahoma City", "Collerville", 421),

        Leg("Houston", "Collerville", 485),

        Leg("Houston", "New Orleans", 318),

        Leg("St. Louis", "Philadelphia", 809),

        Leg("Reno", "Phoenix", 601),

        Leg("Phoenix", "Denver", 586),

        Leg("Phoenix", "El Paso", 345),

        Leg("Naperville", "Philadelphia", 664),

        Leg("Collerville", "Philadelphia", 879),

        Leg("Orlando", "Washington", 759),

        Leg("Washington", "New York City", 203),

        Leg("Philadelphia", "New York City", 80),

        Leg("San Francisco", "New York City", 21000)
};
const int ShortestRoute::legSize = (sizeof(edges) / sizeof(edges[0]));


//Constructor implementation 
Leg::Leg(const char* const startCity, const char* const endCity, const double distance) : startCity(startCity), endCity(endCity), distance(distance) {
}

Route::Route(const Leg &aLeg) : totalDist(aLeg.distance){
    this->legs.push_back(&aLeg);
}

Route::Route(const Route &previousRoute, const Leg &aLeg) : totalDist(strcmp(previousRoute.legs.back()->endCity, aLeg.startCity) != 0 ? previousRoute.getDistance() : previousRoute.getDistance() + aLeg.distance){
    legs = previousRoute.legs; /*copy first parameter's bag into host object's bag*/
    try
    {
        if(strcmp(previousRoute.legs.back()->endCity, aLeg.startCity) != 0)
            throw "Cities don't match!";
        legs.push_back(&aLeg); // add new leg to route
    }
    catch(const char* error)
    {
        cout << error << endl;
    }
}

const Route ShortestRoute::anyRoute(const char* const startCity, const char* const endCity) {
    const Leg *someEdge;
    //cout << legSize << '\n';
    for (int i=0; i < legSize; i++) {
        if((strcmp(endCity, edges[i].endCity) == 0) && (strcmp(startCity, edges[i].startCity) == 0)) {
            //Return One Parameter Constructor
            return Route(edges[i]);
        } else if(strcmp(endCity, edges[i].endCity) == 0) {
            someEdge = &edges[i]; //set someEdge to be the memory address of endCity in Legs Array
            break; // exit loop because there are many "New York City" in Legs (edges)
        } 
    }  
    if(!someEdge) throw "No next city found to contine!"; 
    return Route(ShortestRoute::anyRoute(startCity, someEdge->startCity), *someEdge);
}

const Route ShortestRoute::shortestRoute(const char* const start, const char* const end) 
{ 
    
    set<Route> allRoutes;// auto sort
    for(int i = 0; i < ShortestRoute::legSize; i++)
    { // compare the endCityof the leg from the input parameter, end
        if(strcmp(ShortestRoute::edges[i].endCity, end) == 0) 
        {// compare the startCityof the leg from the input parameter, start
            if(strcmp(ShortestRoute::edges[i].startCity, start) == 0) 
            {
                // create a route object using the found leg, and insert it into the set
                Route newRoute(edges[i]);
                allRoutes.insert(newRoute);
            }
            else 
            {
            //recursive calls to receive a route object with the input parameter, start
            // and the startCityof the found leg as the input parameter, then insert it into the set
                Route someRandomRoute(shortestRoute(start, edges[i].startCity), edges[i]);
                allRoutes.insert(someRandomRoute);
            }
        }
    }

 if (!allRoutes.empty()) return *(allRoutes.begin());
    throw "Can't find the shortest route!";
}

//Assignment Operator
Leg& Leg::operator=(const Leg& copyThis) {
    Leg& host = *this;
    if (this != &copyThis) {
        const_cast<double&>(host.distance) = copyThis.distance;
        const_cast<const char* &>(host.startCity) = copyThis.startCity;
        const_cast<const char* &>(host.endCity) = copyThis.endCity;
    }
    return host; //returns a self-reference
}

Route& Route::operator=(const Route& copyThis) {
    Route& host = *this;
    if (this != &copyThis) {
        const_cast<double&>(host.totalDist) = copyThis.totalDist;
        host.legs = copyThis.legs;
    }
    return host; //returns a self-reference
}

bool operator < (const Route &routeOne, const Route &routeTwo) {
    return routeOne.getDistance() < routeTwo.getDistance();
}
//getDistance 
double::Leg::getDistance() const {
    return this->distance;
}



int main() {
    // identification output code block
    cout << "Programmer: Iziren Okhamafe\n";
    cout << "Programmer's ID: 0997910\n";
    cout << "File: " << __FILE__ << endl;
    cout << endl;

    cout << "Any Route:\n";
    const Route route1 = ShortestRoute::anyRoute("San Francisco", "New York City");
    route1.output(cout);
    
    cout << "\n";

    cout << "Shortest Route:\n";
    const Route route2 = ShortestRoute::shortestRoute("San Francisco", "New York City");
    route2.output(cout);

    return 0;
}


void Leg::output(ostream& out) const {
    out << "Leg: " << startCity << " to " << endCity
        << ", " << precisionTwoDecimals << distance << " miles." << endl;

}

void Route::output(ostream& out) const {
    out << "Route: From ";
    for (int i = 0; i < legs.size(); i++) {
        out << legs[i]->startCity << " to ";
    }
        out << legs.back()->endCity;
        out << " is " << precisionTwoDecimals << totalDist << turnOffFloatingPoint << " miles.";
        out << endl;    
}

ostream& precisionTwoDecimals(ostream& out) {
    out.setf(ios::fixed);
    out.precision(2); // 2 decimal digits
    return out;
}

ostream& turnOffFloatingPoint(ostream& out) {
    out.unsetf(ios::fixed); 
    out.precision(6); // the C++ default 
    return out;
}                              