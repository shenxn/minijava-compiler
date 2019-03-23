#include <map>
#include <string>

class VarItem;
class MethodItem;
class ClassItem;

typedef std::map<std::string, VarItem*> VarTable;
typedef std::pair<std::string, VarItem*> VarPair;

typedef std::map<std::string, MethodItem*> MethodTable;
typedef std::pair<std::string, MethodItem*> MethodPair;