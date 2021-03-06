//
//  a_star_seed.cpp
//  AStarSeed
//
//  Created by Satya Prakash on 06/09/13.
//  Copyright (c) 2013 Satya Prakash. All rights reserved.
//


#include "a_star_seed/a_star_seed.hpp"
namespace navigation {
    std::vector<StateOfCar> AStarSeed::neighborNodesWithSeeds(StateOfCar const& currentState) {
        std::vector<StateOfCar> neighbours;
        
        for ( int i = 0; i < givenSeeds.size(); i++) {
            const double deltaX = givenSeeds[i].finalState.x();
            const double deltaY = givenSeeds[i].finalState.y();
            const double deltaZ = givenSeeds[i].finalState.theta();
            
            const int x = (int) (currentState.x() +deltaX * sin(currentState.theta() * (CV_PI / 180)) + deltaY * cos(currentState.theta() * (CV_PI / 180)));
            const int y=  (int) (currentState.y() -deltaX * cos(currentState.theta() * (CV_PI / 180)) + deltaY * sin(currentState.theta() * (CV_PI / 180)));
            
            const double theta = (int) (deltaZ - (90 - currentState.theta())) ;
            
            const StateOfCar neighbour(x,y,theta,0,givenSeeds[i].costOfseed,0,i);
            
     
            if ( !isWalkableWithSeeds(currentState, neighbour)) {
                continue;
            }
            neighbours.push_back(neighbour);
        }
        return neighbours;
    }
}
