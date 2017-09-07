#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <map>

 class Configuration {

   public:
     static std::map<int, std::string> getAnnotation(){
         std::map<int, std::string> ANNOTATIONS;
         ANNOTATIONS.insert(std::pair<int, std::string>(0,"Car"));
         ANNOTATIONS.insert(std::pair<int, std::string>(1,"Lane"));
         return ANNOTATIONS;
     }
        static std::map<int, std::string> getShapes (){
            std::map<int, std::string> SHAPES;
            SHAPES.insert(std::pair<int, std::string>(0,"Square"));
            SHAPES.insert(std::pair<int, std::string>(1,"Brush"));
            return SHAPES;
        }
        static std::map<int, std::string>  getCursors (){
            std::map<int, std::string> CURSOR;
            CURSOR.insert(std::pair<int, std::string>(0, "Cross"));
            CURSOR.insert(std::pair<int, std::string>(1,"Circle"));
            return CURSOR;
        }

};



//enum SHAPES {Square, Brush};
//enum CURSOR {Cross, Circle};


#endif // CONFIGURATION_H
