#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:

    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;


    // m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
     start_node = &m_Model.FindClosestNode(start_x,start_y);
     //*start_node is a Node. start_node is a pointer
     //Hence &m_Model,FindClosestNode returns the address of the result of FindClosestNode method
     end_node = &m_Model.FindClosestNode(end_x,end_y);
     std::cout << "The start node's address is :" << start_node << "\n";
     std::cout << "The end node's address is : "  << end_node << "\n";

}


// Implement the CalculateHValue method.
// - use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

  std::cout << node->distance(*end_node) << "\n";
  return node->distance(*end_node);
}


// AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value.
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

   current_node->FindNeighbors();
   for(auto v : current_node->neighbors){
     // set the parent to current node
     v->parent = current_node;
     // set the h_value from the calculate h value function
     v->h_value = CalculateHValue(v);
     v->g_value =  current_node->g_value + current_node->distance(*v);
     open_list.emplace_back(v);
     v->visited = true;
  /*   std::cout << "the current node is " << current_node << "\n";
     std::cout << "the parent of the current node" << v->parent << "\n";
     std::cout << "the h_value of the current node is " << v->h_value << "\n";
     std::cout << " the g_value of the current node is " << v->g_value << "\n";
*/
   }

}


// Complete the NextNode method to sort the open list and return the next node.
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
  // Simplify the sort here
  std::sort(open_list.begin(), open_list.end(),
          [](const RouteModel::Node* lhs, const RouteModel::Node* rhs)
{
    return lhs->h_value + lhs->g_value < rhs->h_value + rhs->g_value;
});



  // creation of pointer to the next node
  RouteModel::Node* nextnode = open_list.front();
  // removal of the data from the vector open list
  open_list.erase(open_list.begin());
  //returning the next node
  return nextnode;


}


// Complete the ConstructFinalPath method to return the final path found from your A* search.
// 
// - This method should take the current (final) node as an argument and iteratively follow the
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    // TODO: Implement your solution here.

    while(current_node != start_node)
    {
      path_found.emplace_back(*current_node);
      const RouteModel::Node parent = *(current_node->parent);
      distance += current_node->distance(parent);
      current_node = current_node->parent;
    }
    path_found.emplace_back(*current_node);
    reverse(path_found.begin(),path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {

  start_node->visited = true;
  open_list.emplace_back(start_node);
  while (!open_list.empty()){
      RouteModel::Node* current_node = NextNode();
        if(current_node->distance(*end_node) == 0){
          m_Model.path = ConstructFinalPath(current_node);
          break;
        }else{
          AddNeighbors(current_node);
              }

        }

          //compare g_value of the neighbors if it's already in the open list
               // if the neighbor is not in the open list


      // if the current_node is in the open list
          // sort the open_list vector based on the g_value
          // if current_node->g_value > any auto v : open_list then that v becomes v = curren
}
  /*  while(!open_list.empty()){
      RouteModel::Node* current_node = NextNode();

      if(current_node->distance(*end_node) == 0){
          m_Model.path = ConstructFinalPath(current_node);
      }

      AddNeighbors(current_node);

    }*/
