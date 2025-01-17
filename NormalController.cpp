#include "NormalController.h"
#include "Controller.h"
#include "Food.h"
#include <algorithm>


/**
 * Store a given food object into the refrigerator. 
 * 
 * This function should follows the 'Modified Shelf First Fit (Shelf-FF) algorithm, which is given in the PPT slide.
 * Your implemented algorithm should find the proper position of the new food object.
 * The new food object should be initialized in the heap memory and referenced from both food_list and shelves.
 * 
 * @param name The name of the food to be stored
 * @param size width and height of the food
 * @param exp the expire date of the food
 * @return true if inserting the food for both food_list and shelves has been succeed
 */
bool NormalController::stackFood(const string name, intPair foodSize, int exp)
{
    Food food_to_insert(name, foodSize, exp);
    bool inserted=false;
    //check if space avail on shelves
    int avail_x, avail_y;
    int avail_space_start_x, avail_space_end_x;
    int shelf_height=0;
    for(auto &each_shelf : shelves){//for each shelf
        int counter=0;
        for(auto &each_foodptr : each_shelf.vec){//for each foodptr/foodinfridge that is in the shelf
            if(counter==0){//if first item in shelf
                avail_space_start_x=0;//starting avail x space starts from 0
            }
            else{//if not first item in shelf
                auto prev_element = *(&each_foodptr+1);
                avail_space_start_x=prev_element->getPos().first + prev_element->getSize().first;//starting x pos starts
                //previous element's ending pos
            }
            counter+=1;

            if(counter==each_shelf.vec.size()-1){//if last element of vector
                avail_space_end_x=size.first;//set end_x the 'wall' of fridge
            }
            else{
                avail_space_end_x=each_foodptr->getPos().first;
            }
            avail_x=avail_space_start_x-avail_space_end_x;
            if(avail_x>foodSize.first){//enough space in x, assume height of shelf>height of food always

                //insert food into shelf @ avail_space_start_x
                FoodPtr new_food=new FoodInFridge(food_to_insert, avail_space_end_x, shelf_height);//using heap memory
                each_shelf.vec.push_back(new_food);//food added to shelf
                
                if(foodList.find(name)==foodList.end()){//food does not exist in foodlist
                    vector<FoodPtr> v{new_food};
                    foodList.insert(make_pair(name, v));//add the new food to foodlist
                }
                else{//food already exsists in foodList
                    foodList[name].push_back(new_food);//append foodptr to foodlist
                }
                inserted=true;
            }
        }
        shelf_height+=each_shelf.height;
    }
    return inserted;
}
 

/**
 * Pop a food with shortest expire date from both foodList and shelves.
 * This function does not return the object.
 * The pop process should follows the algorithm in PPT slide.
 * 
 * @param food_name a name of food to be popped
 * @return 
 */
bool NormalController::popFood(const string food_name)
{
    auto food_name_iterator=findMinExpFood(food_name);
    if(food_name_iterator==foodList[food_name].end()){//food not found in shelves
        return false;
    }
    //if food found, and
    //assuming now the food is inside the foodList
    //find location of food from food_name_iterator

    auto food_location = (*food_name_iterator)->getPos();
    //get the food location in shelves
    int food_x = food_location.first;
    int food_y = food_location.second;

    //erase the food at the location from shelves
    for(auto &shelf_element : shelves){
        for(auto it = shelf_element.vec.begin(); it!=shelf_element.vec.end(); it++){
            if((*it)->getPos().second==food_y && (*it)->getPos().first==food_x){
                shelf_element.vec.erase(it);
            }
        }
    }
    
    //erase the food from foodList
    foodList[food_name].erase(food_name_iterator);
    return true;
}
