#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <qstring.h>
#include <vector>

class repository
{
public:
    repository();
    void generateDBIfNotExist();
    std::vector<QString> getNames();
    std::vector<QString> getTasks(int id);
};

#endif // REPOSITORY_H
